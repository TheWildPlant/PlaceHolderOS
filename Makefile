CROSS_COMPILE = i686-elf-
CC = $(CROSS_COMPILE)g++
LD = $(CROSS_COMPILE)ld

CFLAGS = -std=gnu++17 -ffreestanding -O2 -Wall -Wextra
LDFLAGS = -nostdlib -T linker.ld

all: os-image.bin

os-image.bin: bootloader.bin kernel.bin
    cat $^ > $@

bootloader.bin: boot.asm
    nasm -f bin -o $@ $<

kernel.bin: kernel.o
    $(LD) $(LDFLAGS) -o $@ $<

kernel.o: kernel.cpp
    $(CC) $(CFLAGS) -c $< -o $@

clean:
    rm -f *.bin *.o