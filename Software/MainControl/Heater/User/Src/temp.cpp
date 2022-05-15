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

uint32_t ADC_Converted[2];

float GetTemperature(void) {
    uint16_t raw1 = ADC_Converted[0];
    uint16_t raw2 = ADC_Converted[1];



    float R9 = 1300000;
    float VCC = 3.24;
    float v1 = (float) raw1 * VCC / 4096;
    float v2 = (float) raw2 * VCC / 4096;

    float R_NTC;

    if (v1 > 0.2)
        R_NTC = v1 * R9 / (VCC - v1);
    else
        R_NTC = v2 / 20 * R9 / (VCC - v2 / 20);

    float B = 3950;
    float T_Const = 273.15 + 25;        // when Rs = 100k Ohm
    float R_Const = 800000;            // Normal R

    float ret = ((T_Const*B)/(B+T_Const*log(R_NTC/R_Const)))-273.15;
    return ret;
}