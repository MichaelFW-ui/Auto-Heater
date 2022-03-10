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
#include "math.h"

float GetTemperature(void) {
    uint16_t raw = HAL_ADC_GetValue(&hadc1);
    // 以下代码默认开启的是ADC1的Channel 0

    float R9 = 1300000;
    float VCC = 3.29;
    float R_NTC = raw * R9 / (VCC - (float)raw / 4096);

    float B = 3950;
    float T_Const = 273.15 + 25;        // when Rs = 100k Ohm
    float R_Const = 1000000;            // Normal R

    float ret = ((T_Const*B)/(B+T_Const*log(R_NTC/R_Const)))-273.15;
    return ret;
}