/**
 * @file fontlib.h
 * @author Michael Francis Williams (GitHub:Michael-ui)
 * @brief 
 * @version 0.1
 * @date 2022-02-25
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __FONTLIB_H_
#define __FONTLIB_H_

#include "stm32f4xx.h"

struct FontLib_CharInfo_t {
    const uint8_t *font;
    uint16_t width, height;
};

enum FontSize { FontSize_1206, FontSize_1608, FontSize_2412, FontSize_3216 };

struct FontLib {
    SPI_HandleTypeDef * spi;
    uint8_t temp[64];

    enum FontLibLocation { FontLib_STM32, FontLib_Flash } from;
    FontLib_CharInfo_t GetFont_ASCII(uint8_t ch, FontSize size);
    FontLib_CharInfo_t GetFont_ASCII_STM32(uint8_t ch, FontSize size);
    FontLib_CharInfo_t GetFont_ASCII_Flash(uint8_t ch, FontSize size);
};

extern FontLib fontLib;

void FontLib_Init(FontLib *fl);


#endif // !__FONTLIB_H_