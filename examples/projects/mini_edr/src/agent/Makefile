# Makefile for EDR userspace agent

CC := gcc
CFLAGS := -Wall -Wextra -O2 -I../common
TARGET := edr_agent

all: $(TARGET)

$(TARGET): edr_agent.c ../common/edr_events.h
	$(CC) $(CFLAGS) -o $@ edr_agent.c

clean:
	rm -f $(TARGET)

.PHONY: all clean
