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
    hLCD.Fill(0,0,LCD_W,LCD_H,0);
  for(;;)
  {
    osDelay(1);
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
    osDelay(500);

    osDelay(500);
    hLCD.Print_String("Here I am", 20, 5, RGB(0,0x2f, 0), FontSize_2412);
    hLCD.Print_String("MichaelFW Presents", 20, 35, RGB(0,0,0x1f), FontSize_1206);
    hLCD.DrawLine(0, 0, 50, 50, RGB(0x1f, 12, 12));
    hLCD.DrawLine(10, 0, 50, 50, RGB(0x1f, 12, 12));
    hLCD.DrawLine(20, 0, 50, 50, RGB(0x1f, 12, 12));
    hLCD.DrawLine(30, 0, 50, 50, RGB(0x1f, 12, 12));
    hLCD.DrawLine(40, 0, 50, 50, RGB(0x1f, 12, 12));

    hLCD.DrawLine(80, 40,0, 0, RGB(0x1f, 12, 12));
    hLCD.DrawLine(80, 40,40, 0, RGB(0x1f, 12, 12));
    hLCD.DrawLine(80, 40,80, 0, RGB(0x1f, 12, 12));
    hLCD.DrawLine(80, 40,120, 0, RGB(0x1f, 12, 12));
    hLCD.DrawLine(80, 40,160, 0, RGB(0x1f, 12, 12));
    hLCD.DrawLine(80, 40,0, 80, RGB(0x1f, 12, 12));
    hLCD.DrawLine(80, 40,40, 80, RGB(0x1f, 12, 12));
    hLCD.DrawLine(80, 40,80, 80, RGB(0x1f, 12, 12));
    hLCD.DrawLine(80, 40,120, 80, RGB(0x1f, 12, 12));
    hLCD.DrawLine(80, 40,160, 80, RGB(0x1f, 12, 12));
    hLCD.DrawLine(80, 40,0, 40, RGB(0x1f, 12, 12));
    hLCD.DrawLine(80, 40,160, 40, RGB(0x1f, 12, 12));

    hLCD.DrawLine(30, 70, 150, 80, RGB(0, 0x2f, 0));
    hLCD.DrawLine(30, 70, 31, 10, RGB(0, 0x2f, 0));
    hLCD.DrawLine(30, 60, 60, 45, RGB(0, 0x2f, 0));
    hLCD.DrawLine(60, 45, 90, 50, RGB(0, 0x2f, 0));

    // usb_printf("Hearing ...\r\n");
    // if (get_data_flag) {
    //   get_data_flag = 0;
    //   usb_printf("Received: %s\r\n", buf);
    // }
  }
}