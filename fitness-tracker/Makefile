PROJECT_DIR = ..

VPATH = ${PROJECT_DIR}/OrbitOLED ${PROJECT_DIR}/OrbitOLED/lib_OrbitOled
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
	switches.c \
	$(notdir $(wildcard ${PROJECT_DIR}/OrbitOLED/lib_OrbitOled/*.c)) \
	$(notdir $(wildcard ${PROJECT_DIR}/OrbitOLED/*.c))

include $(PROJECT_DIR)/tiva-freertos.mk
