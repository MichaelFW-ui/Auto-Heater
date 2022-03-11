/**
 * @file statemachine.h
 * @author Michael Francis Williams (GitHub:Michael-ui)
 * @brief 
 * @version 0.1
 * @date 2022-03-10
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __STATEMACHINE_H__
#define __STATEMACHINE_H__

enum Event {Spin_CW=0x00, Spin_CCW, SW_Short, SW_Hold};

struct StateMachine {
  enum State {
    Normal = 0x00,
    HeatConfirm,
    Heating,
    Change_ChooseParam,
    ChangeTemp,
    ChangeTime,
    ChangeTemp_Value,
    ChangeTime_Value,
    ChangeTempConfirm,
    ChangeTimeConfirm
  } state;

  short param_choose = 0;
  float back_value = 0;

  State OnEvent(Event event);

  State Normal_OnEvent(Event event);
  State HeatConfirm_OnEvent(Event event);
  State Heating_OnEvent(Event event);
  State Change_ChooseParam_OnEvent(Event event);
  State ChangeTemp_OnEvent(Event event);
  State ChangeTime_OnEvent(Event event);
  State ChangeTemp_Value_OnEvent(Event event);
  State ChangeTime_Value_OnEvent(Event event);
  State ChangeTempConfirm_OnEvent(Event event);
  State ChangeTimeConfirm_OnEvent(Event event);

  void Normal_OnInitialize(void);
  void HeatConfirm_OnInitialize(void);
  void Heating_OnInitialize(void);
  void Change_ChooseParam_OnInitialize(void);
  void ChangeTemp_OnInitialize(void);
  void ChangeTime_OnInitialize(void);
  void ChangeTemp_Value_OnInitialize(void);
  void ChangeTime_Value_OnInitialize(void);
  void ChangeTempConfirm_OnInitialize(void);
  void ChangeTimeConfirm_OnInitialize(void);
};

#endif
