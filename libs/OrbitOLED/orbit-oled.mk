VPATH += ${PROJECT_DIR}/libs/OrbitOLED ${PROJECT_DIR}/libs/OrbitOLED/lib_OrbitOled

SRC += 	$(notdir $(wildcard ${PROJECT_DIR}/libs/OrbitOLED/lib_OrbitOled/*.c)) \
		$(notdir $(wildcard ${PROJECT_DIR}/libs/OrbitOLED/*.c))

INCLUDES += -I"$(PROJECT_DIR)/libs"
