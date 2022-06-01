include $(AT_SDK_HOME_SH)/tools/mk/function.mk
AT_SDK_HOME   = $(AT_SDK_HOME_SH)
DEBUG         = yes
CHIP          = AT103
STACK_SIZE    = 0x400
HEAP_SIZE     = 0x0
NO_FLASH      = yes
FPU           = yes
TARGET        = crc
APP_DIR       = ./project/examples/crc
APP_C_SRC    += $(subst $(AT_SDK_HOME_SH),,$(call wildcards,$(AT_SDK_HOME_SH)/$(APP_DIR),%.c))
APP_C_INC    += $(addprefix -I,$(call dirs,$(AT_SDK_HOME)/$(APP_DIR),%.h))
APP_CPP_INC  +=
APP_CPP_SRC  +=
APP_ASM_SRC  +=

include $(AT_SDK_HOME_SH)/tools/mk/common.mk
