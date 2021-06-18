#  
# Begin DemoProduct executable
#  

COMPONENT_EXTRA_INCLUDES += $(FOTAHUB_INCDIR)
ifeq ($(wildcard $(FOTAHUB_SRCDIR)/fotahub/component.mk),)
COMPONENT_ADD_LDFLAGS += -L$(FOTAHUB_LIBDIR) -lfotahub
endif

COMPONENT_EMBED_TXTFILES += $(PROJECT_PATH)/AmazonRootCA1.pem
COMPONENT_EMBED_TXTFILES += $(PROJECT_PATH)/AmazonRootCA3.pem
COMPONENT_EMBED_TXTFILES += $(PROJECT_PATH)/Certificate.pem
COMPONENT_EMBED_TXTFILES += $(PROJECT_PATH)/PrivateKey.pem

#  
# End DemoProduct executable
#  


