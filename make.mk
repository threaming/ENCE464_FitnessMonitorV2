PROJECT_DIR := $(dir $(lastword $(MAKEFILE_LIST)))

BUILD_DIR ?= build

SCRIPTS_DIR = $(PROJECT_DIR)/scripts

ifndef TARGET
TARGET := $(BUILD_DIR)/app.elf
endif

.PHONY: all
all: $(TARGET)

TOOLCHAIN ?= tm4c
include $(PROJECT_DIR)/toolchains/$(TOOLCHAIN).mk

OPT ?= -O0

INCLUDES += -I"$(PROJECT_DIR)"
OBJS += $(addprefix $(BUILD_DIR)/, $(patsubst %.c,%.o,$(SRC)))
DEPS += $(patsubst %.o,%.d,$(OBJS))

CFLAGS += \
	-g3 -Wall -W $(OPT) \
	$(INCLUDES) \
	$(CFLAGS_EXTRA)

LDFLAGS += $(LDFLAGS_EXTRA)

ifndef VERBOSE
Q=@
endif

$(BUILD_DIR)/%.o: %.c
	@mkdir -p "$(@D)"
	$(info CC $<)
	$(Q)$(CC) $(CFLAGS) -MMD -MP -o $@ -c $<

-include $(DEPS)

$(TARGET): $(OBJS)
	$(info LD $@)
	$(Q)$(LD) $(LDFLAGS) -o $@ $^ $(LDLIBS)

PREPROCESSED_DIR = $(BUILD_DIR)/preprocessed
$(PREPROCESSED_DIR)/%: %
	@mkdir -p "$(@D)"
	$(info CPP $<)
	$(Q)$(CPP) -o "$(@)" $(CFLAGS) $<

.PHONY: preprocessed
preprocessed: $(addprefix $(PREPROCESSED_DIR)/,$(SRC))

CLEAN += $(BUILD_DIR) $(PREPROCESSED_DIR) $(TARGET)
.PHONY: clean
clean:
	rm -rf $(CLEAN)

.PHONY: info
info:
	@echo TARGET=$(TARGET)
	@echo TOOLCHAIN=$(TOOLCHAIN)
	@echo CC=$(CC)
	@echo CFLAGS=$(CFLAGS)
	@echo LD=$(LD)
	@echo LDFLAGS=$(LDFLAGS)
	@echo LDLIBS=$(LDLIBS)
	@echo GDB=$(GDB)
	@echo SRC=$(SRC)
	@echo OBJS=$(OBJS)
	@echo VPATH=$(VPATH)
	@echo DEPS=$(DEPS)
