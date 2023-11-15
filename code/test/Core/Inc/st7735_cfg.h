#ifndef ST7735_CFG_H_
#define ST7735_CFG_H_

#include "main.h"
#define ST7735_SPI_PORT hspi1 //hspi1, hspi2, hspi3...
#define ST7735S_1_8_DEFAULT_ORIENTATION   // WaveShare ST7735S-based 1.8" display, default orientation

//Port and pin connected signal 'RES' (reset) ST7735 display
#ifndef ST7735_RES_Pin
#define ST7735_RES_Pin      GPIO_PIN_12
#endif
#ifndef ST7735_RES_GPIO_Port
#define ST7735_RES_GPIO_Port  GPIOB
#endif
//Port and pin connected signal 'DC' (data or command) ST7735 display
#ifndef ST7735_DC_Pin
#define ST7735_DC_Pin       GPIO_PIN_13
#endif
#ifndef ST7735_DC_GPIO_Port
#define ST7735_DC_GPIO_Port   GPIOB
#endif
//Port and pin connected signal 'CS' (chip select) ST7735 display
#ifndef ST7735_CS_Pin
#define ST7735_CS_Pin       GPIO_PIN_14
#endif
#ifndef ST7735_CS_GPIO_Port
#define ST7735_CS_GPIO_Port   GPIOB
#endif
//Port and pin connected signal 'BL' (back light) ST7735 display
#ifndef ST7735_BL_Pin
#define ST7735_BL_Pin     GPIO_PIN_15
#endif
#ifndef ST7735_BL_GPIO_Port
#define ST7735_BL_GPIO_Port   GPIOB
#endif

#define ST7735_MADCTL_MY  0x80
#define ST7735_MADCTL_MX  0x40
#define ST7735_MADCTL_MV  0x20
#define ST7735_MADCTL_RGB 0x00
#define ST7735_MADCTL_BGR 0x08

// WaveShare ST7735S-based 1.8" display, default orientation
#ifdef ST7735S_1_8_DEFAULT_ORIENTATION
#define ST7735_IS_160X128     1
#define ST7735_WIDTH        128
#define ST7735_HEIGHT       160
#define ST7735_XSTART       2
#define ST7735_YSTART       1
#define ST7735_DATA_ROTATION  ST7735_MADCTL_RGB
#endif //ST7735S_1_8_DEFAULT_ORIENTATION

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

#endif /* ST7735_CFG_H_ */
