/**
 * @file ui.h
 * @author Michael Francis Williams (GitHub:Michael-ui)
 * @brief ui设计
 * @version 0.1
 * @date 2022-03-06
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __UI_H_
#define __UI_H_

#include "lcd.h"
#include "curve.h"
#include "stdarg.h"

struct Display {
  lcd_st7735s * lcd;

    Point<int16_t> y_end, x_end, origin;
    Point<int16_t> split_left, split_right;
    uint16_t color_axis;
    uint16_t color_split;
    uint16_t color_status;
    uint16_t color_background;
    uint16_t color_heat_line;

  void DrawTestPage(void);
  void DrawOperationPage(void);
  void DrawAboutPage(void);
  void DrawWelcomePage(void);
  void DrawParamPage(void);
  void DrawHelpPage(void);

  void DrawPointOnCurve(uint16_t time, float temp);
  void DrawAxisOnPoint(uint8_t tie);

  void ClearCurveArea(void);

  void PrintInformation(const char *fmt, int16_t out);

  void PrintInfoArg(const char *fmt, ...);
  void PrintMultiLineArg(uint8_t pos, const char *fmt, ...);
};

extern Display displayLCD;


#endif // !__UI_H_
