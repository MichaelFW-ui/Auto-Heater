/**
 * @file lcd.h
 * @author Michael Francis Williams (GitHub:Michael-ui)
 * @brief 根据例程编写的LCD驱动。基于ST7735S
 * @version 0.1
 * @date 2022-02-23
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __LCD_H_
#define __LCD_H_

#include "stm32f4xx.h"
#include "spi.h"
#include "gpio.h"

#define MAX_TIMEOUT_MS 1000
#define USE_HORIZONTAL 2

#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
#define LCD_W 80
#define LCD_H 160

#else
#define LCD_W 160
#define LCD_H 80
#endif

extern lcd_st7735s hLCD;

struct lcd_st7735s {
    struct __gpio {
        GPIO_TypeDef *port;
        uint32_t pin;
    };

    SPI_HandleTypeDef * spi;
    __gpio CS;
    __gpio BLK;
    __gpio RES;
    __gpio DC;

    void CS_High(void) { CS.port->ODR |= CS.pin; }
    void CS_Low(void) { CS.port->ODR &= ~CS.pin; }
    void BLK_High(void) { BLK.port->ODR |= BLK.pin; }
    void BLK_Low(void) { BLK.port->ODR &= ~BLK.pin; }
    void RES_High(void) { RES.port->ODR |= RES.pin; }
    void RES_Low(void) { RES.port->ODR &= ~RES.pin; }
    void DC_High(void) { DC.port->ODR |= DC.pin; }
    void DC_Low(void) { DC.port->ODR &= ~DC.pin; }

    void Init(void);
    void WriteByte(uint8_t byte);
    void WriteHalfWord(uint16_t word);
    void WriteData_8bits(uint8_t * bytes, uint8_t len);
    void WriteCommand(uint8_t command);
    void SetAddress(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
};

void LCD_Init(lcd_st7735s *lcd);

#endif // !__LCD_H_

