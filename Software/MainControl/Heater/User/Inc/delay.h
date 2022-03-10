/**
 * @file delay.h
 * @author Michael Francis Williams (GitHub:Michael-ui)
 * @brief 方便实用的延时函数和GetTick(ms)
 * @version 0.1
 * @date 2022-03-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __DELAY_H_
#define __DELAY_H_

#include "stm32f4xx.h"

uint32_t Delay_GetTick_us(void);

void Delay_us(uint32_t us);

#endif // !__DELAY_H_

