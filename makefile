ARMGNU=C:\Users\Gzalo\.platformio\packages\toolchain-gccarmnoneeabi\bin\arm-none-eabi
MACH=arm1176jz-s
CFLAGS=-c -mcpu=$(MACH) -mfloat-abi=soft -Wall -O2 --std=c17 -nostartfiles
LDFLAGS=-mcpu=$(MACH) -mfloat-abi=soft -T link.ld --specs=nano.specs -nostartfiles -Wl,-Map=final.map

all:
	$(ARMGNU)-as -o start.o start.s
	$(ARMGNU)-gcc $(CFLAGS) -c main.c
	$(ARMGNU)-gcc $(CFLAGS) -c syscalls.c
	$(ARMGNU)-gcc $(LDFLAGS) -o kernel.elf start.o main.o syscalls.o
	$(ARMGNU)-objcopy kernel.elf -O binary out.bin
