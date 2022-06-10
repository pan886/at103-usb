include $(AT_SDK_HOME_SH)/tools/mk/function.mk
AT_SDK_HOME   = $(AT_SDK_HOME_SH)
DEBUG         = yes
CHIP          = AT103
STACK_SIZE    = 0x400
NO_FLASH      = yes
TARGET        = I2C_Int
APP_DIR       = ./project/examples/I2C_Int
APP_C_SRC    += $(subst $(AT_SDK_HOME_SH),,$(call wildcards,$(AT_SDK_HOME_SH)/$(APP_DIR),%.c))
APP_C_INC    += $(addprefix -I,$(call dirs,$(AT_SDK_HOME)/$(APP_DIR),%.h))
APP_CPP_INC  +=
APP_CPP_SRC  +=
APP_ASM_SRC  +=

include $(AT_SDK_HOME_SH)/tools/mk/common.mk
