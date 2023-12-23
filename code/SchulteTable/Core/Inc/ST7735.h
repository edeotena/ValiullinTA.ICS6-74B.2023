#ifndef ST7735_H_
#define ST7735_H_

#include <stdbool.h>
#include <stdlib.h>

#include "main.h"

typedef struct {
    const uint8_t width;
    uint8_t height;
    const uint16_t *data;
} FontDef;

extern FontDef Font_7x10;
extern SPI_HandleTypeDef ST7735_SPI_PORT;

void ST7735_Init();
void ST7735_DrawString(uint16_t x, uint16_t y, const char* str, FontDef font);
void ST7735_FillScreen();
void ST7735_DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1);

/* Color definitions */

#define ST7735_BLACK   0x0000
#define ST7735_BLUE    0x001F
#define ST7735_RED     0xF800
#define ST7735_GREEN   0x07E0
#define ST7735_CYAN    0x07FF
#define ST7735_MAGENTA 0xF81F
#define ST7735_YELLOW  0xFFE0
#define ST7735_WHITE   0xFFFF
#define ST7735_RGB(r, g, b) (((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3))



/* ONLY CONFIG BELOW */

#define ST7735_SPI_PORT hspi1 //hspi1, hspi2, hspi3...

//Port and pin connected signal 'DC' (data or command) ST7735 display
#define ST7735_DC_Pin         GPIO_PIN_3
#define ST7735_DC_GPIO_Port   GPIOA

// WaveShare ST7735S-based 1.8" display, default orientation
#define ST7735_IS_160X128     1
#define ST7735_WIDTH          128
#define ST7735_HEIGHT         160
#define ST7735_XSTART         2
#define ST7735_YSTART         1
#define ST7735_DATA_ROTATION  0

#define ST7735_NOP     0x00
#define ST7735_SWRESET 0x01
#define ST7735_RDDID   0x04
#define ST7735_RDDST   0x09

#define ST7735_SLPIN   0x10
#define ST7735_SLPOUT  0x11
#define ST7735_PTLON   0x12
#define ST7735_NORON   0x13

#define ST7735_INVOFF  0x20
#define ST7735_INVON   0x21
#define ST7735_DISPOFF 0x28
#define ST7735_DISPON  0x29
#define ST7735_CASET   0x2A
#define ST7735_RASET   0x2B
#define ST7735_RAMWR   0x2C
#define ST7735_RAMRD   0x2E

#define ST7735_PTLAR   0x30
#define ST7735_COLMOD  0x3A
#define ST7735_MADCTL  0x36

#define ST7735_FRMCTR1 0xB1
#define ST7735_FRMCTR2 0xB2
#define ST7735_FRMCTR3 0xB3
#define ST7735_INVCTR  0xB4
#define ST7735_DISSET5 0xB6

#define ST7735_PWCTR1  0xC0
#define ST7735_PWCTR2  0xC1
#define ST7735_PWCTR3  0xC2
#define ST7735_PWCTR4  0xC3
#define ST7735_PWCTR5  0xC4
#define ST7735_VMCTR1  0xC5

#define ST7735_RDID1   0xDA
#define ST7735_RDID2   0xDB
#define ST7735_RDID3   0xDC
#define ST7735_RDID4   0xDD

#define ST7735_PWCTR6  0xFC

#define ST7735_GMCTRP1 0xE0
#define ST7735_GMCTRN1 0xE1

#define DELAY 0x80

/* Ports config */

#define TFT_DC_D()  HAL_GPIO_WritePin(ST7735_DC_GPIO_Port, ST7735_DC_Pin, GPIO_PIN_SET)
#define TFT_DC_C()  HAL_GPIO_WritePin(ST7735_DC_GPIO_Port, ST7735_DC_Pin, GPIO_PIN_RESET)

/* Init comands */

static const uint8_t
init_cmds1[] = {                     // Init for 7735R, part 1 (red or green tab)
      15,                            // 15 commands in list:
      ST7735_SWRESET, DELAY,         //  1: Software reset, 0 args, w/delay
      150,                           //     150 ms delay
      ST7735_SLPOUT, DELAY,          //  2: Out of sleep mode, 0 args, w/delay
      255,                           //     500 ms delay
      ST7735_FRMCTR1, 3,             //  3: Frame rate ctrl - normal mode, 3 args:
      0x01, 0x2C, 0x2D,              //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
      ST7735_FRMCTR2, 3,             //  4: Frame rate control - idle mode, 3 args:
      0x01, 0x2C, 0x2D,              //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
      ST7735_FRMCTR3, 6,             //  5: Frame rate ctrl - partial mode, 6 args:
      0x01, 0x2C, 0x2D,              //     Dot inversion mode
      0x01, 0x2C, 0x2D,              //     Line inversion mode
      ST7735_INVCTR, 1,              //  6: Display inversion ctrl, 1 arg, no delay:
      0x07,                          //     No inversion
      ST7735_PWCTR1, 3, 0xA2,        //  7: Power control, 3 args, no delay:
      0x02,                          //     -4.6V
      0x84,                          //     AUTO mode
      ST7735_PWCTR2, 1,              //  8: Power control, 1 arg, no delay:
      0xC5,                          //     VGH25 = 2.4C VGSEL = -10 VGH = 3 * AVDD
      ST7735_PWCTR3, 2,              //  9: Power control, 2 args, no delay:
      0x0A,                          //     Opamp current small
      0x00,                          //     Boost frequency
      ST7735_PWCTR4, 2,              // 10: Power control, 2 args, no delay:
      0x8A, 0x2A,                    //     BCLK/2, Opamp current small & Medium low
      ST7735_PWCTR5, 2, 0x8A, 0xEE,  // 11: Power control, 2 args, no delay:
      ST7735_VMCTR1, 1, 0x0E,        // 12: Power control, 1 arg, no delay:
      ST7735_INVOFF, 0,              // 13: Don't invert display, no args, no delay
      ST7735_MADCTL, 1,              // 14: Memory access control (directions), 1 arg:
      ST7735_DATA_ROTATION,          //     row addr/col addr, bottom to top refresh
      ST7735_COLMOD, 1,              // 15: set color mode, 1 arg, no delay:
      0x05 	       			         //     16-bit color
};

#endif /* ST7735_H_ */
