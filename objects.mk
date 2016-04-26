#
#	FreeRTOS portable layer for RaspberryPi
#
OBJECTS += $(BUILD_DIR)FreeRTOS/Source/portable/GCC/RaspberryPi/port.o
OBJECTS += $(BUILD_DIR)FreeRTOS/Source/portable/GCC/RaspberryPi/portisr.o

#
#	FreeRTOS Core
#
OBJECTS += $(BUILD_DIR)FreeRTOS/Source/croutine.o
OBJECTS += $(BUILD_DIR)FreeRTOS/Source/list.o
OBJECTS += $(BUILD_DIR)FreeRTOS/Source/queue.o
OBJECTS += $(BUILD_DIR)FreeRTOS/Source/tasks.o

#
#	Interrupt Manager & GPIO Drivers
#
OBJECTS += $(BUILD_DIR)src/Drivers/rpi_irq.o
OBJECTS += $(BUILD_DIR)src/Drivers/rpi_systimer.o
OBJECTS += $(BUILD_DIR)src/Drivers/rpi_gpio.o
OBJECTS += $(BUILD_DIR)src/Drivers/rpi_i2c.o
OBJECTS += $(BUILD_DIR)src/Drivers/i2c_adxl345.o
OBJECTS += $(BUILD_DIR)src/Drivers/i2c_itg3200.o
OBJECTS += $(BUILD_DIR)src/Drivers/rpi_spi_slave.o
OBJECTS += $(BUILD_DIR)src/Drivers/accel_driver.o
OBJECTS += $(BUILD_DIR)src/Drivers/servo_driver.o

#
#	Selected HEAP implementation for FreeRTOS.
#
OBJECTS += $(BUILD_DIR)/FreeRTOS/Source/portable/MemMang/heap_4.o

#
#	Startup and platform initialisation code.
#
OBJECTS += $(BUILD_DIR)src/startup.o
#OBJECTS += $(BUILD_DIR)src/rpi_boot_v1.o


#
#	Main Test Program
#
OBJECTS += $(BUILD_DIR)src/main.o
OBJECTS += $(BUILD_DIR)src/status-report.o
OBJECTS += $(BUILD_DIR)src/syscalls.o
OBJECTS += $(BUILD_DIR)src/trace.o
OBJECTS += $(BUILD_DIR)src/ex_handlers.o

