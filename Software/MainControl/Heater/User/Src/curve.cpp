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

Point<int16_t> Bresenham::NextPoint(void) {
  switch (status) {
    case 0:
      if (pi <= 0) {
        pi += 2 * dy;
      } else {
        pi += 2 * dy - 2 * dx;
        yi += 1;
      }
      xi += 1;
      return Point<int16_t>{xi, yi};
      break;
    case 1:
      if (pi <= 0) {
        pi += 2 * dx;
      } else {
        pi += 2 * dx - 2 * dy;
        xi += 1;
      }
      yi += 1;
      return Point<int16_t>{xi, yi};
      break;
    case 2:
      if (pi <= 0) {
        pi += 2 * dy;
      } else {
        pi += 2 * dy - 2 * dx;
        yi += 1;
      }
      xi += 1;
      return Point<int16_t>{xi, static_cast<int16_t>(-yi)};
      break;
    case 3:
      if (pi <= 0) {
        pi += 2 * dx;
      } else {
        pi += 2 * dx - 2 * dy;
        xi += 1;
      }
      yi += 1;
      return Point<int16_t>{xi, static_cast<int16_t>(-yi)};
      break;
    case 4:
      if (pi <= 0) {
        pi += 2 * dy;
      } else {
        pi += 2 * dy - 2 * dx;
        yi += 1;
      }
      xi += 1;
      return Point<int16_t>{static_cast<int16_t>(-xi), yi};
      break;
    case 5:
      if (pi <= 0) {
        pi += 2 * dx;
      } else {
        pi += 2 * dx - 2 * dy;
        xi += 1;
      }
      yi += 1;
      return Point<int16_t>{static_cast<int16_t>(-xi), yi};
      break;
    case 6:
      if (pi <= 0) {
        pi += 2 * dy;
      } else {
        pi += 2 * dy - 2 * dx;
        yi += 1;
      }
      xi += 1;
      return Point<int16_t>{static_cast<int16_t>(-xi),
                            static_cast<int16_t>(-yi)};
      break;
    case 7:
      if (pi <= 0) {
        pi += 2 * dx;
      } else {
        pi += 2 * dx - 2 * dy;
        xi += 1;
      }
      yi += 1;
      return Point<int16_t>{static_cast<int16_t>(-xi),
                            static_cast<int16_t>(-yi)};
      break;
    default:
      return Point<int16_t>{0, 0};
      break;
  }
}
