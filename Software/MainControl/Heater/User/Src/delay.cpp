/**
 * @file delay.cpp
 * @author Michael Francis Williams (GitHub:Michael-ui)
 * @brief 方便实用的延时函数和GetTick(ms)
 * @version 0.1
 * @date 2022-03-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "delay.h"
#include "tim.h"

extern volatile uint32_t TIM_OVERFLOW;

uint32_t Delay_GetTick_us(void) {
    return TIM_OVERFLOW * 1000 + TIM9->CNT;
}

void Delay_us(uint32_t us) { 
    if (us) {
        uint32_t last_tick = Delay_GetTick_us();
        while (Delay_GetTick_us() - last_tick < us) {
            ;
        }
    }
}
