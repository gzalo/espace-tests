ARMGNU=C:\Users\Gzalo\.platformio\packages\toolchain-gccarmnoneeabi\bin\arm-none-eabi
MACH=arm8
CFLAGS=-c -mcpu=$(MACH) -Wall -nostdlib -nostartfiles -O2 --std=c17

all:
	$(ARMGNU)-as -o start.o start.s
	$(ARMGNU)-gcc $(CFLAGS) -c main.c
	$(ARMGNU)-ld -T link.ld -o kernel.elf start.o main.o
	$(ARMGNU)-objcopy kernel.elf -O binary out.bin
