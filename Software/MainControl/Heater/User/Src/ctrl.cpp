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

CurveControl curveControl;

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
  uint8_t status = OperatingStatus;
  if (TimeSinceBeginning == TargetTime[status]) {
      ++status;
  }
  CurrentTemperature = GetTemperature();
  TargetTemp =
      (TargetTemperature[status] - TargetTemperature[status - 1]) /
      (TargetTime[status] - TargetTime[status - 1]) *
      (TimeSinceBeginning - TargetTime[status - 1]);

      /*
      TODO
        设置输出量，PID
        输出调试信息
        绘制曲线
      */
}
