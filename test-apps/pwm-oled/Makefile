PROJECT_DIR = ../..

VPATH = ${PROJECT_DIR}/OrbitOLED ${PROJECT_DIR}/OrbitOLED/lib_OrbitOled
SRC = \
	pwm-oled.c \
	buttons.c \
	OrbitOLEDInterface.c \
	$(notdir $(wildcard ${PROJECT_DIR}/OrbitOLED/lib_OrbitOled/*.c))

include $(PROJECT_DIR)/tiva-freertos.mk
