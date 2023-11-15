#include "ST7735.h"

#define TFT_BL_H()  HAL_GPIO_WritePin(ST7735_BL_GPIO_Port, ST7735_BL_Pin, GPIO_PIN_SET)
#define TFT_BL_L()  HAL_GPIO_WritePin(ST7735_BL_GPIO_Port, ST7735_BL_Pin, GPIO_PIN_RESET)
#define TFT_CS_H()  HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_SET)
#define TFT_CS_L()  HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_RESET)
#define TFT_DC_D()  HAL_GPIO_WritePin(ST7735_DC_GPIO_Port, ST7735_DC_Pin, GPIO_PIN_SET)
#define TFT_DC_C()  HAL_GPIO_WritePin(ST7735_DC_GPIO_Port, ST7735_DC_Pin, GPIO_PIN_RESET)
#define TFT_RES_H() HAL_GPIO_WritePin(ST7735_RES_GPIO_Port, ST7735_RES_Pin, GPIO_PIN_SET)
#define TFT_RES_L() HAL_GPIO_WritePin(ST7735_RES_GPIO_Port, ST7735_RES_Pin, GPIO_PIN_RESET)

#define ST7735_COLOR565(r, g, b) (((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3))
#define SWAP_INT16_T(a, b) { int16_t t = a; a = b; b = t; }
#define DELAY 0x80

static int16_t _height = ST7735_HEIGHT, _width = ST7735_WIDTH;
static uint8_t _xstart = ST7735_XSTART, _ystart = ST7735_YSTART;

// based on Adafruit ST7735 library for Arduino
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

/*,
#if (defined(ST7735_IS_128X128) || defined(ST7735_IS_160X128))
init_cmds2[] = {            // Init for 7735R, part 2 (1.44" display)
    2,                  //  2 commands in list:
    ST7735_CASET, 4,    //  1: Column addr set, 4 args, no delay:
    0x00, 0x00,         //     XSTART = 0
    0x00, 0x7F,         //     XEND = 127
    ST7735_RASET, 4,    //  2: Row addr set, 4 args, no delay:
    0x00, 0x00,         //     XSTART = 0
    0x00, 0x7F },       //     XEND = 127
#endif // ST7735_IS_128X128

#ifdef ST7735_IS_160X80
init_cmds2[] = {          // Init for 7735S, part 2 (160x80 display)
    3,                          //  3 commands in list:
    ST7735_CASET, 4,        //  1: Column addr set, 4 args, no delay:
    0x00, 0x00,               //     XSTART = 0
    0x00, 0x4F,               //     XEND = 79
    ST7735_RASET, 4,        //  2: Row addr set, 4 args, no delay:
    0x00, 0x00,               //     XSTART = 0
    0x00, 0x9F ,              //     XEND = 159
    ST7735_INVON, 0 },        //  3: Invert colors
#endif

init_cmds3[] = {                // Init for 7735R, part 3 (red or green tab)
    4,                          //  4 commands in list:
    ST7735_GMCTRP1, 16,     //  1: Magical unicorn dust, 16 args, no delay:
    0x02, 0x1c, 0x07, 0x12,
    0x37, 0x32, 0x29, 0x2d,
    0x29, 0x25, 0x2B, 0x39,
    0x00, 0x01, 0x03, 0x10,
    ST7735_GMCTRN1, 16,     //  2: Sparkles and rainbows, 16 args, no delay:
    0x03, 0x1d, 0x07, 0x06,
    0x2E, 0x2C, 0x29, 0x2D,
    0x2E, 0x2E, 0x37, 0x3F,
    0x00, 0x00, 0x02, 0x10,
    ST7735_NORON, DELAY,    //  3: Normal display on, no args, w/delay
    10,                       //     10 ms delay
    ST7735_DISPON, DELAY,     //  4: Main screen turn on, no args w/delay
    100 };                    //     100 ms delay
*/

static void ST7735_WriteCommand(uint8_t cmd);
static void ST7735_WriteData(uint8_t* buff, size_t buff_size);
static void ST7735_ExecuteCommandList(const uint8_t *addr);
static void ST7735_SetAddressWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
static void ST7735_WriteChar(uint16_t x, uint16_t y, char ch, FontDef font, uint16_t color, uint16_t bgcolor);
static void ST7735_DrawPixel(uint16_t x, uint16_t y, uint16_t color);
static void ST7735_FillRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);

static void ST7735_Reset()
{
  TFT_RES_L();
  HAL_Delay(20);
  TFT_RES_H();
}

static void ST7735_WriteCommand(uint8_t cmd)
{
  TFT_DC_C();
  HAL_SPI_Transmit(&ST7735_SPI_PORT, &cmd, sizeof(cmd), HAL_MAX_DELAY);
}

static void ST7735_WriteData(uint8_t* buff, size_t buff_size)
{
  TFT_DC_D();
  HAL_SPI_Transmit(&ST7735_SPI_PORT, buff, buff_size, HAL_MAX_DELAY);
}

static void ST7735_ExecuteCommandList(const uint8_t *addr)
{
    uint8_t numCommands, numArgs;
    uint16_t ms;

    numCommands = *addr++;
    while(numCommands--)
    {
      uint8_t cmd = *addr++;
        ST7735_WriteCommand(cmd);

        numArgs = *addr++;
        // If high bit set, delay follows args
        ms = numArgs & DELAY;
        numArgs &= ~DELAY;
        if(numArgs)
        {
            ST7735_WriteData((uint8_t*)addr, numArgs);
            addr += numArgs;
        }

        if(ms)
        {
            ms = *addr++;
            if(ms == 255) ms = 500;
            HAL_Delay(ms);
        }
    }
}

static void ST7735_SetAddressWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{
    // column address set
    ST7735_WriteCommand(ST7735_CASET);
    uint8_t data[] = { 0x00, x0 + _xstart, 0x00, x1 + _xstart };
    ST7735_WriteData(data, sizeof(data));

    // row address set
    ST7735_WriteCommand(ST7735_RASET);
    data[1] = y0 + _ystart;
    data[3] = y1 + _ystart;
    ST7735_WriteData(data, sizeof(data));

    // write to RAM
    ST7735_WriteCommand(ST7735_RAMWR);
}

static void ST7735_WriteChar(uint16_t x, uint16_t y, char ch, FontDef font, uint16_t color, uint16_t bgcolor)
{
    uint32_t i, b, j;

    ST7735_SetAddressWindow(x, y, x+font.width-1, y+font.height-1);

    for(i = 0; i < font.height; i++)
    {
        b = font.data[(ch - 32) * font.height + i];
        for(j = 0; j < font.width; j++)
        {
            if((b << j) & 0x8000)
            {
                uint8_t data[] = { color >> 8, color & 0xFF };
                ST7735_WriteData(data, sizeof(data));
            }
            else
            {
                uint8_t data[] = { bgcolor >> 8, bgcolor & 0xFF };
                ST7735_WriteData(data, sizeof(data));
            }
        }
    }
}

void ST7735_Init()
{
	TFT_CS_L();
    ST7735_Reset();
    ST7735_ExecuteCommandList(init_cmds1);
    //ST7735_ExecuteCommandList(init_cmds2);
    //ST7735_ExecuteCommandList(init_cmds3);
    TFT_CS_H();
}

void ST7735_DrawPixel(uint16_t x, uint16_t y, uint16_t color)
{
    if((x >= _width) || (y >= _height))
        return;

    TFT_CS_L();

    ST7735_SetAddressWindow(x, y, x+1, y+1);
    uint8_t data[] = { color >> 8, color & 0xFF };
    ST7735_WriteData(data, sizeof(data));

    TFT_CS_H();
}

void ST7735_DrawString(uint16_t x, uint16_t y, const char* str, FontDef font, uint16_t color, uint16_t bgcolor)
{
	TFT_CS_L();

    while(*str)
    {
        ST7735_WriteChar(x, y, *str++, font, color, bgcolor);
        x += font.width;
    }

    TFT_CS_H();
}

void ST7735_FillRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
    if((x >= _width) || (y >= _height)) return;
    if((x + w - 1) >= _width) w = _width - x;
    if((y + h - 1) >= _height) h = _height - y;

    TFT_CS_L();
    ST7735_SetAddressWindow(x, y, x+w-1, y+h-1);

    uint8_t data[] = { color >> 8, color & 0xFF };
    TFT_DC_D();
    for(y = h; y > 0; y--)
    {
        for(x = w; x > 0; x--)
        {
          ST7735_WriteData(data, sizeof(data));
        }
    }
    TFT_CS_H();
}

void ST7735_FillScreen(uint16_t color)
{
    ST7735_FillRectangle(0, 0, _width, _height, color);
}


/***************************************************************************************
** Function name:           drawLine
** Description:             draw a line between 2 arbitrary points
***************************************************************************************/

// Slower but more compact line drawing function
void ST7735_DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
{
  int16_t steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep)
  {
    SWAP_INT16_T(x0, y0);
    SWAP_INT16_T(x1, y1);
  }

  if (x0 > x1)
  {
    SWAP_INT16_T(x0, x1);
    SWAP_INT16_T(y0, y1);
  }

  int16_t dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);

  int16_t err = dx / 2;
  int16_t ystep;

  if (y0 < y1)
  {
    ystep = 1;
  }
  else
  {
    ystep = -1;
  }

  for (; x0<=x1; x0++)
  {
    if (steep)
    {
      ST7735_DrawPixel(y0, x0, color);
    }
    else
    {
      ST7735_DrawPixel(x0, y0, color);
    }
    err -= dy;
    if (err < 0)
    {
      y0 += ystep;
      err += dx;
    }
  }
}