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

uint8_t get_data_flag;
uint8_t buf[1000];
uint32_t data_nums;


void Process_Main_Init(void) {
    /*TODO*/
    HAL_ADC_Start(&hadc1); 
}

void Process_Main_Run(void) {
    /*TODO*/
    uint16_t color = RGB(0x1f, 0, 0);
    LCD_Init(&hLCD);
    FontLib_Init(&fontLib);
    CurveControl_Init(&curveControl);
    HAL_TIM_Base_Start_IT(&htim9);
    hLCD.Fill(0,0,LCD_W,LCD_H,0xffff);
  for(;;)
  {
    osDelay(1);
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
    osDelay(500);
    
    hLCD.Fill(0,0,LCD_W,LCD_H,0xffff);
    displayLCD.DrawWelcomePage();
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
    // osDelay(2000);
    hLCD.Fill(0,0,LCD_W,LCD_H,0);
    displayLCD.DrawOperationPage();
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
    // osDelay(5000);




    // usb_printf("Hearing ...\r\n");
    // if (get_data_flag) {
    //   get_data_flag = 0;
    //   usb_printf("Received: %s\r\n", buf);
    // }
  }
}

void Process_Circular_Run(void) {
  static uint32_t cnt = 0;
  if (!(cnt % 1000)) {
    curveControl.OnHeatingOperation();
  }
  
  ++cnt;
}