COMP_UNITY_DIR   = $(MID_DIR)/unity/
MID_C_INC       += -I$(AT_SDK_HOME_SH)/$(COMP_UNITY_DIR)/include
MID_C_SRC       += $(shell cd $(AT_SDK_HOME); find $(COMP_UNITY_DIR) -iname "*.c" | sed -e 's/.\///1')
MID_ASM_SRC     += $(shell cd $(AT_SDK_HOME); find $(COMP_UNITY_DIR) -iname "*.S" | sed -e 's/.\///1')