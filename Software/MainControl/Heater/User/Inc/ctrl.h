/**
 * @file ctrl.h
 * @author Michael Francis Williams (GitHub:Michael-ui)
 * @brief 
 * @version 0.1
 * @date 2022-02-26
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __CTRL_H_
#define __CTRL_H_

#include "stm32f4xx.h"
#include "tim.h"
#include "lcd.h"
struct CurveControl {
    TIM_HandleTypeDef *tim;
    uint32_t Channel;

    enum HeatingMode {IDLE = 0x00, HEATING, PRESERVING, WELDING_HEAT, WELDING, COOLING} OperatingStatus;
    uint16_t TimeSinceBeginning;
    float CurrentTemperature;
    float TargetTemp;
    uint32_t TargetTime[10];
    float TargetTemperature[10];
    /*
     * TT[] : IDLE:0,     HEATING:1,       PRESERVING:2,     WELDING:3, COOLING:4
     *        Default,   Temp to keep,   temp for stable,   temp when welding,  Let go!
    */

    void SetTargetTemperature(HeatingMode status, float temp) {
      TargetTemperature[status] = temp;
    }
    void UpdateCurrentTemperature(float temp) { CurrentTemperature = temp; }

    void OnHeatingOperation(void);
    void BeginHeating(void);
};

extern CurveControl curveControl;

void CurveControl_Init(CurveControl *handle);

struct Display {
  lcd_st7735s * lcd;

  void DrawTestPage(void);
  void DrawOperationPage(void);
  void DrawAboutPage(void);
  void DrawWelcomePage(void);
};

extern Display displayLCD;


#endif // !__CTRL_H_
