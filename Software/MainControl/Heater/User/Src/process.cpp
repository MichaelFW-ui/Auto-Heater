/**
 * @file process.cpp
 * @author Michael Francis Williams (GitHub:Michael-ui)
 * @brief 主控文件，cpp文件转发便于混合编程
 * !!!
 *        这是固件第一版本。本版本的主要特点是：内容高度耦合，不利于二次移植；
 *                                      没有充分利用RTOS的优势，代码规范低等。
 *        今后将升级第二版本。
 * !!!
 * @version 0.1
 * @date 2022-02-23
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "stm32f4xx.h"
#include "process.h"
#include "cmsis_os.h"
#include "usbd_cdc_if.h"
#include "lcd.h"
#include "fontlib.h"
#include "ctrl.h"
#include "adc.h"
#include "statemachine.h"
#include "ui.h"
#include "temp.h"

uint8_t get_data_flag;
uint8_t buf[1000];
uint32_t data_nums;

volatile uint32_t TIM_OVERFLOW = 0;

uint8_t flag_spin_cw = 0;
uint8_t flag_spin_ccw = 0;
uint8_t flag_sw_short = 0;
uint8_t flag_sw_hold = 0;
uint8_t flag_error = 0;

StateMachine sm;

extern uint32_t ADC_Converted[2];

void Process_Main_Init(void) {
  /*TODO*/
  HAL_ADC_Start_DMA(&hadc1, (uint32_t *)ADC_Converted,  2);
  LCD_Init(&hLCD);
  FontLib_Init(&fontLib);
  CurveControl_Init(&curveControl);
  HAL_TIM_Base_Start_IT(&htim9);
}

void Process_Main_Run(void) {
  /*TODO*/
  uint16_t color = RGB(0x1f, 0, 0);
  hLCD.Fill(0, 0, LCD_W, LCD_H, 0xffff);
  displayLCD.DrawWelcomePage();
  osDelay(1000);
  displayLCD.DrawOperationPage();
  for(;;)
  {
    osDelay(10);
		// usb_printf("Test\r\n");
    uint16_t raw1 = ADC_Converted[0];
    uint16_t raw2 = ADC_Converted[1];
    float VCC = 3.2;
    float v1 = (float) raw1 * VCC / 4096;
    float v2 = (float) raw2 * VCC / 4096;

    // float temp = GetTemperature();
    // displayLCD.PrintInfoArg("T: %d,%d,%f",ADC_Converted[0], ADC_Converted[1], temp);
    // continue;

    if (flag_spin_ccw) {
      flag_spin_ccw = 0;
      StateMachine::State ret = sm.OnEvent(Event::Spin_CCW);
      sm.state = ret;
    }

    if (flag_spin_cw) {
      flag_spin_cw = 0;
      StateMachine::State ret = sm.OnEvent(Event::Spin_CW);
      sm.state = ret;
    }

    if (flag_sw_hold) {
      flag_sw_hold = 0;
      StateMachine::State ret = sm.OnEvent(Event::SW_Hold);
      sm.state = ret;
    }

    if (flag_sw_short) {
      flag_sw_short = 0;
      StateMachine::State ret = sm.OnEvent(Event::SW_Short);
      sm.state = ret;
    }

    if (flag_error) {
      flag_error = 0;
      displayLCD.PrintInformation("Error", 0);
    }
  }


    // HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
    // osDelay(500);
    
    // hLCD.Fill(0,0,LCD_W,LCD_H,0xffff);
    // displayLCD.DrawWelcomePage();
    // HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
    // osDelay(2000);
    // hLCD.Fill(0,0,LCD_W,LCD_H,0);
    // displayLCD.DrawOperationPage();
    // HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
    // osDelay(5000);
    // for (int i = 100; i < 1000; i += 100) {
    //   displayLCD.PrintInformation("Test:printing", i);
    //   osDelay(500);
    //   displayLCD.PrintInfoArg("%d times now", i);
    //   osDelay(500);
    // }




    // usb_printf("Hearing ...\r\n");
    // if (get_data_flag) {
    //   get_data_flag = 0;
    //   usb_printf("Received: %s\r\n", buf);
    // }
}

// 1ms
void Process_Circular_Run(void) {
  static uint32_t cnt = 0;
  if (!(cnt % 1000)) {
    curveControl.OnHeatingOperation();
  }
  
  ++cnt;
  ++TIM_OVERFLOW;
}
