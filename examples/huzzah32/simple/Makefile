VERBOSE ?= 

ifdef VERBOSE
Q = 
else
Q = @
endif

export MSYSTEM = $(if $(filter Windows_NT,$(OS)),MINGW32,)
BASH_C = $(if $(filter Windows_NT,$(OS)),/c/msys32/usr/bin/bash -lc,/bin/bash -c)

export IDF_PATH ?= /git/esp-idf
export PROJECT_PATH = $(abspath $(dir $(firstword $(MAKEFILE_LIST))))

export FOTAHUB_INCDIR ?=  $(abspath $(PROJECT_PATH)/../../..)/include
export FOTAHUB_SRCDIR ?=
export FOTAHUB_LIBDIR ?=  $(abspath $(PROJECT_PATH)/../../..)/lib

SDKCONFIG := $(abspath sdkconfig)

.PHONY: all clean

default: all

$(SDKCONFIG): 
	$(Q) $(BASH_C) "$(MAKE) -f project.mk --no-print-directory defconfig"

menuconfig: 
	$(Q) $(BASH_C) "$(MAKE) -f project.mk --no-print-directory menuconfig"

defconfig: 
	$(Q) $(BASH_C) "$(MAKE) -f project.mk --no-print-directory defconfig"

all: $(SDKCONFIG)
	$(Q) $(BASH_C) "$(MAKE) -f project.mk --no-print-directory all"

flash: $(SDKCONFIG)
	$(Q) $(BASH_C) "$(MAKE) -f project.mk --no-print-directory flash"

clean: 
	$(Q) $(BASH_C) "$(MAKE) -f project.mk --no-print-directory clean"

size: $(SDKCONFIG)
	$(Q) $(BASH_C) "$(MAKE) -f project.mk --no-print-directory size"

size-components: $(SDKCONFIG)
	$(Q) $(BASH_C) "$(MAKE) -f project.mk --no-print-directory size-components"

size-symbols: $(SDKCONFIG)
	$(Q) $(BASH_C) "$(MAKE) -f project.mk --no-print-directory size-symbols"

erase_flash: $(SDKCONFIG)
	$(Q) $(BASH_C) "$(MAKE) -f project.mk --no-print-directory erase_flash"

erase_ota: $(SDKCONFIG)
	$(Q) $(BASH_C) "$(MAKE) -f project.mk --no-print-directory erase_ota"

monitor: $(SDKCONFIG)
	$(Q) $(BASH_C) "$(MAKE) -f project.mk --no-print-directory monitor"

simple_monitor: $(SDKCONFIG)
	$(Q) $(BASH_C) "$(MAKE) -f project.mk --no-print-directory simple_monitor"

list-components: $(SDKCONFIG)
	$(Q) $(BASH_C) "$(MAKE) -f project.mk --no-print-directory list-components"

app: $(SDKCONFIG)
	$(Q) $(BASH_C) "$(MAKE) -f project.mk --no-print-directory app"

app-flash: $(SDKCONFIG)
	$(Q) $(BASH_C) "$(MAKE) -f project.mk --no-print-directory app-flash"

app-clean: 
	$(Q) $(BASH_C) "$(MAKE) -f project.mk --no-print-directory app-clean"

print_flash_cmd: $(SDKCONFIG)
	$(Q) $(BASH_C) "$(MAKE) -f project.mk --no-print-directory print_flash_cmd"

check_python_dependencies: $(SDKCONFIG)
	$(Q) $(BASH_C) "$(MAKE) -f project.mk --no-print-directory check_python_dependencies"

bootloader: $(SDKCONFIG)
	$(Q) $(BASH_C) "$(MAKE) -f project.mk --no-print-directory bootloader"

bootloader-flash: $(SDKCONFIG)
	$(Q) $(BASH_C) "$(MAKE) -f project.mk --no-print-directory bootloader-flash"

bootloader-clean: 
	$(Q) $(BASH_C) "$(MAKE) -f project.mk --no-print-directory bootloader-clean"

partition_table: $(SDKCONFIG)
	$(Q) $(BASH_C) "$(MAKE) -f project.mk --no-print-directory partition_table"

help: $(SDKCONFIG)
	$(Q) $(BASH_C) "$(MAKE) -f project.mk --no-print-directory help"

