/**
 * @file at103_it.h
 * @brief This file contains the headers of the interrupt handlers.
 * @author Dong Qin (dong.qin@timesintelli.com)
 * @version 1.0
 * @date 2022-01-17
 * @copyright Copyright (c) 2022 Timesintelli, Inc
 */

#ifndef __AT103_IT_H
#define __AT103_IT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "at103.h"

void DMA_Channel1_IRQHandler(void);
void DMA_Channel2_IRQHandler(void);
void DMA_Channel3_IRQHandler(void);
void DMA_Channel4_IRQHandler(void);
void DMA_Channel5_IRQHandler(void);
void DMA_Channel6_IRQHandler(void);
void DMA_Channel7_IRQHandler(void);
void DMA_Channel8_IRQHandler(void);

#ifdef __cplusplus
}
#endif

#endif /* __AT103_IT_H */