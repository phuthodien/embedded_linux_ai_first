# Top-level Makefile for EDR Agent
#
# Builds both kernel module and userspace agent.
#
# Usage:
#   make          - build all
#   make kernel   - build kernel module only
#   make agent    - build userspace agent only
#   make clean    - clean all build artifacts

.PHONY: all kernel agent clean

all: kernel agent

kernel:
	$(MAKE) -C kernel

agent:
	$(MAKE) -C agent

clean:
	$(MAKE) -C kernel clean
	$(MAKE) -C agent clean
