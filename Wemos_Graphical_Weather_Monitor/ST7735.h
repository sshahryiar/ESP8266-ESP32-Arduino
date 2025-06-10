#pragma once

#include <algorithm>
#include <stdint.h>
#include <Arduino.h>
#include <esp8266_peri.h>
#include <SPI.h>


#define default_MOSI_pin                        13
#define default_CLK_pin                         14
#define default_RS_pin                          0
#define default_CS_pin                          2

#define ST7735_NOP                              0x00
#define ST7735_SWRESET                          0x01
#define ST7735_RDDID                            0x04
#define ST7735_RDDST                            0x09
#define ST7735_RDDPM                            0x0A
#define ST7735_RDD_MADCTL                       0x0B
#define STT7735_RDD_COLMOD                      0x0C
#define ST7735_RDDIM                            0x0D
#define ST7735_RDDSM                            0x0E

#define ST7735_SLPIN                            0x10
#define ST7735_SLPOUT                           0x11
#define ST7735_PTLON                            0x12
#define ST7735_NORON                            0x13

#define ST7735_INVOFF                           0x20
#define ST7735_INVON                            0x21
#define ST7735_GAMSET                           0x26
#define ST7735_DISPOFF                          0x28
#define ST7735_DISPON                           0x29
#define ST7735_CASET                            0x2A
#define ST7735_RASET                            0x2B
#define ST7735_RAMWR                            0x2C
#define ST7735_RAMRD                            0x2E

#define ST7735_PTLAR                            0x30
#define ST7735_TEOFF                            0x34
#define ST7735_TEON                             0x35
#define ST7735_MADCTL                           0x36
#define ST7735_IDMOFF                           0x38
#define ST7735_IDMON                            0x39
#define ST7735_COLMOD                           0x3A

#define ST7735_RDID1                            0xDA
#define ST7735_RDID2                            0xDB
#define ST7735_RDID3                            0xDC
#define ST7735_RDID4                            0xDD

#define ST7735_FRMCTR1                          0xB1
#define ST7735_FRMCTR2                          0xB2
#define ST7735_FRMCTR3                          0xB3
#define ST7735_INVCTR                           0xB4
#define ST7735_DISSET5                          0xB6

#define ST7735_PWCTR1                           0xC0
#define ST7735_PWCTR2                           0xC1
#define ST7735_PWCTR3                           0xC2
#define ST7735_PWCTR4                           0xC3
#define ST7735_PWCTR5                           0xC4
#define ST7735_VMCTR1                           0xC5

#define ST7735_RDID1                            0xDA
#define ST7735_RDID2                            0xDB
#define ST7735_RDID3                            0xDC
#define ST7735_RDID4                            0xDD

#define ST7735_PWCTR6                           0xFC

#define ST7735_GMCTRP1                          0xE0
#define ST7735_GMCTRN1                          0xE1

#define ST7735_MADCTL_MY                        0x80
#define ST7735_MADCTL_MX                        0x40
#define ST7735_MADCTL_MV                        0x20
#define ST7735_MADCTL_ML                        0x10
#define ST7735_MADCTL_RGB                       0x08
#define ST7735_MADCTL_MH                        0x04

#define WINDOW_OFFSET_Xs                        2
#define WINDOW_OFFSET_Xe                        2
#define WINDOW_OFFSET_Ys                        2
#define WINDOW_OFFSET_Ye                        0

#define BLACK                                   0x0000
#define BLUE                                    0x001F
#define RED                                     0xF800
#define GREEN                                   0x07E0
#define CYAN                                    0x07FF
#define MAGENTA                                 0xF81F
#define YELLOW                                  0xFFE0
#define WHITE                                   0xFFFF

#define ST7735_TFTWIDTH                         130
#define ST7735_TFTLENGTH                        130

#define CMD                                     false
#define DAT                                     true

#define Full_Circle                             0
#define Upper                                   1
#define Lower                                   2

#define SQUARE                                  false
#define ROUND                                   true

#define NO                                      false
#define YES                                     true


class ST7735
{
  public:
    ST7735(int CS_pin = default_CS_pin, int RS_pin = default_RS_pin);
    void begin(void);
    void Set_Addr_Window(int16_t xs, int16_t ys, int16_t xe, int16_t ye);
    void Invert_Display(uint8_t state);
    uint16_t Swap_Colour(uint16_t colour);
    uint16_t Color565(uint8_t r, uint8_t g, uint8_t b);
    void Set_Rotation(uint8_t rotation);
    void Fill(uint16_t colour);
    void Fill_Area(int16_t x1 , int16_t y1, int16_t x2, int16_t y2, uint16_t colour);
    void Pixel(int16_t x_pos, int16_t y_pos, uint16_t colour);
    void Line(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t colour);
    void Rectangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t fill, uint8_t type, uint16_t colour, uint16_t back_colour);
    void Circle(int16_t xc, int16_t yc, int16_t radius, uint8_t fill, uint16_t colour);
    void Point(int16_t xc, int16_t yc, int16_t size, uint16_t colour);
    void Gauge(int16_t xc, int16_t yc, int16_t radius, uint8_t fill, uint8_t U_L, uint16_t colour);
    void Triangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, uint8_t fill, uint16_t colour);
    void V_Line(int16_t x1, int16_t y1, int16_t y2, uint16_t colour);
    void H_Line(int16_t x1, int16_t x2, int16_t y1, uint16_t colour);
    void H_Bar(int16_t x1, int16_t x2, int16_t y1, int16_t bar_width, int16_t bar_value, uint16_t border_colour, uint16_t bar_colour, uint16_t back_colour, uint8_t border);
    void V_Bar(int16_t x1, int16_t y1, int16_t y2, int16_t bar_width, int16_t bar_value, uint16_t border_colour, uint16_t bar_colour, uint16_t back_colour, uint8_t border);
    void BMP(int16_t x_pos1, int16_t y_pos1, int16_t x_pos2, int16_t y_pos2, uint16_t *bitmap);
    void put_chr(int16_t x_pos, int16_t y_pos, uint8_t font_size, uint16_t colour, uint16_t back_colour, char ch);
    void text(int16_t x_pos, int16_t y_pos, uint8_t font_size, uint16_t colour, uint16_t back_colour, const char *ch);

  private:
    int _CS_pin;  
    int _RS_pin;
    int _BL_pin;
    int _RST_pin; 

    void init(void);
    void swap(int16_t *a, int16_t *b);
    void Write(uint8_t value, uint8_t mode);      
    void Write_Word(uint16_t value);
    void Arc(int16_t xc, int16_t yc, int16_t radius, uint8_t fill, uint8_t circle_type, uint16_t colour);
    void Font_Pixel(int16_t x_pos, int16_t y_pos, uint16_t colour, uint8_t pixel_size);
};