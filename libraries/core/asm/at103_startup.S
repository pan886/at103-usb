/**
 * @file at103_startup.S
 * @brief Device Startup File for AT103. 
 * @author Dong Qin (dong.qin@timesintelli.com)
 * @version 1.0
 * @date 2022-01-07
 * @copyright Copyright (c) 2022 Timesintelli, Inc
 */

                /* Constants to define the additional registers found on the Pulpino RI5KY. */
                .equ     lpstart0, 0x7b0
                .equ     lpend0, 0x7b1
                .equ     lpcount0, 0x7b2
                .equ     lpstart1, 0x7b4
                .equ     lpend1, 0x7b5
                .equ     lpcount1, 0x7b6
                .equ     portasmADDITIONAL_CONTEXT_SIZE, 0x6

                /* Save additional registers found on the Pulpino. */
                .macro   portasmSAVE_ADDITIONAL_REGISTERS
                addi     sp, sp, -(portasmADDITIONAL_CONTEXT_SIZE * 4)
                csrr     t0, lpstart0
                csrr     t1, lpend0
                csrr     t2, lpcount0
                csrr     t3, lpstart1
                csrr     t4, lpend1
                csrr     t5, lpcount1
                sw       t0, 0x0(sp)
                sw       t1, 0x4(sp)
                sw       t2, 0x8(sp)
                sw       t3, 0xC(sp)
                sw       t4, 0x10(sp)
                sw       t5, 0x14(sp)
                .endm

                /* Restore the additional registers found on the Pulpino. */
                .macro   portasmRESTORE_ADDITIONAL_REGISTERS
                lw       t0, 0x0(sp)
                lw       t1, 0x4(sp)
                lw       t2, 0x8(sp)
                lw       t3, 0xC(sp)
                lw       t4, 0x10(sp)
                lw       t5, 0x14(sp)
                csrw     lpstart0, t0
                csrw     lpend0, t1
                csrw     lpcount0, t2
                csrw     lpstart1, t3
                csrw     lpend1, t4
                csrw     lpcount1, t5
                addi     sp, sp, (portasmADDITIONAL_CONTEXT_SIZE * 4 )
                .endm

                /* Save general and system registers */
                .macro   save_context
                addi     sp, sp, -0x7C
                sw       x1, 0x0(sp)
                sw       x4, 0x4(sp)
                sw       x5, 0x8(sp)
                sw       x6, 0xC(sp)
                sw       x7, 0x10(sp)
                sw       x8, 0x14(sp)
                sw       x9, 0x18(sp)
                sw       x10, 0x1C(sp)
                sw       x11, 0x20(sp)
                sw       x12, 0x24(sp)
                sw       x13, 0x28(sp)
                sw       x14, 0x2C(sp)
                sw       x15, 0x30(sp)
                sw       x16, 0x34(sp)
                sw       x17, 0x38(sp)
                sw       x18, 0x3C(sp)
                sw       x19, 0x40(sp)
                sw       x20, 0x44(sp)
                sw       x21, 0x48(sp)
                sw       x22, 0x4C(sp)
                sw       x23, 0x50(sp)
                sw       x24, 0x54(sp)
                sw       x25, 0x58(sp)
                sw       x26, 0x5C(sp)
                sw       x27, 0x60(sp)
                sw       x28, 0x64(sp)
                sw       x29, 0x68(sp)
                sw       x30, 0x6C(sp)
                sw       x31, 0x70(sp)
                
                csrr     t0, mstatus
                sw       t0, 0x74(sp)
                csrr     t0, mepc
                sw       t0, 0x78(sp)
                portasmSAVE_ADDITIONAL_REGISTERS
                .endm




                .macro   restore_context
                portasmRESTORE_ADDITIONAL_REGISTERS
                lw       t0, 0x74(sp)
                csrw     mstatus, t0
                lw       t0, 0x78(sp)
                csrw     mepc, t0

                lw       x1, 0x0(sp)
                lw       x4, 0x4(sp)
                lw       x5, 0x8(sp)
                lw       x6, 0xC(sp)
                lw       x7, 0x10(sp)
                lw       x8, 0x14(sp)
                lw       x9, 0x18(sp)
                lw       x10, 0x1C(sp)
                lw       x11, 0x20(sp)
                lw       x12, 0x24(sp)
                lw       x13, 0x28(sp)
                lw       x14, 0x2C(sp)
                lw       x15, 0x30(sp)
                lw       x16, 0x34(sp)
                lw       x17, 0x38(sp)
                lw       x18, 0x3C(sp)
                lw       x19, 0x40(sp)
                lw       x20, 0x44(sp)
                lw       x21, 0x48(sp)
                lw       x22, 0x4C(sp)
                lw       x23, 0x50(sp)
                lw       x24, 0x54(sp)
                lw       x25, 0x58(sp)
                lw       x26, 0x5C(sp)
                lw       x27, 0x60(sp)
                lw       x28, 0x64(sp)
                lw       x29, 0x68(sp)
                lw       x30, 0x6C(sp)
                lw       x31, 0x70(sp)
                addi     sp, sp, 0x7C
                .endm

/* Hardware interrupt vector table */
                .section .vectors
                .align   2
                .globl   __HW_Vectors
                .globl   __HW_Vectors_End
                .globl   __HW_Vectors_Size
__HW_Vectors:
                j        HW_WWDG_IRQHandler
                j        HW_RCC_IRQHandler
                j        HW_TAMPER_IRQHandler
                j        HW_RTC_IRQHandler
                j        HW_EXTI0_IRQHandler
                j        HW_EXTI1_IRQHandler
                j        HW_EXTI2_IRQHandler
                j        HW_EXTI3_IRQHandler
                j        HW_EXTI4_IRQHandler
                j        HW_DMA_IRQHandler
                j        HW_ADC1_2_IRQHandler
                j        HW_CAN_IRQHandler
                j        HW_EXTI9_5_IRQHandler
                j        HW_TIM1_BRK_IRQHandler
                j        HW_TIM1_UP_IRQHandler
                j        HW_TIM1_TRG_COM_IRQHandler
                j        HW_TIM1_CC_IRQHandler
                j        HW_TIM2_IRQHandler
                j        HW_TIM3_IRQHandler
                j        HW_TIM4_IRQHandler
                j        HW_I2C1_IRQHandler
                j        HW_I2C2_IRQHandler
                j        HW_SPI1_IRQHandler
                j        HW_SPI2_IRQHandler
                j        HW_USART1_IRQHandler
                j        HW_USART2_IRQHandler
                j        HW_USART3_IRQHandler
                j        HW_EXTI15_10_IRQHandler
                j        HW_USB_MC_IRQHandler
                j        HW_USB_DMA_IRQHandler
                .word    0
                .word    0
                .word    0
                .word    0
                .word    0
                .word    0
                .word    0
                .word    0
                .word    0
                .word    0
                .word    0
                .word    0
                .word    0
                .word    0
                .word    0
                .word    0
                .word    0
                .word    0
                .word    0
                .word    0
                .word    0
                .word    0
                .word    0
                .word    0
                .word    0
                .word    0
                .word    0
                .word    0
                .word    0
                .word    0
                j        Reset_Handler
                j        IllegalInstruction_Handler
                j        ECALL_InstructionExecuted_Handler
                .word    0
__HW_Vectors_End:
                .equ     __HW_Vectors_Size, __HW_Vectors_End - __HW_Vectors
                .size    __HW_Vectors, . - __HW_Vectors


/* This is the code that gets called when the processor first starts execution
 * following a reset event. Only the absolutely necessary set is performed,
 * after which the application supplied main() routine is called.
 */
                .text
                .align   2
                .type    Reset_Handler, %function
                .globl   Reset_Handler
                .func    Reset_Handler
Reset_Handler:
                /* disable interrupt intensionly by single instruction. */
                csrrci   a0, mstatus, 0x8
                /* Set MSTATUS : Machine Mode */
                li       a0, 0x1800
                csrw     mstatus, a0
                /* Set MTVEC */
                la       a0, __HW_Vectors
                csrw     mtvec, a0
                /* init global pointer(global var) */
                la       gp, __data_start
                /* init stack pointer */
                la       sp, __stack_top

/* fill .bss section, .bss section is 4-byte aligned. */
_fill_bss:
                .extern  __bss_start
                .extern  __bss_end
                la       a0, __bss_start
                la       a1, __bss_end
.LC1:
                sw       x0, (a0)
                addi     a0, a0, 4
                bltu     a0, a1, .LC1

#ifndef CONFIG_NO_FALSH
/* copy .data section, .data section must be 4-byte aligned. */
_copy_data:
                .extern __etext
                .extern __data_start
                .extern __data_end
                la       a0, __etext
                la       a1, __data_start
                la       a2, __data_end
.LC2:
                lw       a3, 0(a0)
                sw       a3, 0(a1)
                addi     a0, a0, 4
                addi     a1, a1, 4
                bltu     a1, a2, .LC2
#endif
                .extern  _SysInit
                call     _SysInit

                /* argc = argv = 0 */
                li       a0, 0
                li       a1, 0
                .extern  main
                call     main

                .endfunc
                .size    Reset_Handler, . - Reset_Handler


/* Software interrupt vector table */
                .text
                .align   2
                .globl   __Vectors
                .globl   __Vectors_End
                .globl   __Vectors_Size
__Vectors:
                .word    WWDG_IRQHandler
                .word    RCC_IRQHandler
                .word    TAMPER_IRQHandler
                .word    RTC_IRQHandler
                .word    EXTI0_IRQHandler
                .word    EXTI1_IRQHandler
                .word    EXTI2_IRQHandler
                .word    EXTI3_IRQHandler
                .word    EXTI4_IRQHandler
                .word    DMA_IRQHandler
                .word    ADC1_2_IRQHandler
                .word    CAN_IRQHandler
                .word    EXTI9_5_IRQHandler
                .word    TIM1_BRK_IRQHandler
                .word    TIM1_UP_IRQHandler
                .word    TIM1_TRG_COM_IRQHandler
                .word    TIM1_CC_IRQHandler
                .word    TIM2_IRQHandler
                .word    TIM3_IRQHandler
                .word    TIM4_IRQHandler
                .word    I2C1_IRQHandler
                .word    I2C2_IRQHandler
                .word    SPI1_IRQHandler
                .word    SPI2_IRQHandler
                .word    USART1_IRQHandler
                .word    USART2_IRQHandler
                .word    USART3_IRQHandler
                .word    EXTI15_10_IRQHandler
                .word    USB_MC_IRQHandler
                .word    USB_DMA_IRQHandler
__Vectors_End:
                .equ     __Vectors_Size, __Vectors_End - __Vectors
                .size    __Vectors, . - __Vectors


/* Peripherals interrupt unified function */
                .text
                .align   2
                .type    IRQ_Handler, %function
                .weak    IRQ_Handler
                .func    IRQ_Handler
IRQ_Handler:
                save_context
                la       t0, __Vectors
                csrr     t1, mcause
                slli     t1, t1, 2
                add      t0, t1, t0
                lw	     t0,0(t0)
                jalr     t0
                restore_context
                mret
                .endfunc
                .size    IRQ_Handler, . - IRQ_Handler


                .macro   Set_IRQ_Handler  Handler_Name
                .weak    \Handler_Name
                .set     \Handler_Name, IRQ_Handler
                .endm

/* Hardware unified interrupt handler */
                Set_IRQ_Handler  HW_WWDG_IRQHandler
                Set_IRQ_Handler  HW_RCC_IRQHandler
                Set_IRQ_Handler  HW_TAMPER_IRQHandler
                Set_IRQ_Handler  HW_RTC_IRQHandler
                Set_IRQ_Handler  HW_EXTI0_IRQHandler
                Set_IRQ_Handler  HW_EXTI1_IRQHandler
                Set_IRQ_Handler  HW_EXTI2_IRQHandler
                Set_IRQ_Handler  HW_EXTI3_IRQHandler
                Set_IRQ_Handler  HW_EXTI4_IRQHandler
                Set_IRQ_Handler  HW_DMA_IRQHandler
                Set_IRQ_Handler  HW_ADC1_2_IRQHandler
                Set_IRQ_Handler  HW_CAN_IRQHandler
                Set_IRQ_Handler  HW_EXTI9_5_IRQHandler
                Set_IRQ_Handler  HW_TIM1_BRK_IRQHandler
                Set_IRQ_Handler  HW_TIM1_UP_IRQHandler
                Set_IRQ_Handler  HW_TIM1_TRG_COM_IRQHandler
                Set_IRQ_Handler  HW_TIM1_CC_IRQHandler
                Set_IRQ_Handler  HW_TIM2_IRQHandler
                Set_IRQ_Handler  HW_TIM3_IRQHandler
                Set_IRQ_Handler  HW_TIM4_IRQHandler
                Set_IRQ_Handler  HW_I2C1_IRQHandler
                Set_IRQ_Handler  HW_I2C2_IRQHandler
                Set_IRQ_Handler  HW_SPI1_IRQHandler
                Set_IRQ_Handler  HW_SPI2_IRQHandler
                Set_IRQ_Handler  HW_USART1_IRQHandler
                Set_IRQ_Handler  HW_USART2_IRQHandler
                Set_IRQ_Handler  HW_USART3_IRQHandler
                Set_IRQ_Handler  HW_EXTI15_10_IRQHandler
                Set_IRQ_Handler  HW_USB_MC_IRQHandler
                Set_IRQ_Handler  HW_USB_DMA_IRQHandler

/* Default interrupt handler */
                .text
                .align   2
                .type    Default_Handler, %function
                .weak    Default_Handler
                .func    Default_Handler
Default_Handler:
Infinite_Loop:
                j        Infinite_Loop
                .endfunc
                .size    Default_Handler, . - Default_Handler

/* Macro to define default interrupt handlers.
 * Default handler are weak symbols with an endless loop.
 * They can be overwritten by real handlers.
 */
                .macro   Set_Default_Handler  Handler_Name
                .weak    \Handler_Name
                .set     \Handler_Name, Default_Handler
                .endm

                .text
                .align   2
                .type    IllegalInstruction_Handler, %function
                .weak    IllegalInstruction_Handler
                .func    IllegalInstruction_Handler
IllegalInstruction_Handler:
1: 
                j        1b
                .endfunc
                .size    IllegalInstruction_Handler, . - IllegalInstruction_Handler

                .text
                .align   2
                .type    ECALL_InstructionExecuted_Handler, %function
                .weak    ECALL_InstructionExecuted_Handler
                .func    ECALL_InstructionExecuted_Handler
ECALL_InstructionExecuted_Handler:
1:
                j        1b
                .endfunc
                .size    ECALL_InstructionExecuted_Handler, . - ECALL_InstructionExecuted_Handler

/* Provide weak aliases for each Exception handler to the Default_Handler. 
 * As they are weak aliases, any function with the same name will override 
 * this definition.
 */
                Set_Default_Handler  WWDG_IRQHandler
                Set_Default_Handler  RCC_IRQHandler
                Set_Default_Handler  TAMPER_IRQHandler
                Set_Default_Handler  RTC_IRQHandler
                Set_Default_Handler  EXTI0_IRQHandler
                Set_Default_Handler  EXTI1_IRQHandler
                Set_Default_Handler  EXTI2_IRQHandler
                Set_Default_Handler  EXTI3_IRQHandler
                Set_Default_Handler  EXTI4_IRQHandler
                Set_Default_Handler  DMA_IRQHandler
                Set_Default_Handler  ADC1_2_IRQHandler
                Set_Default_Handler  CAN_IRQHandler
                Set_Default_Handler  EXTI9_5_IRQHandler
                Set_Default_Handler  TIM1_BRK_IRQHandler
                Set_Default_Handler  TIM1_UP_IRQHandler
                Set_Default_Handler  TIM1_TRG_COM_IRQHandler
                Set_Default_Handler  TIM1_CC_IRQHandler
                Set_Default_Handler  TIM2_IRQHandler
                Set_Default_Handler  TIM3_IRQHandler
                Set_Default_Handler  TIM4_IRQHandler
                Set_Default_Handler  I2C1_IRQHandler
                Set_Default_Handler  I2C2_IRQHandler
                Set_Default_Handler  SPI1_IRQHandler
                Set_Default_Handler  SPI2_IRQHandler
                Set_Default_Handler  USART1_IRQHandler
                Set_Default_Handler  USART2_IRQHandler
                Set_Default_Handler  USART3_IRQHandler
                Set_Default_Handler  EXTI15_10_IRQHandler
                Set_Default_Handler  USB_MC_IRQHandler
                Set_Default_Handler  USB_DMA_IRQHandler

                .end