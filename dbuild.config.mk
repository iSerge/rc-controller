
CFLAGS += -march=armv7-a -mtune=cortex-a7 -mfloat-abi=hard -mfpu=neon-vfpv4
CFLAGS += -g -Wall -Wextra
CFLAGS += -D RPI2
CFLAGS += -I $(BASE)FreeRTOS/Source/portable/GCC/RaspberryPi/
CFLAGS += -I $(BASE)FreeRTOS/Source/include/
#CFLAGS += -I $(BASE)src/Drivers/
CFLAGS += -I $(BASE)src/

ASFLAGS += -march=armv7-a -mcpu=cortex-a7 -mfpu=neon-vfpv4 -mfloat-abi=hard

TOOLCHAIN=arm-none-eabi-

