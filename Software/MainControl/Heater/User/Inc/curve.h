/**
 * @file curve.h
 * @author Michael Francis Williams (GitHub:Michael-ui)
 * @brief 
 * @version 0.1
 * @date 2022-02-26
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __CURVE_H_
#define __CURVE_H_

#include "stm32f4xx.h"

struct LowPassFilter {
    private:
    float last_value, current_value;
    public:
    float trust;
    LowPassFilter():trust(0), current_value(0), last_value(0) {}
    LowPassFilter(float trust):trust(trust), current_value(0), last_value(0) {}
    float filter(float value) {
        last_value = current_value;
       return current_value = last_value * (1 - trust) + value * trust;
    }
};



struct PID_TypeDef {
    float Ki, Kp, Kd;
    float I_Term_Max;
    float Out_Max;

    float Target, Current, Error;

    float Out;

    float I_Term, P_Term, D_Term;

    float I_SeparThresh;
    // 积分分离阈值

    float DeadZone;

    uint32_t last_tick;
    float last_error;

    LowPassFilter error_filter = LowPassFilter(1);
    LowPassFilter d_filter = LowPassFilter(1);

    void UpdateCurrent(float current) {
        Current = current;
    }

    float Compute(void);
};

__STATIC_INLINE float constrain(float value, float min, float max) {
    if (value > max) return max;
    if (value < min) return min;
    return value;
}

#endif // !__CURVE_H_

