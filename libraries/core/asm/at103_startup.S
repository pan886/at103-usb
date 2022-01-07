/**************************************************************************//**
 * @file     at103_startup.S
 * @brief    Device Startup File for TS103
 * @version  V1.0
 * @date     2022.1.5
 ******************************************************************************/

                .section .vectors
                .align   2
                .globl   __Vectors
                .globl   __Vectors_End
                .globl   __Vectors_Size
__Vectors:
                .word    WDT_IRQHandler
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
                .word    Reset_Handler
                .word    IllegalInstruction_Handler
                .word    ECALL_InstructionExecuted_Handler
                .word    0
__Vectors_End:
                .equ     __Vectors_Size, __Vectors_End - __Vectors
                .size    __Vectors, . - __Vectors

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
                la       a0, __Vectors
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
                /* copy .data section, .data section must be 4-byte aligned. */
#ifndef CONFIG_NO_FALSH
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


                .type    Default_Handler, %function
                .weak    Default_Handler
                .func    Default_Handler
Default_Handler:
Infinite_Loop:
                j        Infinite_Loop
                .endfunc
                .size    Default_Handler, . - Default_Handler

/* Macro to define default exception/interrupt handlers.
 * Default handler are weak symbols with an endless loop.
 * They can be overwritten by real handlers.
 */
                .macro   Set_Default_Handler  Handler_Name
                .weak    \Handler_Name
                .set     \Handler_Name, Default_Handler
                .endm

/* Default exception/interrupt handler */
                Set_Default_Handler  WDT_IRQHandler
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
                Set_Default_Handler  IllegalInstruction_Handler
                Set_Default_Handler  ECALL_InstructionExecuted_Handler

                .end