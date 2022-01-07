# Global configurations
CHIP                 ?= AT103
HEAP_SIZE            ?= 0x0
STACK_SIZE           ?= 0x400
NO_FLASH             ?= no
FPU                  ?= yes
CXXSUPPORT           ?= yes
CXX_RTTI             ?= yes
CXX_EXCEPTION        ?= no
RELEASE              ?= no


BUILDTOPDIR          = ./BUILD
BUILDSCRIPT_DIR     ?= ./tools/mk
BUILDDIR             = ./$(BUILDTOPDIR)/$(CHIP)/GCC_RISCV

ifeq ($(CHIP), AT103)
CROSS_COMPILE        = riscv32-unknown-elf-
endif

CC                   = $(CROSS_COMPILE)gcc
CXX                  = $(CROSS_COMPILE)g++
AR                   = $(CROSS_COMPILE)ar
LD                   = $(CROSS_COMPILE)ld
OBJCOPY              = $(CROSS_COMPILE)objcopy
OBJDUMP              = $(CROSS_COMPILE)objdump
READELF              = $(CROSS_COMPILE)readelf

ifeq ($(CHIP), AT103)
ifeq ($(FPU), yes)
MCU_RISCV_ARCH      ?= rv32imfcxpulpv2
MCU_RISCV_ABI       ?= ilp32f
else
MCU_RISCV_ARCH      ?= rv32imcxpulpv2
MCU_RISCV_ABI       ?= ilp32
endif
endif

ifeq ($(CHIP), AT103)
ifeq ($(NO_FLASH), yes)
LINKER_TEMPLATE     ?= $(AT_SDK_HOME_SH)/tools/mk/at103_no_flash.lds
CFLAGS              += -DCONFIG_NO_FALSH
ASMFLAGS            += -DCONFIG_NO_FALSH
else
LINKER_TEMPLATE     ?= $(AT_SDK_HOME_SH)/tools/mk/at103_default.lds
endif
GDB_SCRIPT_TEMPLATE  = $(AT_SDK_HOME_SH)/tools/gen_gdbscript/$(CHIP).gdb
endif

LINKER_SCRIPT       ?= $(AT_SDK_HOME_SH)/$(APP_DIR)/link.lds
LINK_DEPS           += $(LINKER_SCRIPT)
GDB_SCRIPT           = $(AT_SDK_HOME_SH)/$(APP_DIR)/load.gdb

# for Optimization and debug options
ifeq ($(DEBUG), yes)
GCCFLAGS            += -g -O0 -DDEBUG
ASMFLAGS            += -g -O0 -DDEBUG
else
GCCFLAGS            += -O1
ASMFLAGS            += -O1
endif

# for heap size options
CFLAGS              += -D__HEAP_SIZE=$(HEAP_SIZE)

# for heap size options
CFLAGS              += -D__STACK_SIZE=$(STACK_SIZE)

#toolchain specific flags
ifeq ($(CHIP), AT103)
GCCFLAGS            += -mnohwloop
# "-fomit-frame-pointer -fno-exceptions -fno-asynchronous-unwind-tables -fno-unwind-tables" is supposed to remove .eh_frame meanwhile is not working.
GCCFLAGS            += -nodefaultlibs -nostdlib #-nostdinc
# For sign extension failure. 
GCCFLAGS            += -fno-expensive-optimizations
# For O3 enablement
GCCFLAGS            += -fno-tree-loop-vectorize
# For strict alignment
GCCFLAGS            += -mstrict-align
# Compilation warning message setting
WARNINGGLOGAL        = -Wall -Wextra -Wfatal-errors
## loose warning messages, will be opened in future.
WARNINGLOOSE         = -Wno-unused-parameter \
                       -Wno-unused-function \
                       -Wno-unused-variable \
                       -Wno-unused-but-set-variable \
                       -Wno-unused-value \
                       -Wno-missing-field-initializers \
                       -Wno-sign-compare
                       #-Wno-implicit-function-declaration\
                       #-Wno-format \
                       #-Wno-implicit-int \

GCCFLAGS            += $(WARNINGGLOGAL) $(WARNINGLOOSE)
GCCFLAGS            += -Wstack-usage=$(STACK_SIZE)
GCCFLAGS            += -mcmodel=medany  -ffunction-sections -fdata-sections
GCCFLAGS            += -ffreestanding -fno-builtin # bypass compiler impacts

CFLAGS              += $(GCCFLAGS)
CFLAGS              += -std=gnu99
CFLAGS              += -fno-builtin-printf -fno-builtin-malloc # bypass compiler impact in fine granularity

ifeq ($(CXXSUPPORT), yes)
CPPFLAGS            += $(GCCFLAGS)
CPPFLAGS            += $(GCCFLAGS)                       \
                       -std=c++11                        \
                       -x c++                            \
                       -fstrict-enums                    \
                       -fno-use-cxa-atexit               \
                       -fno-use-cxa-get-exception-ptr    \
                       -fno-nonansi-builtins             \
                       -fno-threadsafe-statics           \
                       -fno-enforce-eh-specs             \
                       -ftemplate-depth-64               
ifeq ($(CXX_RTTI), no)
CPPFLAGS            += -fno-rtti
else
CPPFLAGS            += -DCONFIG_CXX_RTTI
endif
ifeq ($(CXX_EXCEPTIONS), no)
CPPFLAGS            += -fno-exceptions
else
CPPFLAGS            += -fexceptions
CPPFLAGS            += -DCONFIG_CXX_EXCEPTIONS
endif
endif

ASMFLAGS            += -x assembler-with-cpp -march=$(MCU_RISCV_ARCH) -mabi=$(MCU_RISCV_ABI) $(CLFAGS)
MCU_CFLAGS          += -march=$(MCU_RISCV_ARCH) -mabi=$(MCU_RISCV_ABI) $(CFLAGS) 
MCU_CPPFLAGS        += -march=$(MCU_RISCV_ARCH) -mabi=$(MCU_RISCV_ABI) $(CPPFLAGS) 

# LDFLAGS should be first appear in this file. If there is LDFLAGS defined outside, then the line below is ignored.
# LDFLAGS             ?= -T$(AT_SDK_HOME_SH)/$(BOARD_DIR)/$(PLATFORM)/$(CHIP)_$(PLATFORM).lds
LDFLAGS             ?= -T$(LINKER_SCRIPT)
LDFLAGS             += -static -Wl,--gc-sections -Wl,--check-sections --stats -Wl,--print-memory-usage -Wl,-Map=$@.map #-Wl,--print-gc-sections
LDFLAGS             += -Wl,--noinhibit-exec
LDFLAGS             += -march=$(MCU_RISCV_ARCH) -mabi=$(MCU_RISCV_ABI)

endif

CORE_DIR             = ./libraries/core/
DRIVER_DIR           = ./libraries/driver/

include $(AT_SDK_HOME_SH)/$(CORE_DIR)/core-component.mk
C_INCLUDE           += $(CORE_INC)
ASM_SRC             += $(CORE_ASM_SRC)
C_SRC               += $(CORE_C_SRC)

include $(AT_SDK_HOME_SH)/$(DRIVER_DIR)/drv-component.mk
C_INCLUDE           += $(DRV_INC)
C_SRC               += $(DRV_C_SRC)

C_INCLUDE           += $(APP_C_INC)
CPP_INCLUDE         += $(APP_CPP_INC)
ASM_SRC             += $(APP_ASM_SRC)
C_SRC               += $(APP_C_SRC)
CPP_SRC             += $(APP_CPP_SRC)

ASM_OBJS             = $(patsubst %.S, $(BUILDDIR)/%.o, $(ASM_SRC))
C_OBJS               = $(patsubst %.c, $(BUILDDIR)/%.o, $(C_SRC))
CPP_OBJS             = $(patsubst %.cpp, $(BUILDDIR)/%.o, $(CPP_SRC))
OBJS                 = $(ASM_OBJS) $(C_OBJS) $(CPP_OBJS)
BIN                  = $(TARGET)
BIN_DUMP             = $(TARGET).dump

OBJS_DUMP            = $(patsubst %.o, %.dump, $(OBJS))
OBJDUMP_OPT          = -S -D -l -f

DEPS                 = $(call wildcards,$(AT_SDK_HOME_SH)/$(APP_DIR)/$(BUILDDIR),%.d)
LIBS                ?= -nostartfiles $(USERLIB_LDFLAGS) $(SYSLIBS) -lstdc++ -lm -lc -lgcc -nostdlib  -nodefaultlibs

#CLEAN_OBJS += $(TARGET) $(LINK_OBJS) $(TARGET).simple $(SU_OBJS)
all::   dir $(LINK_DEPS) $(GDB_SCRIPT) $(OBJS) $(BIN) $(BIN_DUMP)
dir:
	@mkdir -p $(AT_SDK_HOME_SH)/$(APP_DIR)/$(BUILDDIR)

# Assembly compilation section.
$(ASM_OBJS): $(BUILDDIR)/%.o: $(AT_SDK_HOME_SH)/%.S
	@echo "    SS  $(<F)"
	@mkdir -p $(dir $@)
	@$(CC) $(ASMFLAGS) $(CFLAGS) $(C_INCLUDE) -MD -MF $(basename $@).d -c -o $@ $<

# C Source code compilation section.
$(C_OBJS): $(BUILDDIR)/%.o : $(AT_SDK_HOME_SH)/%.c
	@echo "    CC  $(<F)"
	@mkdir -p $(dir $@)
	@$(CC) $(MCU_CFLAGS) $(C_INCLUDE) -MD -MF $(basename $@).d -c -o $@ $<

# CPP Source code compilation section.
$(CPP_OBJS): $(BUILDDIR)/%.o : $(AT_SDK_HOME_SH)/%.cpp
	@echo "    CX  $(<F)"
	@mkdir -p $(dir $@)
	@$(CXX) $(MCU_CPPFLAGS) $(CPP_INCLUDE) -MD -MF $(basename $@).d -c -o $@ $<

$(BIN): $(OBJS) $(EXTERNAL_OBJ)
	@$(CC) -MMD -MP -o $@ $(LDFLAGS) $+ $(LIBS) $(LIBSFLAGS) $(STRIP)

$(BIN_DUMP): $(BIN)
	@echo "    OBJDUMP  $(<F) > $(@F)"
	@$(OBJDUMP) $(OBJDUMP_OPT) $< > $@
	@$(OBJCOPY) -O binary -j .text -j .data $< $<.bin


force: 
	@touch $(LINKER_TEMPLATE)

$(LINKER_SCRIPT): $(LINKER_TEMPLATE) force
	@$(CC) $(CFLAGS) $(C_INCLUDE) $(LDFLAGS) -I./link -x assembler-with-cpp -nostdinc -undef -E -P $< -o $@

$(GDB_SCRIPT)::
	@cp $(GDB_SCRIPT_TEMPLATE) $@
	@sed -i -e "s/filename/$(TARGET)/g" $@

-include $(DEPS)

clean:
	@rm -rf $(BUILDTOPDIR) $(BIN) $(BIN).map $(BIN_DUMP) $(LINKER_SCRIPT) $(GDB_SCRIPT) -rf
	@rm -rf $(BIN).*bin $(BIN).*img *.ini

.PHONY: clean dir
