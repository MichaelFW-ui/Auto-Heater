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

template<typename T>
struct Point {
  T x, y;
};

/**
 * @brief 绘制直线的高阶算法
 * 
 */
struct Bresenham {
    int16_t x1, x2;
    int16_t y1, y2;

    int16_t dx = x2 - x1, dy = y2 - y1;

    int16_t pi, xi, yi;

    uint8_t status=0;
    /*
    *   0: dx > 0, dy > 0, |dy| < |dx|
    *   1: dx > 0, dy > 0, |dy| > |dx|
    *   2: dx > 0, dy < 0, |dy| < |dx|
    *   3: dx > 0, dy < 0, |dy| > |dx|
    *   4: dx < 0, dy > 0, |dy| < |dx|
    *   5: dx < 0, dy > 0, |dy| > |dx|
    *   6: dx < 0, dy < 0, |dy| < |dx|
    *   7: dx < 0, dy < 0, |dy| > |dx|
    *   8: dx = 0
    *   9: dy = 0
    */
    Bresenham(int16_t x1, int16_t y1, int16_t x2, int16_t y2)
        : x1(x1), y1(y1), x2(x2), y2(y2) {
          dx = x2 - x1;
          dy = y2 - y1;
          status = 0;
      if (dx > 0 && dy > 0 && dy <= dx) { status = 0; }
      if (dx > 0 && dy > 0 && dy > dx) { status = 1; }
      if (dx > 0 && dy < 0 && -dy <= dx) { status = 2; dy = -dy;}
      if (dx > 0 && dy < 0 && -dy > dx) { status = 3; dy = -dy;}
      if (dx < 0 && dy > 0 && dy <= -dx) { status = 4; dx = -dx;}
      if (dx < 0 && dy > 0 && dy > -dx) { status = 5; dx = -dx;}
      if (dx < 0 && dy < 0 && -dy <= -dx) { status = 6; dx = -dx; dy = -dy;}
      if (dx < 0 && dy < 0 && -dy > -dx) { status = 7; dx = -dx; dy = -dy;}
      if (dx == 0) status = 8;
      if (dy == 0) status = 9;

      switch (status) {
        case 0:
          xi = x1;
          yi = y1;
          pi = 2 * dy - dx;
          break;
        case 1:
          xi = x1;
          yi = y1;
          pi = 2 * dx - dy;
          break;
        case 2:
          xi = x1;
          yi = -y1;
          pi = 2 * dy - dx;
          break;
        case 3:
          xi = x1;
          yi = -y1;
          pi = 2 * dx - dy;
          break;
        case 4:
          xi = -x1;
          yi = y1;
          pi = 2 * dy - dx;
          break;
        case 5:
          xi = -x1;
          yi = y1;
          pi = 2 * dx - dy;
          break;
        case 6:
          xi = -x1;
          yi = -y1;
          pi = 2 * dy - dx;
          break;
        case 7:
          xi = -x1;
          yi = -y1;
          pi = 2 * dx - dy;
          break;
        case 8:
        case 9:
          xi = x1;
          yi = y1;
        default:
          break;
      }
    }

    Point<int16_t> NextPoint(void);
    uint8_t empty(void);
};

#endif // !__CURVE_H_

