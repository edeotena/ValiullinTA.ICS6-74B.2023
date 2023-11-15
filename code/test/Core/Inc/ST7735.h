#ifndef ST7735_H_
#define ST7735_H_

#include "fonts.h"
#include "st7735_cfg.h"
#include <stdbool.h>
#include <stdlib.h>

extern SPI_HandleTypeDef ST7735_SPI_PORT;

// Color definitions
#define ST7735_BLACK   0x0000
#define ST7735_BLUE    0x001F
#define ST7735_RED     0xF800
#define ST7735_GREEN   0x07E0
#define ST7735_CYAN    0x07FF
#define ST7735_MAGENTA 0xF81F
#define ST7735_YELLOW  0xFFE0
#define ST7735_WHITE   0xFFFF

void ST7735_Init(void);
void ST7735_DrawString(uint16_t x, uint16_t y, const char* str, FontDef font, uint16_t color, uint16_t bgcolor);
void ST7735_FillScreen(uint16_t color);
void ST7735_DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);

#endif /* ST7735_H_ */
