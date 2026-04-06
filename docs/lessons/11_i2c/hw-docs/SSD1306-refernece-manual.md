## **Tổng quan về SSD1306**

![](_page_0_Picture_1.jpeg)

| Display<br>Technology             | OLED<br>(Organic<br>LED) |
|-----------------------------------|--------------------------|
| MCU<br>Interface                  | I2C<br>/<br>SPI          |
| Screen<br>Size                    | 0.96<br>Inch<br>Across   |
| Resolution                        | 128×64<br>pixels         |
| Operating<br>Voltage              | 3.3V<br>–<br>5V          |
| Operating<br>Current              | 20mA<br>max              |
| Viewing<br>Angle                  | 160°                     |
| Characters<br>Per<br>Row          | 21                       |
| Number<br>of<br>Character<br>Rows | 7                        |

SSD1306 có dộ phân giải 128x64 và được chia vào 8(0-7) pages. Mỗi page chứa 128(0-127) columns(segments). Và mỗi column chứa 8 bits.

![](_page_0_Picture_4.jpeg)

Và SSD1306 được built-in 1KB **Graphic Display Data RAM (GDDRAM)** để giữ những bit parern cho màn hình.

![](_page_1_Figure_0.jpeg)

Giả sử muốn print kĩ tự A có kích thứớc 5x8 từ vị trí 0x0

- Đầu tiên, dịch chuyển cursor đến vị tri 0x0 s
- Sau đó, thực hiện ghi lần lượt 0x7C, 0x12, x011, 0x12, 0x7C

![](_page_1_Figure_4.jpeg)

SSD1306 có 2 chế độ gửi nhận dữ liệu

- Command
- Data được ghi vào trong GDDRAM

## Send command 0XAE

- 1. Send the Start condition
- 2. Send the Slave address with R/W bit
- 3. Send the Control Byte (0x00)
- 4. Send the Command byte (0xAE)
- 5. Send the Stop condition

- Write 0xFF to display
  1. Send the Start condition
  2. Send the Slave address with R/W bit
  3. Send the Control Byte (0x40)
  4. Send the Command byte (0xFF)
  5. Send the Stop condition

## **SSD1306 Command Table**

| Command   | Command                      | Description                                                                                                                                                                        |
|-----------|------------------------------|------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| 0x00      | COMMAND                      | This command is used to indicate the next data byte is acted as a command.                                                                                                         |
| 0x40~0x7F | Display Start<br>Line        | This command sets the Display Start Line register to determine the starting address of display RAM                                                                                 |
| 0xAE      | Display OFF                  | This command is used to turn OFF the OLED display panel.                                                                                                                           |
| 0xAF      | Display ON                   | This command is used to turn ON the OLED display panel.                                                                                                                            |
| 0x20      | Memory<br>Addressing<br>Mode | If horizontal address increment mode is enabled by command 20h, after finishing read/write one column data, it is incremented automatically to the next column address.            |
| 0x2E      | Deactivate<br>Scroll         | This command deactivates the scroll.                                                                                                                                               |
| 0x2F      | Activate Scroll              | This command activates the scroll if it is configured before.                                                                                                                      |
| 0x21      | Column Address               | This command is used to define the current read/write column address in graphic display data RAM.                                                                                  |
| 0x22      | Page Address                 | This command is used to define the current read/write Line(Page as per data sheet) address in graphic display data RAM.                                                            |
| 0x81      | Contrast<br>Control          | This command sets the Contrast Setting of the display. The chip has 256 contrast steps from 00h to FFh. The segment output current increases as the contrast step value increases. |
| 0xA0      | Segment Re-<br>map           | This command sets the column address 0 is mapped to SEG0.                                                                                                                          |
| 0xA1      | Segment Re-<br>map           | This command sets the column address 127 is mapped to SEG0.                                                                                                                        |

| 0xA6      | Normal Display                                           | This command sets the display to normal mode. In the normal display, a RAM data of 1 indicates an "ON" pixel.                          |
|-----------|----------------------------------------------------------|----------------------------------------------------------------------------------------------------------------------------------------|
| 0xA7      | Inverse Display                                          | This command sets the display to inverse mode. In<br>the normal display, a RAM data of 0 indicates an<br>"ON" pixel.                   |
| 8Ax0      | Multiplex Ratio                                          | This command sets the display multiplex ratio.                                                                                         |
| 0xC0/0xC8 | COM Output<br>Scan direction                             | 0xC0 - Normal mode (RESET) Scan from COM0 to COM[N -1] 0xC8- Remapped mode. Scan from COM[N-1] to COM0 Where N is the Multiplex ratio. |
| 0xD3      | Display Offset                                           | This command sets vertical shift by COM from 0d~63d.                                                                                   |
| 0xD5      | Display Clock<br>Divide<br>Ratio/Oscillator<br>Frequency | This command sets the display Clock Divide Ratio and Oscillator Frequency.                                                             |
| 0x26/0x27 | Continuous<br>Horizontal Scroll                          | 0x26 - Right horizontal scroll<br>0x27 - Left horizontal scroll                                                                        |
| 0x29/0x2A | Continuous<br>Vertical and<br>Horizontal Scroll          | 0x29 - Vertical and Right Horizontal Scroll<br>0x2A - Vertical and Left Horizontal Scroll                                              |