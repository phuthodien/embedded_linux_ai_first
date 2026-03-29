/*
 * am335x_uart.c - UART driver for AM335x SoC (BeagleBone Black)
 *
 * Educational driver following the Linux serial subsystem pattern.
 * Template: stm32-usart.c, adapted for AM335x register model.
 *
 * Target: UART1 on BBB (base 0x48022000, IRQ 73, P9_24 TXD / P9_26 RXD)
 * Creates /dev/ttyAM0 via uart_driver + platform_driver.
 *
 * Key difference from STM32: AM335x uses LCR-based register mode switching.
 * The same offset maps to different registers depending on LCR[7] value:
 *   LCR = 0x00..0x7F  -> Operational mode (RHR/THR, IER, IIR)
 *   LCR = 0x80        -> Config Mode A   (DLL, DLH)
 *   LCR = 0xBF        -> Config Mode B   (EFR, XON/XOFF)
 *
 * Reference: AM335x TRM (SPRUH73Q) Chapter 19 - UART
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/serial_core.h>
#include <linux/serial.h>
#include <linux/tty.h>
#include <linux/tty_flip.h>
#include <linux/clk.h>
#include <linux/io.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/delay.h>
#include <linux/console.h>

#define DRIVER_NAME	"am335x_uart"
#define DEV_NAME	"ttyAM"
#define AM335X_UART_NR	1	/* single port for training */

/* Custom port type (must not collide with PORT_* in serial_core.h) */
#define PORT_AM335X	120

/*
 * ============================================================
 * Register offsets (TRM ch19, Table 19-5)
 *
 * Many offsets are shared between different registers,
 * accessed via LCR mode switching.
 * ============================================================
 */

/* Offset 0x00: RHR (read, operational), THR (write, operational), DLL (config A/B) */
#define AM335X_UART_RHR	0x00
#define AM335X_UART_THR	0x00
#define AM335X_UART_DLL	0x00

/* Offset 0x04: IER (operational), DLH (config A/B) */
#define AM335X_UART_IER	0x04
#define AM335X_UART_DLH	0x04

/* Offset 0x08: IIR (read, operational), FCR (write, operational), EFR (config B) */
#define AM335X_UART_IIR	0x08
#define AM335X_UART_FCR	0x08
#define AM335X_UART_EFR	0x08

/* Offset 0x0C..0x1C: always accessible */
#define AM335X_UART_LCR	0x0C
#define AM335X_UART_MCR	0x10
#define AM335X_UART_LSR	0x14
#define AM335X_UART_MSR	0x18
#define AM335X_UART_SPR	0x1C
#define AM335X_UART_TLR	0x1C	/* when TCR_TLR submode active */

/* Mode definition and supplementary */
#define AM335X_UART_MDR1	0x20
#define AM335X_UART_SCR	0x40
#define AM335X_UART_SSR	0x44

/* System control */
#define AM335X_UART_SYSC	0x54
#define AM335X_UART_SYSS	0x58

/* FIFO level (read-only, useful for debug) */
#define AM335X_UART_RXFIFO_LVL	0x64
#define AM335X_UART_TXFIFO_LVL	0x68

/*
 * ============================================================
 * Bit definitions
 * ============================================================
 */

/* IER bits (operational mode) */
#define AM335X_UART_IER_RHR	BIT(0)	/* RX data available */
#define AM335X_UART_IER_THR	BIT(1)	/* TX FIFO ready */
#define AM335X_UART_IER_LSR	BIT(2)	/* Line status error */
#define AM335X_UART_IER_MSR	BIT(3)	/* Modem status change */

/* IIR bits */
#define AM335X_UART_IIR_NO_INT	BIT(0)	/* 1 = no interrupt pending */
#define AM335X_UART_IIR_TYPE_MASK	0x3E	/* IT_TYPE bits [5:1] */

/* IIR IT_TYPE values (bits [5:1], already shifted) */
#define AM335X_IIR_MODEM	0x00	/* Modem status */
#define AM335X_IIR_THR		0x02	/* TX FIFO empty */
#define AM335X_IIR_RHR		0x04	/* RX data available */
#define AM335X_IIR_RXERR	0x06	/* RX line status error */
#define AM335X_IIR_RXTIMEOUT	0x0C	/* RX timeout */

/* FCR bits (write-only) */
#define AM335X_UART_FCR_FIFO_EN	BIT(0)
#define AM335X_UART_FCR_RX_CLR	BIT(1)
#define AM335X_UART_FCR_TX_CLR	BIT(2)
#define AM335X_UART_FCR_RX_TRIG_8	(0 << 6)	/* RX trigger at 8 chars */
#define AM335X_UART_FCR_TX_TRIG_8	(0 << 4)	/* TX trigger at 8 spaces */

/* LCR register */
#define AM335X_UART_LCR_CHAR_5	0x00
#define AM335X_UART_LCR_CHAR_6	0x01
#define AM335X_UART_LCR_CHAR_7	0x02
#define AM335X_UART_LCR_CHAR_8	0x03
#define AM335X_UART_LCR_STOP2	BIT(2)	/* 2 stop bits (else 1) */
#define AM335X_UART_LCR_PARITY	BIT(3)	/* enable parity */
#define AM335X_UART_LCR_EVEN	BIT(4)	/* even parity (else odd) */
#define AM335X_UART_LCR_FIXED	BIT(5)	/* fixed parity (mark/space) */
#define AM335X_UART_LCR_BREAK	BIT(6)	/* break control */

/* LCR mode selection values */
#define AM335X_LCR_MODE_OP	0x00	/* operational mode */
#define AM335X_LCR_MODE_A	0x80	/* config mode A */
#define AM335X_LCR_MODE_B	0xBF	/* config mode B */

/* MCR bits */
#define AM335X_UART_MCR_DTR	BIT(0)
#define AM335X_UART_MCR_RTS	BIT(1)
#define AM335X_UART_MCR_LOOP	BIT(4)	/* loopback mode */
#define AM335X_UART_MCR_TCR_TLR	BIT(6)	/* enable TCR/TLR access */

/* LSR bits (read-only) */
#define AM335X_UART_LSR_RXFIFOE	BIT(0)	/* RX FIFO data available */
#define AM335X_UART_LSR_OE	BIT(1)	/* overrun error */
#define AM335X_UART_LSR_PE	BIT(2)	/* parity error */
#define AM335X_UART_LSR_FE	BIT(3)	/* framing error */
#define AM335X_UART_LSR_BI	BIT(4)	/* break interrupt */
#define AM335X_UART_LSR_TXFIFOE	BIT(5)	/* TX FIFO empty */
#define AM335X_UART_LSR_TXSRE	BIT(6)	/* TX shift register empty */

/* MSR bits (read-only) */
#define AM335X_UART_MSR_DCTS	BIT(0)	/* delta CTS */
#define AM335X_UART_MSR_DDSR	BIT(1)	/* delta DSR */
#define AM335X_UART_MSR_DRI	BIT(2)	/* delta RI */
#define AM335X_UART_MSR_DDCD	BIT(3)	/* delta DCD */
#define AM335X_UART_MSR_CTS	BIT(4)
#define AM335X_UART_MSR_DSR	BIT(5)
#define AM335X_UART_MSR_RI	BIT(6)
#define AM335X_UART_MSR_DCD	BIT(7)

/* EFR bits (config mode B only) */
#define AM335X_UART_EFR_ENHANCED	BIT(4)

/* MDR1 bits */
#define AM335X_UART_MDR1_DISABLE	0x07	/* disable UART */
#define AM335X_UART_MDR1_UART16X	0x00	/* UART 16x mode */
#define AM335X_UART_MDR1_UART13X	0x03	/* UART 13x mode */
#define AM335X_UART_MDR1_MODE_MASK	0x07

/* SSR bits */
#define AM335X_UART_SSR_TXFULL	BIT(0)	/* TX FIFO full */

/* SYSC bits */
#define AM335X_UART_SYSC_SOFTRESET	BIT(1)

/* SYSS bits */
#define AM335X_UART_SYSS_RESETDONE	BIT(0)

#define AM335X_UART_FIFO_SIZE	64

/*
 * ============================================================
 * Driver-private structure
 * ============================================================
 */
struct am335x_uart_port {
	struct uart_port	port;
	struct clk		*fclk;		/* functional clock, 48 MHz */
	u8			ier;		/* shadow of IER */
	u8			lcr;		/* shadow of LCR (frame format) */
	u8			mcr;		/* shadow of MCR */
	u8			efr;		/* shadow of EFR */
};

static struct am335x_uart_port *am335x_ports[AM335X_UART_NR];

/*
 * ============================================================
 * Register access helpers
 *
 * AM335x UART registers are 32-bit wide but only lower 8 bits
 * are meaningful. We use readl/writel for memory-mapped access.
 * ============================================================
 */
static inline u32 uart_read(struct am335x_uart_port *up, u32 reg)
{
	return readl(up->port.membase + reg);
}

static inline void uart_write(struct am335x_uart_port *up, u32 reg, u32 val)
{
	writel(val, up->port.membase + reg);
}

/*
 * Mode switching helpers.
 *
 * The AM335x UART uses LCR[7] to select which registers are visible
 * at shared offsets (0x00, 0x04, 0x08). Before accessing DLL/DLH or
 * EFR, we must set LCR to the appropriate mode and restore it after.
 */
static u8 am335x_uart_set_mode(struct am335x_uart_port *up, u8 mode)
{
	u8 old_lcr = uart_read(up, AM335X_UART_LCR);

	uart_write(up, AM335X_UART_LCR, mode);
	return old_lcr;
}

static void am335x_uart_restore_mode(struct am335x_uart_port *up, u8 lcr)
{
	uart_write(up, AM335X_UART_LCR, lcr);
}

/*
 * Write IER safely. In operational mode, IER[3:0] are always writable.
 * IER[7:4] require EFR[4]=1 (enhanced mode).
 * We only use IER[3:0] in this driver, so direct write is fine.
 */
static void am335x_uart_write_ier(struct am335x_uart_port *up, u8 ier)
{
	u8 old_lcr;

	/* Ensure operational mode for IER access */
	old_lcr = am335x_uart_set_mode(up, AM335X_LCR_MODE_OP);
	uart_write(up, AM335X_UART_IER, ier);
	up->ier = ier;
	am335x_uart_restore_mode(up, old_lcr);
}

/*
 * ============================================================
 * Hardware initialization (TRM 19.6.1 - 19.6.3)
 * ============================================================
 */

/* Step 1: Software reset (TRM 19.6.1) */
static int am335x_uart_soft_reset(struct am335x_uart_port *up)
{
	int timeout = 100;

	uart_write(up, AM335X_UART_SYSC, AM335X_UART_SYSC_SOFTRESET);

	while (!(uart_read(up, AM335X_UART_SYSS) &
		 AM335X_UART_SYSS_RESETDONE)) {
		if (--timeout == 0) {
			dev_err(up->port.dev, "soft reset timeout\n");
			return -ETIMEDOUT;
		}
		udelay(10);
	}
	return 0;
}

/*
 * Step 2: FIFO configuration (TRM 19.6.2)
 *
 * The FIFO setup requires a precise register access dance:
 *   1. Switch to config mode B, enable enhanced features (EFR[4]=1)
 *   2. Switch to config mode A, enable TCR_TLR access (MCR[6]=1)
 *   3. Write FCR (FIFO enable, clear, trigger levels)
 *   4. Restore MCR[6], EFR[4], LCR
 */
static void am335x_uart_fifo_config(struct am335x_uart_port *up)
{
	u8 old_lcr, efr, mcr;

	/* Config mode B -> enable enhanced features */
	old_lcr = am335x_uart_set_mode(up, AM335X_LCR_MODE_B);
	efr = uart_read(up, AM335X_UART_EFR);
	uart_write(up, AM335X_UART_EFR, efr | AM335X_UART_EFR_ENHANCED);

	/* Config mode A -> enable TCR_TLR access */
	am335x_uart_set_mode(up, AM335X_LCR_MODE_A);
	mcr = uart_read(up, AM335X_UART_MCR);
	uart_write(up, AM335X_UART_MCR, mcr | AM335X_UART_MCR_TCR_TLR);

	/* Write FCR: enable FIFOs, clear both, RX trigger=8, TX trigger=8 */
	uart_write(up, AM335X_UART_FCR,
		   AM335X_UART_FCR_FIFO_EN |
		   AM335X_UART_FCR_RX_CLR |
		   AM335X_UART_FCR_TX_CLR |
		   AM335X_UART_FCR_RX_TRIG_8 |
		   AM335X_UART_FCR_TX_TRIG_8);

	/* Restore MCR[6] */
	uart_write(up, AM335X_UART_MCR, mcr);

	/* Config mode B -> restore EFR */
	am335x_uart_set_mode(up, AM335X_LCR_MODE_B);
	uart_write(up, AM335X_UART_EFR, efr);

	/* Restore original LCR */
	am335x_uart_restore_mode(up, old_lcr);
}

/*
 * Step 3: Baud rate and frame format (TRM 19.6.3)
 *
 * Baud rate formula (16x mode):
 *   divisor = UART_CLK / (16 * baud_rate)
 *   DLH = divisor >> 8, DLL = divisor & 0xFF
 *
 * The protocol config sequence:
 *   1. Disable UART (MDR1=0x7)
 *   2. Config mode B, enable enhanced, config mode operational, clear IER
 *   3. Write DLL/DLH in config mode A (or B)
 *   4. Restore IER, enhanced, write LCR frame format
 *   5. Enable UART (MDR1=0x0)
 */
static void am335x_uart_set_baud(struct am335x_uart_port *up,
				  unsigned int baud, u8 lcr_val)
{
	u8 old_lcr, efr;
	unsigned int divisor;

	divisor = DIV_ROUND_CLOSEST(up->port.uartclk, 16 * baud);
	if (divisor == 0)
		divisor = 1;

	/* 1. Disable UART */
	uart_write(up, AM335X_UART_MDR1, AM335X_UART_MDR1_DISABLE);

	/* 2. Config mode B -> save and enable enhanced features */
	old_lcr = am335x_uart_set_mode(up, AM335X_LCR_MODE_B);
	efr = uart_read(up, AM335X_UART_EFR);
	uart_write(up, AM335X_UART_EFR, efr | AM335X_UART_EFR_ENHANCED);

	/* Switch to operational mode, clear IER */
	am335x_uart_set_mode(up, AM335X_LCR_MODE_OP);
	uart_write(up, AM335X_UART_IER, 0x00);

	/* 3. Config mode A -> write divisor latch */
	am335x_uart_set_mode(up, AM335X_LCR_MODE_A);
	uart_write(up, AM335X_UART_DLL, divisor & 0xFF);
	uart_write(up, AM335X_UART_DLH, (divisor >> 8) & 0x3F);

	/* 4. Operational mode -> restore IER */
	am335x_uart_set_mode(up, AM335X_LCR_MODE_OP);
	uart_write(up, AM335X_UART_IER, up->ier);

	/* Config mode B -> restore EFR */
	am335x_uart_set_mode(up, AM335X_LCR_MODE_B);
	uart_write(up, AM335X_UART_EFR, efr);

	/* Write LCR with frame format (exits config mode B) */
	uart_write(up, AM335X_UART_LCR, lcr_val);
	up->lcr = lcr_val;

	/* 5. Enable UART 16x mode */
	uart_write(up, AM335X_UART_MDR1, AM335X_UART_MDR1_UART16X);
}

/*
 * ============================================================
 * RX / TX character handling
 * ============================================================
 */

static void am335x_uart_handle_rx(struct am335x_uart_port *up)
{
	struct uart_port *port = &up->port;
	struct tty_port *tport = &port->state->port;
	u32 lsr;
	u8 ch;
	unsigned int flag;

	lsr = uart_read(up, AM335X_UART_LSR);

	while (lsr & AM335X_UART_LSR_RXFIFOE) {
		ch = uart_read(up, AM335X_UART_RHR);
		flag = TTY_NORMAL;
		port->icount.rx++;

		/* Check for errors */
		if (lsr & AM335X_UART_LSR_BI) {
			port->icount.brk++;
			if (uart_handle_break(port))
				goto next;
			flag = TTY_BREAK;
		} else if (lsr & AM335X_UART_LSR_PE) {
			port->icount.parity++;
			flag = TTY_PARITY;
		} else if (lsr & AM335X_UART_LSR_FE) {
			port->icount.frame++;
			flag = TTY_FRAME;
		}

		if (lsr & AM335X_UART_LSR_OE)
			port->icount.overrun++;

		/* Apply read_status_mask and ignore_status_mask */
		lsr &= port->read_status_mask;

		if (lsr & AM335X_UART_LSR_BI)
			flag = TTY_BREAK;
		else if (lsr & AM335X_UART_LSR_PE)
			flag = TTY_PARITY;
		else if (lsr & AM335X_UART_LSR_FE)
			flag = TTY_FRAME;

		uart_insert_char(port, lsr, AM335X_UART_LSR_OE, ch, flag);
next:
		lsr = uart_read(up, AM335X_UART_LSR);
	}

	tty_flip_buffer_push(tport);
}

static void am335x_uart_handle_tx(struct am335x_uart_port *up)
{
	struct uart_port *port = &up->port;
	struct circ_buf *xmit = &port->state->xmit;
	int count;

	/* Handle x_char (high-priority XON/XOFF) */
	if (port->x_char) {
		uart_write(up, AM335X_UART_THR, port->x_char);
		port->icount.tx++;
		port->x_char = 0;
		return;
	}

	/* Nothing to send or TX stopped */
	if (uart_circ_empty(xmit) || uart_tx_stopped(port)) {
		up->ier &= ~AM335X_UART_IER_THR;
		am335x_uart_write_ier(up, up->ier);
		return;
	}

	/* Fill TX FIFO (up to 64 bytes) */
	count = AM335X_UART_FIFO_SIZE;
	while (count > 0 && !uart_circ_empty(xmit)) {
		/* Check TX FIFO not full */
		if (uart_read(up, AM335X_UART_SSR) & AM335X_UART_SSR_TXFULL)
			break;

		uart_write(up, AM335X_UART_THR, xmit->buf[xmit->tail]);
		xmit->tail = (xmit->tail + 1) & (UART_XMIT_SIZE - 1);
		port->icount.tx++;
		count--;
	}

	if (uart_circ_chars_pending(xmit) < WAKEUP_CHARS)
		uart_write_wakeup(port);

	if (uart_circ_empty(xmit)) {
		up->ier &= ~AM335X_UART_IER_THR;
		am335x_uart_write_ier(up, up->ier);
	}
}

/*
 * ============================================================
 * Interrupt handler
 *
 * AM335x UART interrupt identification (IIR register):
 *   IIR[0] = 1 means NO interrupt pending
 *   IIR[5:1] = IT_TYPE identifies the source
 *
 * Priority (highest first):
 *   1. Line status error (IT_TYPE = 0x03, shifted = 0x06)
 *   2. RX data available (IT_TYPE = 0x02, shifted = 0x04)
 *   3. RX timeout       (IT_TYPE = 0x06, shifted = 0x0C)
 *   4. TX FIFO empty    (IT_TYPE = 0x01, shifted = 0x02)
 *   5. Modem status     (IT_TYPE = 0x00, shifted = 0x00)
 * ============================================================
 */
static irqreturn_t am335x_uart_irq(int irq, void *dev_id)
{
	struct am335x_uart_port *up = dev_id;
	struct uart_port *port = &up->port;
	u32 iir;
	unsigned int handled = 0;
	unsigned long flags;

	spin_lock_irqsave(&port->lock, flags);

	iir = uart_read(up, AM335X_UART_IIR);

	while (!(iir & AM335X_UART_IIR_NO_INT)) {
		handled = 1;

		switch (iir & AM335X_UART_IIR_TYPE_MASK) {
		case AM335X_IIR_RXERR:	/* RX line status error */
			am335x_uart_handle_rx(up);
			break;

		case AM335X_IIR_RHR:		/* RX data available */
		case AM335X_IIR_RXTIMEOUT:	/* RX timeout */
			am335x_uart_handle_rx(up);
			break;

		case AM335X_IIR_THR:		/* TX FIFO ready */
			am335x_uart_handle_tx(up);
			break;

		case AM335X_IIR_MODEM:		/* Modem status change */
			/* Read MSR to clear the interrupt */
			uart_read(up, AM335X_UART_MSR);
			break;
		}

		iir = uart_read(up, AM335X_UART_IIR);
	}

	spin_unlock_irqrestore(&port->lock, flags);

	return handled ? IRQ_HANDLED : IRQ_NONE;
}

/*
 * ============================================================
 * uart_ops implementation
 *
 * These functions are called by the serial core (serial_core.c)
 * in response to userspace operations on /dev/ttyAM0.
 * ============================================================
 */

static unsigned int am335x_uart_tx_empty(struct uart_port *port)
{
	struct am335x_uart_port *up =
		container_of(port, struct am335x_uart_port, port);
	u32 lsr;

	lsr = uart_read(up, AM335X_UART_LSR);
	return (lsr & (AM335X_UART_LSR_TXFIFOE | AM335X_UART_LSR_TXSRE)) ==
		(AM335X_UART_LSR_TXFIFOE | AM335X_UART_LSR_TXSRE) ?
		TIOCSER_TEMT : 0;
}

static void am335x_uart_set_mctrl(struct uart_port *port, unsigned int mctrl)
{
	struct am335x_uart_port *up =
		container_of(port, struct am335x_uart_port, port);
	u8 mcr = 0;

	if (mctrl & TIOCM_RTS)
		mcr |= AM335X_UART_MCR_RTS;
	if (mctrl & TIOCM_DTR)
		mcr |= AM335X_UART_MCR_DTR;
	if (mctrl & TIOCM_LOOP)
		mcr |= AM335X_UART_MCR_LOOP;

	up->mcr = mcr;
	uart_write(up, AM335X_UART_MCR, mcr);
}

static unsigned int am335x_uart_get_mctrl(struct uart_port *port)
{
	struct am335x_uart_port *up =
		container_of(port, struct am335x_uart_port, port);
	u32 msr;
	unsigned int mctrl = 0;

	msr = uart_read(up, AM335X_UART_MSR);

	if (msr & AM335X_UART_MSR_CTS)
		mctrl |= TIOCM_CTS;
	if (msr & AM335X_UART_MSR_DSR)
		mctrl |= TIOCM_DSR;
	if (msr & AM335X_UART_MSR_RI)
		mctrl |= TIOCM_RI;
	if (msr & AM335X_UART_MSR_DCD)
		mctrl |= TIOCM_CD;

	return mctrl;
}

static void am335x_uart_stop_tx(struct uart_port *port)
{
	struct am335x_uart_port *up =
		container_of(port, struct am335x_uart_port, port);

	up->ier &= ~AM335X_UART_IER_THR;
	am335x_uart_write_ier(up, up->ier);
}

static void am335x_uart_start_tx(struct uart_port *port)
{
	struct am335x_uart_port *up =
		container_of(port, struct am335x_uart_port, port);

	up->ier |= AM335X_UART_IER_THR;
	am335x_uart_write_ier(up, up->ier);
}

static void am335x_uart_stop_rx(struct uart_port *port)
{
	struct am335x_uart_port *up =
		container_of(port, struct am335x_uart_port, port);

	up->ier &= ~(AM335X_UART_IER_RHR | AM335X_UART_IER_LSR);
	am335x_uart_write_ier(up, up->ier);
}

/*
 * startup - called when userspace opens /dev/ttyAM0
 *
 * Performs the full AM335x UART initialization sequence:
 *   1. Soft reset
 *   2. FIFO configuration
 *   3. Default baud rate (115200) and frame format (8N1)
 *   4. Register interrupt handler
 *   5. Enable RX interrupts
 */
static int am335x_uart_startup(struct uart_port *port)
{
	struct am335x_uart_port *up =
		container_of(port, struct am335x_uart_port, port);
	int ret;

	/* Soft reset */
	ret = am335x_uart_soft_reset(up);
	if (ret)
		return ret;

	/* Configure FIFOs */
	am335x_uart_fifo_config(up);

	/* Set default 115200 8N1 */
	up->ier = 0;
	am335x_uart_set_baud(up, 115200, AM335X_UART_LCR_CHAR_8);

	/* Register interrupt handler */
	ret = request_irq(port->irq, am335x_uart_irq, IRQF_SHARED,
			  DRIVER_NAME, up);
	if (ret) {
		dev_err(port->dev, "failed to request IRQ %d: %d\n",
			port->irq, ret);
		return ret;
	}

	/* Enable RX and line status interrupts */
	up->ier = AM335X_UART_IER_RHR | AM335X_UART_IER_LSR;
	am335x_uart_write_ier(up, up->ier);

	return 0;
}

/*
 * shutdown - called when userspace closes /dev/ttyAM0
 */
static void am335x_uart_shutdown(struct uart_port *port)
{
	struct am335x_uart_port *up =
		container_of(port, struct am335x_uart_port, port);

	/* Disable all interrupts */
	up->ier = 0;
	am335x_uart_write_ier(up, 0);

	/* Free interrupt handler */
	free_irq(port->irq, up);

	/* Disable UART */
	uart_write(up, AM335X_UART_MDR1, AM335X_UART_MDR1_DISABLE);
}

/*
 * set_termios - configure baud rate, data bits, parity, stop bits
 *
 * Called by serial core when userspace changes terminal settings
 * (e.g., stty -F /dev/ttyAM0 9600 cs7 parenb).
 *
 * Must follow TRM 19.6.3 protocol configuration sequence.
 */
static void am335x_uart_set_termios(struct uart_port *port,
				     struct ktermios *termios,
				     struct ktermios *old)
{
	struct am335x_uart_port *up =
		container_of(port, struct am335x_uart_port, port);
	unsigned int baud;
	u8 lcr = 0;
	unsigned long flags;

	/* Calculate baud rate */
	baud = uart_get_baud_rate(port, termios, old, 300, 3686400);

	/* Data bits */
	switch (termios->c_cflag & CSIZE) {
	case CS5:
		lcr = AM335X_UART_LCR_CHAR_5;
		break;
	case CS6:
		lcr = AM335X_UART_LCR_CHAR_6;
		break;
	case CS7:
		lcr = AM335X_UART_LCR_CHAR_7;
		break;
	case CS8:
	default:
		lcr = AM335X_UART_LCR_CHAR_8;
		break;
	}

	/* Stop bits */
	if (termios->c_cflag & CSTOPB)
		lcr |= AM335X_UART_LCR_STOP2;

	/* Parity */
	if (termios->c_cflag & PARENB) {
		lcr |= AM335X_UART_LCR_PARITY;
		if (!(termios->c_cflag & PARODD))
			lcr |= AM335X_UART_LCR_EVEN;
		if (termios->c_cflag & CMSPAR)
			lcr |= AM335X_UART_LCR_FIXED;
	}

	/*
	 * Configure status mask for error reporting to tty layer.
	 * read_status_mask: which LSR bits we care about.
	 * ignore_status_mask: which errors to silently discard.
	 */
	port->read_status_mask = AM335X_UART_LSR_OE;
	if (termios->c_iflag & INPCK)
		port->read_status_mask |= AM335X_UART_LSR_PE |
					  AM335X_UART_LSR_FE;
	if (termios->c_iflag & (BRKINT | PARMRK))
		port->read_status_mask |= AM335X_UART_LSR_BI;

	port->ignore_status_mask = 0;
	if (termios->c_iflag & IGNPAR)
		port->ignore_status_mask |= AM335X_UART_LSR_PE |
					    AM335X_UART_LSR_FE;
	if (termios->c_iflag & IGNBRK) {
		port->ignore_status_mask |= AM335X_UART_LSR_BI;
		if (termios->c_iflag & IGNPAR)
			port->ignore_status_mask |= AM335X_UART_LSR_OE;
	}

	/* Ignore all characters if CREAD is not set */
	if ((termios->c_cflag & CREAD) == 0)
		port->ignore_status_mask |= AM335X_UART_LSR_RXFIFOE;

	spin_lock_irqsave(&port->lock, flags);

	/* Update timeout based on new baud rate */
	uart_update_timeout(port, termios->c_cflag, baud);

	/* Apply hardware configuration */
	am335x_uart_set_baud(up, baud, lcr);

	spin_unlock_irqrestore(&port->lock, flags);
}

static const char *am335x_uart_type(struct uart_port *port)
{
	if (port->type == PORT_AM335X)
		return "AM335X_UART";
	return NULL;
}

static void am335x_uart_config_port(struct uart_port *port, int flags)
{
	if (flags & UART_CONFIG_TYPE)
		port->type = PORT_AM335X;
}

static int am335x_uart_request_port(struct uart_port *port)
{
	/* Memory region already managed by devm */
	return 0;
}

static void am335x_uart_release_port(struct uart_port *port)
{
	/* Nothing to do - devm handles cleanup */
}

static int am335x_uart_verify_port(struct uart_port *port,
				    struct serial_struct *ser)
{
	if (ser->type != PORT_UNKNOWN && ser->type != PORT_AM335X)
		return -EINVAL;
	return 0;
}

static void am335x_uart_break_ctl(struct uart_port *port, int break_state)
{
	struct am335x_uart_port *up =
		container_of(port, struct am335x_uart_port, port);
	unsigned long flags;

	spin_lock_irqsave(&port->lock, flags);
	if (break_state == -1)
		up->lcr |= AM335X_UART_LCR_BREAK;
	else
		up->lcr &= ~AM335X_UART_LCR_BREAK;
	uart_write(up, AM335X_UART_LCR, up->lcr);
	spin_unlock_irqrestore(&port->lock, flags);
}

static const struct uart_ops am335x_uart_ops = {
	.tx_empty	= am335x_uart_tx_empty,
	.set_mctrl	= am335x_uart_set_mctrl,
	.get_mctrl	= am335x_uart_get_mctrl,
	.stop_tx	= am335x_uart_stop_tx,
	.start_tx	= am335x_uart_start_tx,
	.stop_rx	= am335x_uart_stop_rx,
	.break_ctl	= am335x_uart_break_ctl,
	.startup	= am335x_uart_startup,
	.shutdown	= am335x_uart_shutdown,
	.set_termios	= am335x_uart_set_termios,
	.type		= am335x_uart_type,
	.config_port	= am335x_uart_config_port,
	.request_port	= am335x_uart_request_port,
	.release_port	= am335x_uart_release_port,
	.verify_port	= am335x_uart_verify_port,
};

/*
 * ============================================================
 * uart_driver - registration with serial core
 *
 * This creates the /dev/ttyAM* device nodes.
 * The serial core handles most of the tty layer integration.
 * ============================================================
 */
static struct uart_driver am335x_uart_driver = {
	.owner		= THIS_MODULE,
	.driver_name	= DRIVER_NAME,
	.dev_name	= DEV_NAME,
	.nr		= AM335X_UART_NR,
};

/*
 * ============================================================
 * Platform driver (device tree binding)
 * ============================================================
 */

static int am335x_uart_probe(struct platform_device *pdev)
{
	struct am335x_uart_port *up;
	struct resource *res;
	int irq, ret;

	up = devm_kzalloc(&pdev->dev, sizeof(*up), GFP_KERNEL);
	if (!up)
		return -ENOMEM;

	/* Map register base */
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	up->port.membase = devm_ioremap_resource(&pdev->dev, res);
	if (IS_ERR(up->port.membase))
		return PTR_ERR(up->port.membase);
	up->port.mapbase = res->start;

	/* Get IRQ */
	irq = platform_get_irq(pdev, 0);
	if (irq < 0)
		return irq;
	up->port.irq = irq;

	/* Get and enable functional clock */
	up->fclk = devm_clk_get(&pdev->dev, "fck");
	if (IS_ERR(up->fclk)) {
		/*
		 * Fallback: some device trees provide the clock frequency
		 * directly via the clock-frequency property.
		 */
		u32 freq;

		if (of_property_read_u32(pdev->dev.of_node,
					 "clock-frequency", &freq)) {
			dev_err(&pdev->dev, "no clock source\n");
			return -ENODEV;
		}
		up->fclk = NULL;
		up->port.uartclk = freq;
	} else {
		ret = clk_prepare_enable(up->fclk);
		if (ret) {
			dev_err(&pdev->dev, "failed to enable clock: %d\n",
				ret);
			return ret;
		}
		up->port.uartclk = clk_get_rate(up->fclk);
	}

	/* Fill uart_port structure */
	up->port.dev = &pdev->dev;
	up->port.type = PORT_AM335X;
	up->port.iotype = UPIO_MEM32;
	up->port.regshift = 2;
	up->port.fifosize = AM335X_UART_FIFO_SIZE;
	up->port.ops = &am335x_uart_ops;
	up->port.flags = UPF_BOOT_AUTOCONF;
	up->port.line = 0;	/* ttyAM0 */

	platform_set_drvdata(pdev, up);
	am335x_ports[0] = up;

	/* Register with serial core */
	ret = uart_add_one_port(&am335x_uart_driver, &up->port);
	if (ret) {
		dev_err(&pdev->dev, "uart_add_one_port failed: %d\n", ret);
		if (up->fclk)
			clk_disable_unprepare(up->fclk);
		return ret;
	}

	dev_info(&pdev->dev, "AM335x UART registered as ttyAM0, "
		 "base=0x%08llx, irq=%d, clock=%u Hz\n",
		 (unsigned long long)res->start, irq, up->port.uartclk);

	return 0;
}

static int am335x_uart_remove(struct platform_device *pdev)
{
	struct am335x_uart_port *up = platform_get_drvdata(pdev);

	uart_remove_one_port(&am335x_uart_driver, &up->port);

	if (up->fclk)
		clk_disable_unprepare(up->fclk);

	am335x_ports[0] = NULL;

	return 0;
}

static const struct of_device_id am335x_uart_of_match[] = {
	{ .compatible = "training,am335x-uart" },
	{ },
};
MODULE_DEVICE_TABLE(of, am335x_uart_of_match);

static struct platform_driver am335x_uart_platform_driver = {
	.probe	= am335x_uart_probe,
	.remove	= am335x_uart_remove,
	.driver	= {
		.name		= DRIVER_NAME,
		.of_match_table	= am335x_uart_of_match,
	},
};

/*
 * ============================================================
 * Module init / exit
 *
 * The init order matters:
 *   1. uart_register_driver() - register /dev/ttyAM* with serial core
 *   2. platform_driver_register() - triggers probe() when DT matches
 *
 * Exit is reverse order.
 * ============================================================
 */
static int __init am335x_uart_init(void)
{
	int ret;

	ret = uart_register_driver(&am335x_uart_driver);
	if (ret)
		return ret;

	ret = platform_driver_register(&am335x_uart_platform_driver);
	if (ret) {
		uart_unregister_driver(&am335x_uart_driver);
		return ret;
	}

	pr_info(DRIVER_NAME ": driver loaded\n");
	return 0;
}

static void __exit am335x_uart_exit(void)
{
	platform_driver_unregister(&am335x_uart_platform_driver);
	uart_unregister_driver(&am335x_uart_driver);
	pr_info(DRIVER_NAME ": driver unloaded\n");
}

module_init(am335x_uart_init);
module_exit(am335x_uart_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Training - Embedded Linux AI-First");
MODULE_DESCRIPTION("AM335x UART driver for BeagleBone Black (educational)");
