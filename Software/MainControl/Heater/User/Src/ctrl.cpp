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

CurveControl curveControl;
Display displayLCD;



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




  void Display::DrawTestPage(void) {
    lcd->Fill(0, 0, LCD_W, LCD_H, RGB(0, 0, 0x0f));
    lcd->Print_String("Here I am", 20, 5, RGB(0,0x2f, 0), FontSize_2412);
    lcd->Print_String("MichaelFW Presents", 20, 35, RGB(0,0,0x1f), FontSize_1206);
    lcd->DrawLine(0, 0, 50, 50, RGB(0x1f, 12, 12));
    lcd->DrawLine(10, 0, 50, 50, RGB(0x1f, 12, 12));
    lcd->DrawLine(20, 0, 50, 50, RGB(0x1f, 12, 12));
    lcd->DrawLine(30, 0, 50, 50, RGB(0x1f, 12, 12));
    lcd->DrawLine(40, 0, 50, 50, RGB(0x1f, 12, 12));
    lcd->DrawLine_WithDelay(80, 40,0, 0, RGB(0x1f, 12, 12), 5);
    lcd->DrawLine_WithDelay(80, 40,40, 0, RGB(0x1f, 12, 12), 5);
    lcd->DrawLine_WithDelay(80, 40,80, 0, RGB(0x1f, 12, 12), 5);
    lcd->DrawLine_WithDelay(80, 40,120, 0, RGB(0x1f, 12, 12), 5);
    lcd->DrawLine_WithDelay(80, 40,160, 0, RGB(0x1f, 12, 12), 5);
    lcd->DrawLine_WithDelay(80, 40,0, 80, RGB(0x1f, 12, 12), 5);
    lcd->DrawLine_WithDelay(80, 40,40, 80, RGB(0x1f, 12, 12), 5);
    lcd->DrawLine_WithDelay(80, 40,80, 80, RGB(0x1f, 12, 12), 5);
    lcd->DrawLine_WithDelay(80, 40,120, 80, RGB(0x1f, 12, 12), 5);
    lcd->DrawLine_WithDelay(80, 40,160, 80, RGB(0x1f, 12, 12), 5);
    lcd->DrawLine_WithDelay(80, 40,0, 40, RGB(0x1f, 12, 12), 5);
    lcd->DrawLine_WithDelay(80, 40,160, 40, RGB(0x1f, 12, 12), 5);

    lcd->DrawLine_WithDelay(30, 70, 150, 80, RGB(0, 0x2f, 0), 10);
    lcd->DrawLine_WithDelay(30, 70, 31, 10, RGB(0, 0x2f, 0), 10);
    lcd->DrawLine_WithDelay(30, 60, 60, 45, RGB(0, 0x2f, 0), 10);
    lcd->DrawLine_WithDelay(60, 45, 90, 50, RGB(0, 0x2f, 0), 10);
  }
  void Display::DrawWelcomePage(void) {
    lcd->Fill(0, 0, LCD_W, LCD_H, RGB(0, 0, 0x0f));
    lcd->Print_String("Welcome!", 37, 10, 0xffff, FontSize_2412);
    lcd->Print_String("Auto Heater", 15, 40, 0xefff, FontSize_2412);
    lcd->Print_String("MichaelFW Presents", 30, 67, 0x0fff, FontSize_1206);
  }

  void Display::PrintInformation(const char *fmt, int16_t out) {
    lcd->Fill(0, 0, LCD_W, split_right.y, color_background);
    lcd->DrawLine(split_left.x, split_left.y, split_right.x, split_right.y, color_split);
    lcd->Print_String(fmt, 5, 3, color_status, FontSize_1206);
    if (out > 100)
      lcd->Fill(145, split_right.y, 155, out / 50, 0xffff);
  }

  void Display::DrawOperationPage(void) {
    y_end.x = 5;
    y_end.y = 28;
    x_end.x = 151;
    x_end.y = 75;
    origin.x = 5;
    origin.y = 75;

    split_left.x = 0;
    split_left.y = 21;
    split_right.x = 160;
    split_right.y = 21;

    color_axis = 0xffff;
    color_split = RGB(0,0x1f,0);
    color_status = RGB(0x1f,0,0);
    color_background = RGB(0,0,0x0f);
    color_heat_line = RGB(0x1f, 0x2f, 0x07);

    lcd->Fill(0, 0, LCD_W, LCD_H, color_background);

    lcd->Print_String("Idle: initializing", 5, 3, color_status, FontSize_1206);
    lcd->DrawLine(split_left.x, split_left.y, split_right.x, split_right.y, color_split);
    lcd->DrawLine_WithDelay(origin.x, origin.y, y_end.x, y_end.y, color_axis, 5);
    lcd->DrawLine_WithDelay(origin.x, origin.y, x_end.x, x_end.y, color_axis, 5);

    lcd->DrawLine(y_end.x, y_end.y, y_end.x - 3, y_end.y + 3, color_axis);
    lcd->DrawLine(y_end.x, y_end.y, y_end.x + 3, y_end.y + 3, color_axis);
    lcd->DrawLine(x_end.x, x_end.y, x_end.x - 3, x_end.y - 3, color_axis);
    lcd->DrawLine(x_end.x, x_end.y, x_end.x - 3, x_end.y + 3, color_axis);

    lcd->Print_String("T/s", x_end.x - 10, x_end.y - 15, color_status, FontSize_1206);
    lcd->Print_String("Temp", y_end.x + 6, y_end.y, color_status, FontSize_1206);

    curveControl.TargetTime;
    curveControl.TargetTemperature;
    CurveControl::HeatingMode::IDLE;

    lcd->DrawLine_WithDelay(
        origin.x,
        origin.y -
            curveControl.TargetTemperature[CurveControl::HeatingMode::IDLE] / 5,
        origin.x +
            curveControl.TargetTime[CurveControl::HeatingMode::HEATING] / 2,
        origin.y -
            curveControl.TargetTemperature[CurveControl::HeatingMode::HEATING] /
                5,
        color_heat_line, 10);

    lcd->DrawLine_WithDelay(
        origin.x +
            curveControl.TargetTime[CurveControl::HeatingMode::HEATING] / 2,
        origin.y -
            curveControl.TargetTemperature[CurveControl::HeatingMode::HEATING] /
                5,
        origin.x +
            curveControl.TargetTime[CurveControl::HeatingMode::PRESERVING] / 2,
        origin.y -
            curveControl
                    .TargetTemperature[CurveControl::HeatingMode::PRESERVING] /
                5,
        color_heat_line, 10);

    lcd->DrawLine_WithDelay(
        origin.x +
            curveControl.TargetTime[CurveControl::HeatingMode::PRESERVING] / 2,
        origin.y -
            curveControl
                    .TargetTemperature[CurveControl::HeatingMode::PRESERVING] /
                5,
        origin.x +
            curveControl.TargetTime[CurveControl::HeatingMode::WELDING_HEAT] /
                2,
        origin.y - curveControl.TargetTemperature
                           [CurveControl::HeatingMode::WELDING_HEAT] /
                       5,
        color_heat_line, 10);

    lcd->DrawLine_WithDelay(
        origin.x +
            curveControl.TargetTime[CurveControl::HeatingMode::WELDING_HEAT] /
                2,
        origin.y - curveControl.TargetTemperature
                           [CurveControl::HeatingMode::WELDING_HEAT] /
                       5,
        origin.x +
            curveControl.TargetTime[CurveControl::HeatingMode::WELDING] / 2,
        origin.y -
            curveControl.TargetTemperature[CurveControl::HeatingMode::WELDING] /
                5,
        color_heat_line, 10);

    lcd->DrawLine_WithDelay(
        origin.x +
            curveControl.TargetTime[CurveControl::HeatingMode::WELDING] / 2,
        origin.y -
            curveControl.TargetTemperature[CurveControl::HeatingMode::WELDING] /
                5,
        origin.x +
            curveControl.TargetTime[CurveControl::HeatingMode::COOLING] / 2,
        origin.y -
            curveControl.TargetTemperature[CurveControl::HeatingMode::COOLING] /
                5,
        color_heat_line, 10);

    lcd->Fill(0, 0, LCD_W, split_right.y, color_background);
    lcd->DrawLine(split_left.x, split_left.y, split_right.x, split_right.y, color_split);
    lcd->Print_String("Idle: waiting", 5, 3, color_status, FontSize_1206);

  }
  void Display::DrawAboutPage(void) {

  }

  void Display::DrawPointOnCurve(uint16_t time, float temp) {
    int16_t x = origin.x + time / 3;
    int16_t y = origin.y - temp / 5;
    lcd->DrawPoint(x, y, color_heat_line);
  }