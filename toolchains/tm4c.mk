MCU ?= TM4C123GH6PM

COMPILER_TRIPLET = arm-none-eabi

CC = $(COMPILER_TRIPLET)-gcc
CPP = $(CC) -E -P
LD = $(COMPILER_TRIPLET)-gcc

ifneq (,$(shell which $(COMPILER_TRIPLET)-gdb))
GDB ?= $(COMPILER_TRIPLET)-gdb
else
# This supersedes arm-none-eabi-gdb on Debian (Ubuntu?) installs
GDB ?= gdb-multiarch
endif

OPENOCD ?= openocd

CFLAGS += \
	-mcpu=cortex-m4 \
	-mthumb \
	-specs=nano.specs \
	-specs=nosys.specs \
	-mfloat-abi=hard \
	-mfpu=fpv4-sp-d16 \
	-DPART_$(MCU)

LDFLAGS += \
	-T$(SCRIPTS_DIR)/link.ld \
	-mcpu=cortex-m4 \
	-mthumb \
	-specs=nano.specs \
	-specs=nosys.specs \
	-mfloat-abi=hard \
	-mfpu=fpv4-sp-d16

include $(PROJECT_DIR)/libs/tivaware/tivaware.mk
SRC += startup.c
VPATH += $(PROJECT_DIR)

# Use GDB to load the code onto the device
.PHONY: program
program: $(TARGET)
	$(GDB) -batch -x $(SCRIPTS_DIR)/program.gdb $<

# Use GDB to debug the device
.PHONY: debug
debug: $(TARGET)
	$(GDB) -x $(SCRIPTS_DIR)/debug.gdb $<

.PHONY: openocd
openocd:
	$(OPENOCD) -f board/ek-tm4c123gxl.cfg
