/**
 * @file process.cpp
 * @author Michael Francis Williams (GitHub:Michael-ui)
 * @brief 主控文件，cpp文件转发便于混合编程
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

uint8_t get_data_flag;
uint8_t buf[1000];
uint32_t data_nums;


void Process_Main_Init(void) {
    /*TODO*/
}

void Process_Main_Run(void) {
    /*TODO*/
    uint16_t color = RGB(0x1f, 0, 0);
    LCD_Init(&hLCD);
    FontLib_Init(&fontLib);
    CurveControl_Init(&curveControl);
    hLCD.Fill(0,0,LCD_W,LCD_H,0);
  for(;;)
  {
    osDelay(1);
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
    osDelay(500);
    
    displayLCD.DrawWelcomePage();
    osDelay(2000);
    displayLCD.DrawOperationPage();
    osDelay(5000);

    // usb_printf("Hearing ...\r\n");
    // if (get_data_flag) {
    //   get_data_flag = 0;
    //   usb_printf("Received: %s\r\n", buf);
    // }
  }
}