/**
 * @file curve.cpp
 * @author Michael Francis Williams (GitHub:Michael-ui)
 * @brief 提供曲线调控时需要的种种工具
 * @version 0.1
 * @date 2022-02-26
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "curve.h"
#include "cmath"

float PID_TypeDef::Compute(void) {
    Error = Target - Current;

    uint32_t dt = HAL_GetTick() - last_tick;
    last_tick = HAL_GetTick();

    if (std::abs(Error) < DeadZone) {
        Out = 0; return Out;
    }

    Error = error_filter.filter(Error);

    P_Term = Kp * Error;

    if (std::abs(Error) < I_SeparThresh) {
        I_Term += Ki * Error * dt;
        I_Term = constrain(I_Term, -I_Term_Max, I_Term_Max);
    }

    D_Term = Kd * d_filter.filter((Error - last_error) * dt);

    last_error = Error;

    Out = P_Term + I_Term + D_Term;
    Out = constrain(Out, -Out_Max, Out_Max);
    return Out;
}
