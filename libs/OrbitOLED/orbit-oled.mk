ORBIT_OLED_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

VPATH += $(ORBIT_OLED_DIR) $(ORBIT_OLED_DIR)lib_OrbitOled

SRC += 	$(notdir $(wildcard $(ORBIT_OLED_DIR)lib_OrbitOled/*.c)) \
		$(notdir $(wildcard $(ORBIT_OLED_DIR)*.c))

INCLUDES += -I"$(ORBIT_OLED_DIR).."
