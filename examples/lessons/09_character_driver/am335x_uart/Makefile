KERNEL_SRC := /home/phula/work/bbb/kernelbuildscripts/KERNEL
CROSS_COMPILE := /home/phula/work/bbb/gcc-arm-10.3-2021.07-x86_64-arm-none-linux-gnueabihf/bin/arm-none-linux-gnueabihf-
ARCH := arm

obj-m := am335x_uart.o

all:
	$(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERNEL_SRC) M=$(PWD) modules

dtbo: am335x-uart1-overlay.dtbo

%.dtbo: %.dts
	$(KERNEL_SRC)/scripts/dtc/dtc -@ -I dts -O dtb -o $@ $<

deploy:
	scp am335x_uart.ko am335x-uart1-overlay.dtbo debian@192.168.137.2:/home/debian/

clean:
	$(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERNEL_SRC) M=$(PWD) clean
	rm -f *.dtbo
