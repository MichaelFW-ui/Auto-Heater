/**
 * @file temp.cpp
 * @author Michael Francis Williams (GitHub:Michael-ui)
 * @brief 读取温度，传统C编程结构
 * @version 0.1
 * @date 2022-03-06
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "temp.h"
#include "adc.h"

float GetTemperature(void) {
    uint16_t raw = HAL_ADC_GetValue(&hadc1);
    // 以下代码默认开启的是ADC1的Channel 0

    float temp = /*TODO*/ raw;
    return temp;
}