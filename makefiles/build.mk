# fujinet-lib-examples main build script

# Ensure WSL2 Ubuntu and other linuxes use bash by default instead of /bin/sh, which does not always like the shell commands.
SHELL := /usr/bin/env bash
ALL_TASKS =
DISK_TASKS =
OBJEXT = .o
ASMEXT = .s

-include ../../makefiles/os.mk
-include ../../makefiles/compiler.mk

SRCDIR := src
BUILD_DIR := build
OBJDIR := obj
DIST_DIR := dist

# This allows src to be nested withing sub-directories.
rwildcard=$(wildcard $(1)$(2))$(foreach d,$(wildcard $1*), $(call rwildcard,$d/,$2))

PROGRAM_TGT := $(PROGRAM).$(CURRENT_TARGET)

SOURCES := $(wildcard $(SRCDIR)/*.c)
SOURCES += $(wildcard $(SRCDIR)/*$(ASMEXT))

SOURCES += $(call rwildcard,$(SRCDIR)/common/,*.c)
SOURCES += $(call rwildcard,$(SRCDIR)/common/,*$(ASMEXT))
SOURCES += $(call rwildcard,$(SRCDIR)/$(CURRENT_TARGET)/,*$(ASMEXT))
SOURCES += $(call rwildcard,$(SRCDIR)/$(CURRENT_TARGET)/,*.c)

# remove trailing and leading spaces.
SOURCES := $(strip $(SOURCES))

# convert from src/your/long/path/foo.[c|s] to obj/<target>/your/long/path/foo$(OBJEXT)
# we need the target because compiling for previous target does not pick up potential macro changes
OBJ1 := $(SOURCES:.c=$(OBJEXT))
OBJECTS := $(OBJ1:$(ASMEXT)=$(OBJEXT))

OBJECTS_ARC := $(OBJECTS)

-include ../../makefiles/objects-$(CURRENT_TARGET).mk

OBJECTS := $(OBJECTS:$(SRCDIR)/%=$(OBJDIR)/$(CURRENT_TARGET)/%)
OBJECTS_ARC := $(OBJECTS_ARC:$(SRCDIR)/%=$(OBJDIR)/$(CURRENT_TARGET)/%)

# Ensure make recompiles parts it needs to if src files change
DEPENDS := $(OBJECTS:$(OBJEXT)=.d)

ASFLAGS += \
	$(INCS_ARG) src/common \
	$(INCS_ARG) src/$(CURRENT_TARGET) \
	$(INCS_ARG) $(SRCDIR)

ifeq ($(CC),iix compile)
CFLAGS += \
	$(INCC_ARG)src/common \
	$(INCC_ARG)src/$(CURRENT_TARGET) \
	$(INCC_ARG)$(SRCDIR)
else
CFLAGS += \
	$(INCC_ARG) src/common \
	$(INCC_ARG) src/$(CURRENT_TARGET) \
	$(INCC_ARG) $(SRCDIR)
endif

# allow for additional flags etc
-include ../../makefiles/common.mk
-include ../../makefiles/custom-$(CURRENT_PLATFORM).mk

# allow for application specific config
-include ./application.mk

.SUFFIXES:
.PHONY: all clean release $(DISK_TASKS) $(BUILD_TASKS) $(PROGRAM_TGT) $(ALL_TASKS)

$(info LIBS: $(LIBS))

all: $(ALL_TASKS) $(PROGRAM_TGT)

-include $(DEPENDS)

$(OBJDIR):
	$(call MKDIR,$@)

$(BUILD_DIR):
	$(call MKDIR,$@)

$(DIST_DIR):
	$(call MKDIR,$@)

SRC_INC_DIRS := \
  $(sort $(dir $(wildcard $(SRCDIR)/$(CURRENT_TARGET)/*))) \
  $(sort $(dir $(wildcard $(SRCDIR)/common/*))) \
  $(SRCDIR)

vpath %.c $(SRC_INC_DIRS)
vpath %$(ASMEXT) $(SRC_INC_DIRS)

$(OBJDIR)/$(CURRENT_TARGET)/%$(OBJEXT): %.c $(VERSION_FILE) | $(OBJDIR)
	@$(call MKDIR,$(dir $@))
ifeq ($(CC),cl65)
	$(CC) -t $(CURRENT_TARGET) -c --create-dep $(@:.o=.d) $(CFLAGS) --listing $(@:.o=.lst) -Ln $@.lbl -o $@ $<
else ifeq ($(CC),iix compile)
	$(CC) $(CFLAGS) $< keep=$(subst .root,,$@)
else
	$(CC) -c --deps $(CFLAGS) -o $@ $<
endif


$(OBJDIR)/$(CURRENT_TARGET)/%$(OBJEXT): %$(ASMEXT) $(VERSION_FILE) | $(OBJDIR)
	@$(call MKDIR,$(dir $@))
ifeq ($(CC),cl65)
	$(CC) -t $(CURRENT_TARGET) -c --create-dep $(@:.o=.d) $(CFLAGS) --listing $(@:.o=.lst) -Ln $@.lbl -o $@ $<
else ifeq ($(CC),iix compile)
	$(CC) $(CFLAGS) $< keep=$(subst .root,,$@)
else
	$(CC) -c --deps $(CFLAGS) -o $@ $<
endif


$(BUILD_DIR)/$(PROGRAM_TGT): $(OBJECTS) $(LIBS) | $(BUILD_DIR)
ifeq ($(CC),cl65)
	$(CC) -t $(CURRENT_TARGET) $(LDFLAGS) -o $@ $<
else ifeq ($(CC),iix compile)
	@echo "TODO: What is the compile command for the application on apple2gs"
	# $(CC) $(CFLAGS) $< keep=$(subst .root,,$@)
else
	@echo "TODO: Other compilers go here"
endif

$(PROGRAM_TGT): $(BUILD_DIR)/$(PROGRAM_TGT) | $(BUILD_DIR)

test: $(PROGRAM_TGT)
	$(PREEMUCMD)
	$(EMUCMD) $(BUILD_DIR)\\$<
	$(POSTEMUCMD)

# Use "./" in front of all dirs being removed as a simple safety guard to
# ensure deleting from current dir, and not something like root "/".
clean:
	@for d in $(BUILD_DIR) $(OBJDIR) $(DIST_DIR); do \
      if [ -d "./$$d" ]; then \
	    echo "Removing $$d"; \
        rm -rf ./$$d; \
      fi; \
    done

release: all | $(BUILD_DIR) $(DIST_DIR)
	cp $(BUILD_DIR)/$(PROGRAM_TGT) $(DIST_DIR)/$(PROGRAM_TGT)$(SUFFIX)

disk: release $(DISK_TASKS)
