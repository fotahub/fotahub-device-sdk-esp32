export FOTAHUB_INCDIR ?=  $(abspath $(PROJECT_PATH)/../../..)/include
export FOTAHUB_SRCDIR ?=
export FOTAHUB_LIBDIR ?=  $(abspath $(PROJECT_PATH)/../../..)/lib

#  
# Begin DemoProduct executable
#  

COMPONENT_EXTRA_INCLUDES += $(FOTAHUB_INCDIR)
ifeq ($(wildcard $(FOTAHUB_SRCDIR)/fotahub/component.mk),)
COMPONENT_ADD_LDFLAGS += -L$(FOTAHUB_LIBDIR) -lfotahub
endif

#  
# End DemoProduct executable
#  


