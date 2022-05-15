/**
 * @file statemachine.cpp
 * @author Michael Francis Williams (GitHub:Michael-ui)
 * @brief 尝试状态机编程
 * @version 0.1
 * @date 2022-03-10
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "statemachine.h"

#include "ctrl.h"
#include "ui.h"
#include "cmsis_os.h"

StateMachine::State StateMachine::OnEvent(Event event) {
  switch (state) {
    case Normal:
      return Normal_OnEvent(event);
      break;
    case HeatConfirm:
      return HeatConfirm_OnEvent(event);
      break;
    case Heating:
      return Heating_OnEvent(event);
      break;
    case Change_ChooseParam:
      return Change_ChooseParam_OnEvent(event);
      break;
    case ChangeTemp:
      return ChangeTemp_OnEvent(event);
      break;
    case ChangeTime:
      return ChangeTime_OnEvent(event);
      break;
    case ChangeTemp_Value:
      return ChangeTemp_Value_OnEvent(event);
      break;
    case ChangeTime_Value:
      return ChangeTime_Value_OnEvent(event);
      break;
    case ChangeTempConfirm:
      return ChangeTempConfirm_OnEvent(event);
      break;
    case ChangeTimeConfirm:
      return ChangeTimeConfirm_OnEvent(event);
      break;
    default:
      return Normal;
  }
}

StateMachine::State StateMachine::Normal_OnEvent(Event event) {
    static uint8_t row = 0;
    const char *msg[] = {"Idle:Waiting", "To ABOUT page?", "To HELP page?", "To CHANGE mode?", "To HEATING mode?"};
    StateMachine::State ret = Normal;
    if (event == Spin_CW) {
        row = (row + 1) % 5;
        displayLCD.PrintInformation(msg[row], 0);
    }

    if (event == Spin_CCW) {
        row = (row + 4) % 5;
        displayLCD.PrintInformation(msg[row], 0);
    }

    if (event == SW_Hold) {
      switch (row) {
        case 1:
          displayLCD.DrawAboutPage();
          osDelay(5000);
          displayLCD.PrintInformation(msg[row], 0);
          displayLCD.ClearCurveArea();
          break;
        case 2:
          displayLCD.DrawHelpPage();
          osDelay(5000);
          displayLCD.PrintInformation(msg[row], 0);
          displayLCD.ClearCurveArea();
          break;
        case 3:
          ret = Change_ChooseParam;
          Change_ChooseParam_OnInitialize();
          break;
        case 4:
          ret = HeatConfirm;
          HeatConfirm_OnInitialize();
          break;
        default:
          break;
      }
    }

    if (event == SW_Short)
        ;
    return ret;
}
StateMachine::State StateMachine::HeatConfirm_OnEvent(Event event) {
    StateMachine::State ret = HeatConfirm;
    if (event == SW_Hold) {
        ret  = Heating;
        Heating_OnInitialize();
    }
    if (event == SW_Short) {
        ret = Normal;
        Normal_OnInitialize();
    }
    return ret;
}
StateMachine::State StateMachine::Heating_OnEvent(Event event) {
    StateMachine::State ret = Heating;
    if (event == SW_Short) {
        ret = Normal;
        Normal_OnInitialize();
        curveControl.HaltHeat();
    }
    return Heating;
}
StateMachine::State StateMachine::Change_ChooseParam_OnEvent(Event event) {
    const char * msg[] = {"Change time?", "Change temperature?"};
    static uint8_t row = 0;

    StateMachine::State ret = Change_ChooseParam;
    if (event == Spin_CW) {
        row = (row + 1) % 2;
        displayLCD.PrintInformation(msg[row], 0);
    }

    if (event == Spin_CCW) {
        row = (row + 1) % 2;
        displayLCD.PrintInformation(msg[row], 0);
    }
    if (event == SW_Hold) {
      switch (row) {
        case 0:
          ret = ChangeTime;
          ChangeTime_OnInitialize();
          break;
        case 1:
          ret = ChangeTemp;
          ChangeTemp_OnInitialize();
          break;
        default:
          break;
      }
    }
    if (event == SW_Short) {
        ret = Normal;
        Normal_OnInitialize();
    }
    return ret;
}
StateMachine::State StateMachine::ChangeTemp_OnEvent(Event event) {
    const char * msg = "CW/CCW to choose:%d";

    StateMachine::State ret = ChangeTemp;
    if (event == Spin_CW) {
        param_choose = (param_choose) % 4 + 1;
        displayLCD.PrintInfoArg(msg, param_choose);
        displayLCD.ClearCurveArea();
        displayLCD.DrawAxisOnPoint(param_choose);
    }

    if (event == Spin_CCW) {
        param_choose = (param_choose + 2) % 4 + 1;
        displayLCD.PrintInfoArg(msg, param_choose);
        displayLCD.ClearCurveArea();
        displayLCD.DrawAxisOnPoint(param_choose);
    }
    if (event == SW_Hold) {
        ret = ChangeTemp_Value;
        ChangeTemp_Value_OnInitialize();
    }
    if (event == SW_Short) {
        ret = Normal;
        Normal_OnInitialize();
    }
    return ret;
}
StateMachine::State StateMachine::ChangeTime_OnEvent(Event event) {
    const char * msg = "CW/CCW to choose:%d";

    StateMachine::State ret = ChangeTime;
    if (event == Spin_CW) {
        param_choose = (param_choose) % 4 + 1;
        displayLCD.PrintInfoArg(msg, param_choose);
        displayLCD.ClearCurveArea();
        displayLCD.DrawAxisOnPoint(param_choose);
    }

    if (event == Spin_CCW) {
        param_choose = (param_choose + 2) % 4 + 1;
        displayLCD.PrintInfoArg(msg, param_choose);
        displayLCD.ClearCurveArea();
        displayLCD.DrawAxisOnPoint(param_choose);
    }
    if (event == SW_Hold) {
        ret = ChangeTime_Value;
        ChangeTime_Value_OnInitialize();
    }
    if (event == SW_Short) {
        ret = Normal;
        Normal_OnInitialize();
    }
    return ret;
}
StateMachine::State StateMachine::ChangeTemp_Value_OnEvent(Event event) {
    const char * msg = "CW/CCW to change:%f";
    
    float tmp = curveControl.TargetTemperature[param_choose];

    StateMachine::State ret = ChangeTemp_Value;
    if (event == Spin_CW) {
        tmp += 2;
        displayLCD.PrintInfoArg(msg, tmp);
        curveControl.TargetTemperature[param_choose] = tmp;
        displayLCD.ClearCurveArea();
    }

    if (event == Spin_CCW) {
        tmp -= 2;
        displayLCD.PrintInfoArg(msg, tmp);
        curveControl.TargetTemperature[param_choose] = tmp;
        displayLCD.ClearCurveArea();
    }
    if (event == SW_Hold) {
        ret = ChangeTempConfirm;
        ChangeTempConfirm_OnInitialize();
    }
    if (event == SW_Short) {
        ret = Normal;
        Normal_OnInitialize();
    }
    return ret;
}
StateMachine::State StateMachine::ChangeTime_Value_OnEvent(Event event) {
    const char * msg = "CW/CCW to change:%f";
    
    float tmp = curveControl.TargetTime[param_choose];

    StateMachine::State ret = ChangeTime_Value;
    if (event == Spin_CW) {
        tmp += 2;
        displayLCD.PrintInfoArg(msg, tmp);
        curveControl.TargetTime[param_choose] = tmp;
        displayLCD.ClearCurveArea();
    }

    if (event == Spin_CCW) {
        tmp -= 2;
        displayLCD.PrintInfoArg(msg, tmp);
        curveControl.TargetTime[param_choose] = tmp;
        displayLCD.ClearCurveArea();
    }
    if (event == SW_Hold) {
        ret = ChangeTimeConfirm;
        ChangeTimeConfirm_OnInitialize();
    }
    if (event == SW_Short) {
        ret = Normal;
        Normal_OnInitialize();
    }
    return ret;
}
StateMachine::State StateMachine::ChangeTempConfirm_OnEvent(Event event) {
    StateMachine::State ret = ChangeTempConfirm;
    if (event == SW_Hold) {
        ret = Normal;
        Normal_OnInitialize();
    }
    if (event == SW_Short) {
        ret = Normal;
        curveControl.TargetTemperature[param_choose] = back_value;
        back_value = 0;
        displayLCD.ClearCurveArea();
        Normal_OnInitialize();
    }
    return ret;
}
StateMachine::State StateMachine::ChangeTimeConfirm_OnEvent(Event event) {
    StateMachine::State ret = ChangeTimeConfirm;
    if (event == SW_Hold) {
        ret = Normal;
        Normal_OnInitialize();
    }
    if (event == SW_Short) {
        ret = Normal;
        curveControl.TargetTime[param_choose] = back_value;
        back_value = 0;
        displayLCD.ClearCurveArea();
        Normal_OnInitialize();
    }
    return ret;
}

void StateMachine::Normal_OnInitialize(void) {
    const char * msg = "Idle:Waiting";
    displayLCD.PrintInformation(msg, 0);
    displayLCD.ClearCurveArea();
}
void StateMachine::HeatConfirm_OnInitialize(void) {
    const char * msg = "Ready to HEAT?";
    displayLCD.PrintInformation(msg, 0);
    // const char * msg = "NOT AVAILABLE";
    // displayLCD.PrintInformation(msg, 0);
}
void StateMachine::Heating_OnInitialize(void) {
    // const char * msg = "NOT AVAILABLE";
    // displayLCD.PrintInformation(msg, 0);
    curveControl.BeginHeating();
}
void StateMachine::Change_ChooseParam_OnInitialize(void) {
    const char * msg = "Change time?";
    displayLCD.PrintInformation(msg, 0);
}
void StateMachine::ChangeTemp_Value_OnInitialize(void) {
    const char * msg = "CW/CCW to change:%d";
    displayLCD.PrintInfoArg(msg, (int32_t)curveControl.TargetTemperature[param_choose]);
    back_value = curveControl.TargetTemperature[param_choose];
}
void StateMachine::ChangeTime_Value_OnInitialize(void) {
    const char * msg = "CW/CCW to change:%d";
    displayLCD.PrintInfoArg(msg, (int32_t)curveControl.TargetTime[param_choose]);
    back_value = curveControl.TargetTime[param_choose];
}
void StateMachine::ChangeTemp_OnInitialize(void) {
    param_choose = 1;
    const char * msg = "CW/CCW to choose:%d";
    displayLCD.PrintInfoArg(msg, param_choose);
    displayLCD.DrawAxisOnPoint(param_choose);
}
void StateMachine::ChangeTime_OnInitialize(void) {
    param_choose = 1;
    const char * msg = "CW/CCW to choose:%d";
    displayLCD.PrintInfoArg(msg, param_choose);
    displayLCD.DrawAxisOnPoint(param_choose);
}
void StateMachine::ChangeTempConfirm_OnInitialize(void) {
    const char * msg = "Hold to confirm change";
    displayLCD.PrintInformation(msg, 0);
}
void StateMachine::ChangeTimeConfirm_OnInitialize(void) {
    const char * msg = "Hold to confirm change";
    displayLCD.PrintInformation(msg, 0);
}
