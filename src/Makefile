PROJECT_DIR = ..

LDLIBS = -lm
SRC = \
	accl_manager.c \
	ADC_read.c \
	button_manager.c \
	buttons4.c \
	circBufT.c \
	circBufV.c \
	display_manager.c \
	i2c_driver.c \
	serial_sender.c \
	step_counter_main.c \
	switches.c

USE_FREERTOS = 0

include $(PROJECT_DIR)/libs/OrbitOLED/orbit-oled.mk
include $(PROJECT_DIR)/tiva-freertos.mk
