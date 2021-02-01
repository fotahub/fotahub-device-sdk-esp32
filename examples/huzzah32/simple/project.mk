PROJECT_NAME = demoproduct

DEBUG_FLAGS = -g0
EXTRA_CFLAGS = -D_ESP32_ -Wno-error=unused-but-set-parameter -Wno-error=type-limits -Wno-error=switch -Wno-error=maybe-uninitialized -Wno-error=format

#  
# Begin DemoProduct executable
#  

EXTRA_COMPONENT_DIRS := $(PROJECT_PATH)/demoproduct

ifneq ($(wildcard $(FOTAHUB_SRCDIR)/fotahub/component.mk),)
EXTRA_COMPONENT_DIRS += $(FOTAHUB_SRCDIR)/fotahub
endif

#  
# End DemoProduct executable
#  

include $(IDF_PATH)/make/project.mk

gen_flash_cmds: partition_table_get_info blank_ota_data
	$(if $(wildcard $(IDF_PATH)/tools/gen_flash_cmds.sh),$(IDF_PATH)/tools/gen_flash_cmds.sh $(if $(HOST),$(HOST),Linux) $(ESPBAUD) $(CONFIG_ESPTOOLPY_BEFORE) $(CONFIG_ESPTOOLPY_AFTER) $(if $(CONFIG_ESPTOOLPY_COMPRESSED),-z,-u) "$(ESPTOOL_WRITE_FLASH_OPTIONS)" "$(ESPTOOL_ALL_FLASH_ARGS)",)

all: gen_flash_cmds

# Prevent full rebuild including all components because of this Makefile getting re-generated
COMMON_MAKEFILES := $(abspath $(IDF_PATH)/make/project.mk $(IDF_PATH)/make/common.mk $(IDF_PATH)/make/component_wrapper.mk)



