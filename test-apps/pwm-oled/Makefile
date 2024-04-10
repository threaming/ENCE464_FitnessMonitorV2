PROJECT_DIR = ../..

VPATH = OrbitOLED OrbitOLED/lib_OrbitOled
SRC = \
	pwm-oled.c \
	buttons.c \
	OrbitOLEDInterface.c \
	$(notdir $(wildcard OrbitOLED/lib_OrbitOled/*.c))

include $(PROJECT_DIR)/tiva-freertos.mk
