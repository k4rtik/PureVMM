# Makefile for JamesM's kernel tutorials.
# The C and C++ rules are already setup by default.
# The only one that needs changing is the assembler
# rule, as we use nasm instead of GNU as.

TARGET := i586-elf
CC := $(TARGET)-gcc
LD := $(TARGET)-ld

SOURCES=boot.o main.o monitor.o common.o gdt.o descriptor_tables.o interrupt.o isr.o timer.o kheap.o paging.o

CFLAGS=-fno-builtin -fno-stack-protector
LDFLAGS=-Tlink.ld
ASFLAGS=-felf

all: $(SOURCES) link 

clean:
	-rm *.o kernel

link:
	$(LD) $(LDFLAGS) -o kernel $(SOURCES)

.s.o:
	nasm $(ASFLAGS) $< 
