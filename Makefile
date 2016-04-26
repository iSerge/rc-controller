#
#	Makefile for FreeRTOS demo on Raspberry Pi
#
BASE=$(shell pwd)/
BUILD_DIR=$(shell pwd)/build/

MODULE_NAME="RaspberryPi BSP"

TARGETS=kernel7.img kernel7.list kernel7.syms kernel7.elf kernel7.hex kernel7.map
LINKER_SCRIPT=raspberrypi.ld
#LINKER_SCRIPT=src/rpi_boot_v1.lds

-include .dbuild/dbuild.mk


all: kernel7.list kernel7.img kernel7.syms kernel7.hex
	@$(SIZE) kernel7.elf

kernel7.img: kernel7.elf
	$(Q)$(PRETTY) IMAGE $(MODULE_NAME) $@
	$(Q)$(OBJCOPY) kernel7.elf -O binary $@

kernel7.list: kernel7.elf
	$(Q)$(PRETTY) LIST $(MODULE_NAME) $@
	$(Q)$(OBJDUMP) -D -S  kernel7.elf > $@

kernel7.syms: kernel7.elf
	$(Q)$(PRETTY) SYMS $(MODULE_NAME) $@
	$(Q)$(OBJDUMP) -t kernel7.elf > $@

kernel7.hex : kernel7.elf
	$(Q)$(OBJCOPY) kernel7.elf -O ihex $@

kernel7.elf: LDFLAGS += -L "/usr/lib/arm-none-eabi/newlib/fpu" -lc -lm
kernel7.elf: LDFLAGS += -L "/usr/lib/gcc/arm-none-eabi/4.8/fpu" -lgcc
#kernel7.elf: LDFLAGS += -L "/usr/lib/gcc/arm-linux-gnueabihf/4.9" -lgcc_eh

kernel7.elf: $(OBJECTS)
	$(Q)$(LD) $(OBJECTS) -static -Map kernel7.map -o $@ -T $(LINKER_SCRIPT) $(LDFLAGS)
