/**
 * @file lcd.cpp
 * @author Michael Francis Williams (GitHub:Michael-ui)
 * @brief 根据例程编写的LCD驱动。基于ST7735S. 例程由制造商提供。
 * @version 0.1
 * @date 2022-02-23
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "lcd.h"
#include "cmsis_os.h"
#include "fontlib.h"

lcd_st7735s hLCD;

void LCD_Init(lcd_st7735s *lcd) {
  lcd->spi = &hspi1;
  lcd->CS.port = IPS_CS_GPIO_Port;
  lcd->CS.pin = IPS_CS_Pin;
  lcd->BLK.port = IPS_BLK_GPIO_Port;
  lcd->BLK.pin = IPS_BLK_Pin;
  lcd->RES.port = IPS_RES_GPIO_Port;
  lcd->RES.pin = IPS_RES_Pin;
  lcd->DC.port = IPS_DC_GPIO_Port;
  lcd->DC.pin = IPS_DC_Pin;
  lcd->Init();
}

void lcd_st7735s::WriteByte(uint8_t byte) {
  CS_Low();
  HAL_SPI_Transmit(this->spi, &byte, 1, MAX_TIMEOUT_MS);
}

void lcd_st7735s::WriteHalfWord(uint16_t word) {
  CS_Low();
  uint8_t bytes[2] = {(uint8_t)(word >> 8), (uint8_t)word};
  HAL_SPI_Transmit(this->spi, bytes, 2, MAX_TIMEOUT_MS);
}

void lcd_st7735s::WriteCommand(uint8_t command) {
  CS_Low();
  DC_Low();
  HAL_SPI_Transmit(this->spi, &command, 1, MAX_TIMEOUT_MS);
  DC_High();
}

void lcd_st7735s::WriteData_8bits(uint8_t *bytes, uint8_t len) {
  CS_Low();
  HAL_SPI_Transmit(this->spi, bytes, len, MAX_TIMEOUT_MS);
}

void lcd_st7735s::Init(void) {
  RES_Low();  //复位
  osDelay(100);
  RES_High();  // 启动
  osDelay(100);

  BLK_High();  //打开背光
  osDelay(100);

  WriteCommand(0x11);  // Sleep out
  osDelay(120);     // Delay 120ms
  WriteCommand(0xB1);  // Normal mode
  WriteByte(0x05);
  WriteByte(0x3C);
  WriteByte(0x3C);
  WriteCommand(0xB2);  // Idle mode
  WriteByte(0x05);
  WriteByte(0x3C);
  WriteByte(0x3C);
  WriteCommand(0xB3);  // Partial mode
  WriteByte(0x05);
  WriteByte(0x3C);
  WriteByte(0x3C);
  WriteByte(0x05);
  WriteByte(0x3C);
  WriteByte(0x3C);
  WriteCommand(0xB4);  // Dot inversion
  WriteByte(0x03);
  WriteCommand(0xC0);  // AVDD GVDD
  WriteByte(0xAB);
  WriteByte(0x0B);
  WriteByte(0x04);
  WriteCommand(0xC1);    // VGH VGL
  WriteByte(0xC5);  // C0
  WriteCommand(0xC2);    // Normal Mode
  WriteByte(0x0D);
  WriteByte(0x00);
  WriteCommand(0xC3);  // Idle
  WriteByte(0x8D);
  WriteByte(0x6A);
  WriteCommand(0xC4);  // Partial+Full
  WriteByte(0x8D);
  WriteByte(0xEE);
  WriteCommand(0xC5);  // VCOM
  WriteByte(0x0F);
  WriteCommand(0xE0);  // positive gamma
  WriteByte(0x07);
  WriteByte(0x0E);
  WriteByte(0x08);
  WriteByte(0x07);
  WriteByte(0x10);
  WriteByte(0x07);
  WriteByte(0x02);
  WriteByte(0x07);
  WriteByte(0x09);
  WriteByte(0x0F);
  WriteByte(0x25);
  WriteByte(0x36);
  WriteByte(0x00);
  WriteByte(0x08);
  WriteByte(0x04);
  WriteByte(0x10);
  WriteCommand(0xE1);  // negative gamma
  WriteByte(0x0A);
  WriteByte(0x0D);
  WriteByte(0x08);
  WriteByte(0x07);
  WriteByte(0x0F);
  WriteByte(0x07);
  WriteByte(0x02);
  WriteByte(0x07);
  WriteByte(0x09);
  WriteByte(0x0F);
  WriteByte(0x25);
  WriteByte(0x35);
  WriteByte(0x00);
  WriteByte(0x09);
  WriteByte(0x04);
  WriteByte(0x10);

  WriteCommand(0xFC);
  WriteByte(0x80);

  WriteCommand(0x3A);
  WriteByte(0x05);
  WriteCommand(0x36);
  if (USE_HORIZONTAL == 0)
    WriteByte(0x08);
  else if (USE_HORIZONTAL == 1)
    WriteByte(0xC8);
  else if (USE_HORIZONTAL == 2)
    WriteByte(0x78);
  else
    WriteByte(0xA8);
  WriteCommand(0x21);  // Display inversion
  WriteCommand(0x29);  // Display on
  WriteCommand(0x2A);  // Set Column Address
  WriteByte(0x00);
  WriteByte(0x1A);  // 26
  WriteByte(0x00);
  WriteByte(0x69);  // 105
  WriteCommand(0x2B);    // Set Page Address
  WriteByte(0x00);
  WriteByte(0x01);  // 1
  WriteByte(0x00);
  WriteByte(0xA0);  // 160
  WriteCommand(0x2C);
  CS_High();
}

void lcd_st7735s::SetAddress(uint16_t x1, uint16_t y1, uint16_t x2,
                             uint16_t y2) {
  if (USE_HORIZONTAL == 0) {
    WriteCommand(0x2a);  //列地址设置
    WriteHalfWord(x1 + 26);
    WriteHalfWord(x2 + 26);
    WriteCommand(0x2b);  //行地址设置
    WriteHalfWord(y1 + 1);
    WriteHalfWord(y2 + 1);
    WriteCommand(0x2c);  //储存器写
  } else if (USE_HORIZONTAL == 1) {
    WriteCommand(0x2a);  //列地址设置
    WriteHalfWord(x1 + 26);
    WriteHalfWord(x2 + 26);
    WriteCommand(0x2b);  //行地址设置
    WriteHalfWord(y1 + 1);
    WriteHalfWord(y2 + 1);
    WriteCommand(0x2c);  //储存器写
  } else if (USE_HORIZONTAL == 2) {
    WriteCommand(0x2a);  //列地址设置
    WriteHalfWord(x1 + 1);
    WriteHalfWord(x2 + 1);
    WriteCommand(0x2b);  //行地址设置
    WriteHalfWord(y1 + 26);
    WriteHalfWord(y2 + 26);
    WriteCommand(0x2c);  //储存器写
  } else {
    WriteCommand(0x2a);  //列地址设置
    WriteHalfWord(x1 + 1);
    WriteHalfWord(x2 + 1);
    WriteCommand(0x2b);  //行地址设置
    WriteHalfWord(y1 + 26);
    WriteHalfWord(y2 + 26);
    WriteCommand(0x2c);  //储存器写
  }
  // CS_High();
}

void lcd_st7735s::Fill(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                       uint16_t color) {
  SetAddress(x1, y1, x2, y2);
  for (auto i = y1; i <= y2; ++i) {
    for (auto j = x1; j <= x2; ++j) {
      WriteHalfWord(color);
    }
  }
  CS_High();
}

void lcd_st7735s::PrintASCII(uint8_t ch, uint16_t x, uint16_t y, uint16_t color,
                             uint16_t bg_color, FontSize size) {
  FontLib_CharInfo_t ch_t = fontLib.GetFont_ASCII(ch, size);
  if (!ch_t.font) return;
  SetAddress(x, y, x + ch_t.width-1, y + ch_t.height-1);
  uint8_t LocalSize = (ch_t.width - 1) / 8 + 1;
  for (int i = 0; i < ch_t.height; ++i) {
    for (int j = 0; j < ch_t.width; ++j) {
      if ((ch_t.font)[i * LocalSize + j / 8] & (1 << (j % 8))) WriteHalfWord(color);
      else WriteHalfWord(bg_color);
    }
  }
}