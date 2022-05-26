/**
 * @file at103_it.h
 * @brief This file contains the headers of the interrupt handlers.
 *
 * @author zhangsheng (zhangsheng@zhangsheng.ic.com)
 * @version 1.0
 * @date 2022-02-13
 * @copyright Copyright (c) 2022 Icore, Inc
 */

#ifndef __AT103_IT_H
#define __AT103_IT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "at103.h"

void SPI1_IRQHandler(void);
void SPI2_IRQHandler(void);
void IllegalInstruction_Handler(void);
void ECALL_InstructionExecuted_Handler(void);

#ifdef __cplusplus
}
#endif

#endif /* __AT103_IT_H */