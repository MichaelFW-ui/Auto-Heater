/**
 * @file ctrl.cpp
 * @author Michael Francis Williams (GitHub:Michael-ui)
 * @brief 对加热端的控制
 * @version 0.1
 * @date 2022-02-26
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "ctrl.h"
#include "tim.h"
#include "curve.h"
#include "temp.h"
#include "ui.h"
#include "delay.h"

CurveControl curveControl;

extern uint8_t flag_spin_cw;
extern uint8_t flag_spin_ccw;
extern uint8_t flag_sw_short;
extern uint8_t flag_sw_hold;
extern uint8_t flag_error;



void CurveControl_Init(CurveControl *handle) {
    handle->tim = &htim1;
    handle->Channel = TIM_CHANNEL_2;
    HAL_TIM_Base_Start(handle->tim);
    __HAL_TIM_SetCompare(handle->tim, handle->Channel, 0);
    HAL_TIM_PWM_Start(handle->tim, handle->Channel);
    handle->OperatingStatus = CurveControl::HeatingMode::IDLE;
    handle->CurrentTemperature = GetTemperature();/*  TODO  */
    handle->TargetTemperature[CurveControl::HeatingMode::IDLE] = GetTemperature();
    handle->TargetTemperature[CurveControl::HeatingMode::HEATING] = 150;
    handle->TargetTemperature[CurveControl::HeatingMode::PRESERVING] = 180;
    handle->TargetTemperature[CurveControl::HeatingMode::WELDING_HEAT] = 250;
    handle->TargetTemperature[CurveControl::HeatingMode::WELDING] = 250;
    handle->TargetTemperature[CurveControl::HeatingMode::COOLING] = 0;

    handle->TargetTime[CurveControl::HeatingMode::IDLE] = 0;
    handle->TargetTime[CurveControl::HeatingMode::HEATING] = 90;
    handle->TargetTime[CurveControl::HeatingMode::PRESERVING] = 90 + 80;
    handle->TargetTime[CurveControl::HeatingMode::WELDING_HEAT] = 90 + 80 + 30;
    handle->TargetTime[CurveControl::HeatingMode::WELDING] = 90 + 80 + 30 + 30;
    handle->TargetTime[CurveControl::HeatingMode::COOLING] = 90 + 80 + 30 + 30 + 70;

    displayLCD.lcd = &hLCD;

    handle->pid.Ki = 0;
    handle->pid.Kp = 0;
    handle->pid.Kd = 0;
    handle->pid.I_Term_Max = 0;
    handle->pid.Out_Max = 1000;
    
    handle->pid.Current = GetTemperature();
    handle->pid.Target = GetTemperature();
    handle->pid.I_SeparThresh = 0;
    handle->pid.DeadZone = 0;
    handle->pid.last_tick = HAL_GetTick();
    handle->pid.last_error = 0;
    handle->pid.error_filter.trust = 1;
    handle->pid.d_filter.trust = 1;
  


}
void CurveControl::BeginHeating(void) {
  TimeSinceBeginning = 0;
  OperatingStatus = HeatingMode::HEATING;
}

void CurveControl::OnHeatingOperation(void) {
  if (OperatingStatus == HeatingMode::IDLE) {
    return;
  }
  ++TimeSinceBeginning;
  if (TimeSinceBeginning == TargetTime[OperatingStatus]) {
    switch (OperatingStatus) {
      case HeatingMode::HEATING:
        OperatingStatus = HeatingMode::PRESERVING;
        break;
      case HeatingMode::PRESERVING:
        OperatingStatus = HeatingMode::WELDING_HEAT;
        break;
      case HeatingMode::WELDING_HEAT:
        OperatingStatus = HeatingMode::WELDING;
        break;
      case HeatingMode::WELDING:
        OperatingStatus = HeatingMode::COOLING;
        break;
      case HeatingMode::COOLING:
        OperatingStatus = HeatingMode::IDLE;
        break;
      case HeatingMode::IDLE:
      default:
        break;
    }
  }
  uint8_t status = OperatingStatus;
  CurrentTemperature = GetTemperature();
  TargetTemp =
      (TargetTemperature[status] - TargetTemperature[status - 1]) /
      (TargetTime[status] - TargetTime[status - 1]) *
      (TimeSinceBeginning - TargetTime[status - 1]);
  
  pid.UpdateCurrent(CurrentTemperature);
  
  int16_t out = (int16_t)(pid.Compute());
  if (out < 0) out = 0;

// 输出，打印信息
  switch (OperatingStatus) {
    case HeatingMode::HEATING:
      displayLCD.PrintInformation("HEATING:", out);
      __HAL_TIM_SetCompare(this->tim, this->Channel, (uint16_t)out);
      break;
    case HeatingMode::PRESERVING:
      displayLCD.PrintInformation("PRESERVING:", out);
      __HAL_TIM_SetCompare(this->tim, this->Channel, (uint16_t)out);
      break;
    case HeatingMode::WELDING_HEAT:
      displayLCD.PrintInformation("WELD_HEATING:", out);
      __HAL_TIM_SetCompare(this->tim, this->Channel, (uint16_t)out);
      break;
    case HeatingMode::WELDING:
      displayLCD.PrintInformation("WELDING:", out);
      __HAL_TIM_SetCompare(this->tim, this->Channel, (uint16_t)out);
      break;
    case HeatingMode::COOLING:
      displayLCD.PrintInformation("COOLING:", out);
      __HAL_TIM_SetCompare(this->tim, this->Channel, 0);
      break;
    case HeatingMode::IDLE:
      displayLCD.PrintInformation("IDLE:waiting", 0);
      __HAL_TIM_SetCompare(this->tim, this->Channel, 0);
      break;
    default:
      break;
  }

      /*
      TODO
        设置输出量，PID
        输出调试信息
        绘制曲线
      */

     displayLCD.DrawPointOnCurve(TimeSinceBeginning, CurrentTemperature);
}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) { 
  static uint32_t last_tick_sw = 0;
  if (GPIO_Pin == GPIO_PIN_12) {
    if (EC11_SW_GPIO_Port->IDR & EC11_SW_Pin) {
      last_tick_sw = Delay_GetTick_us();
    } else {
      if (Delay_GetTick_us() - last_tick_sw > 700000) {
        flag_sw_hold = 1;
      } else {
        flag_sw_short = 1;
      }
    }
  }
  if (GPIO_Pin == GPIO_PIN_14) {
    if (EC11_A_GPIO_Port->IDR & EC11_A_Pin) {
      flag_spin_ccw = 1;
    } else {
      flag_spin_cw = 1;
    }
  }
}


