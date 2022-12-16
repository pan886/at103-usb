COMP_UNITY_DIR   = $(MID_DIR)/unity/
MID_C_INC       += -I$(AT_SDK_HOME_SH)/$(COMP_UNITY_DIR)/include
MID_C_SRC       += $(subst $(AT_SDK_HOME_SH),,$(call wildcards,$(AT_SDK_HOME_SH)/$(COMP_UNITY_DIR),%.c))
MID_ASM_SRC     += $(subst $(AT_SDK_HOME_SH),,$(call wildcards,$(AT_SDK_HOME_SH)/$(COMP_UNITY_DIR),%.S))