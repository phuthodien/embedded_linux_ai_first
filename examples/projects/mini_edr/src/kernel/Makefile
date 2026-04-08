# SPDX-License-Identifier: GPL-2.0
#
# Makefile for EDR kernel module
#

obj-m := edr_kmodule.o

# Include path for shared headers
ccflags-y := -I$(src)/../common

# Default kernel build directory
KDIR ?= /lib/modules/$(shell uname -r)/build

# Use CURDIR instead of PWD to get correct path when called from parent
KMOD_DIR := $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))

all:
	$(MAKE) -C $(KDIR) M=$(KMOD_DIR) modules

clean:
	$(MAKE) -C $(KDIR) M=$(KMOD_DIR) clean

.PHONY: all clean
