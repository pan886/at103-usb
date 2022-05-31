/**
 * @file at103_it.h
 * @brief This file contains the headers of the interrupt handlers.
 * @author liyouchuang (lycdemeng@163.com)
 * @version 1.0
 * @date 2022-03-31
 * @copyright Copyright (c) 2022 Icore, Inc
 */

#ifndef __AT103_IT_H
#define __AT103_IT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "at103.h"

void DMA_Channel1_IRQHandler(void);

#ifdef __cplusplus
}
#endif

#endif /* __AT103_IT_H */