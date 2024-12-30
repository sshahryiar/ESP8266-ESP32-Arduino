#pragma once


#include <Arduino.h>
#include <Wire.h>


#define SSD1306_I2C_Address                               0x3C

#define SSD1306_Set_Lower_Column_Start_Address_CMD        0x00
#define SSD1306_Set_Higher_Column_Start_Address_CMD       0x10
#define SSD1306_Set_Memory_Addressing_Mode_CMD            0x20
#define SSD1306_Set_Column_Address_CMD                    0x21
#define SSD1306_Set_Page_Address_CMD                      0x22
#define SSD1306_Set_Display_Start_Line_CMD                0x40
#define SSD1306_Set_Contrast_Control_CMD                  0x81
#define SSD1306_Set_Charge_Pump_CMD                       0x8D
#define SSD1306_Set_Segment_Remap_CMD                     0xA0
#define SSD1306_Set_Entire_Display_ON_CMD                 0xA4
#define SSD1306_Set_Normal_or_Inverse_Display_CMD         0xA6
#define SSD1306_Set_Multiplex_Ratio_CMD                   0xA8
#define SSD1306_Set_Display_ON_or_OFF_CMD                 0xAE
#define SSD1306_Set_Page_Start_Address_CMD                0xB0
#define SSD1306_Set_COM_Output_Scan_Direction_CMD         0xC0
#define SSD1306_Set_Display_Offset_CMD                    0xD3
#define SSD1306_Set_Display_Clock_CMD                     0xD5
#define SSD1306_Set_Pre_charge_Period_CMD                 0xD9
#define SSD1306_Set_Common_HW_Config_CMD                  0xDA
#define SSD1306_Set_VCOMH_Level_CMD                       0xDB
#define SSD1306_Set_NOP_CMD                               0xE3

#define SSD1306_Horizontal_Addressing_Mode                0x00
#define SSD1306_Vertical_Addressing_Mode                  0x01
#define SSD1306_Page_Addressing_Mode                      0x02

#define SSD1306_Disable_Charge_Pump                       0x00
#define SSD1306_Enable_Charge_Pump                        0x04

#define SSD1306_Column_Address_0_Mapped_to_SEG0           0x00
#define SSD1306_Column_Address_0_Mapped_to_SEG127         0x01

#define SSD1306_Normal_Display                            0x00
#define SSD1306_Entire_Display_ON                         0x01

#define SSD1306_Non_Inverted_Display                      0x00
#define SSD1306_Inverted_Display                          0x01

#define SSD1306_Display_OFF                               0x00
#define SSD1306_Display_ON                                0x01

#define SSD1306_Scan_from_COM0_to_63                      0x00
#define SSD1306_Scan_from_COM63_to_0                      0x08

#define SSD1306_DAT                                       0x60
#define SSD1306_CMD                                       0x00

#define x_size                                            128
#define x_max                                             x_size
#define x_min                                             0
#define y_size                                            64
#define y_max                                             8
#define y_min                                             0

#define ON                                                1
#define OFF                                               0

#define YES                                               1
#define NO                                                0

#define ROUND                                             1
#define SQUARE                                            0

#define buffer_size                                       1024//(x_max * y_max)


class OLED
{
  public:
    OLED();
    void begin(void);
    void gotoxy(uint8_t x_pos, uint8_t y_pos);
    void fill(uint8_t bmp_data);
    void clear_screen(void);
    void clear_buffer(void);
    void cursor(uint8_t x_pos, uint8_t y_pos);
    void print_image(const uint8_t* bmp, uint8_t pixel);
    void print_char(uint8_t x_pos, uint8_t y_pos, char ch);
    void print_string(uint8_t x_pos, uint8_t y_pos, char *ch);
    void Draw_Bitmap(uint8_t xb, uint8_t yb, uint8_t xe, uint8_t ye, uint8_t *bmp_img);
    void Draw_Pixel(uint8_t x_pos, uint8_t y_pos, uint8_t colour);
    void Draw_Line(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t colour);
    void Draw_V_Line(int16_t x1, int16_t y1, int16_t y2, uint8_t colour);
    void Draw_H_Line(int16_t x1, int16_t x2, int16_t y1, uint8_t colour);
    void Draw_Rectangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t fill, uint8_t colour, uint8_t type);
    void Draw_Circle(int16_t xc, int16_t yc, int16_t radius, uint8_t fill, uint8_t colour);
    void Draw_Triangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, uint8_t fill, int8_t colour);

    uint8_t buffer[buffer_size];

  private:
    const uint8_t font_regular[92][6] =
    {
      {0x00, 0x00, 0x00, 0x00, 0x00, 0x00},   // sp
      {0x00, 0x00, 0x00, 0x2f, 0x00, 0x00},   // !
      {0x00, 0x00, 0x07, 0x00, 0x07, 0x00},   // "
      {0x00, 0x14, 0x7f, 0x14, 0x7f, 0x14},   // #
      {0x00, 0x24, 0x2a, 0x7f, 0x2a, 0x12},   // $
      {0x00, 0x62, 0x64, 0x08, 0x13, 0x23},   // %
      {0x00, 0x36, 0x49, 0x55, 0x22, 0x50},   // &
      {0x00, 0x00, 0x05, 0x03, 0x00, 0x00},   // '
      {0x00, 0x00, 0x1c, 0x22, 0x41, 0x00},   // (
      {0x00, 0x00, 0x41, 0x22, 0x1c, 0x00},   // )
      {0x00, 0x14, 0x08, 0x3E, 0x08, 0x14},   // *
      {0x00, 0x08, 0x08, 0x3E, 0x08, 0x08},   // +
      {0x00, 0x00, 0x00, 0xA0, 0x60, 0x00},   // ,
      {0x00, 0x08, 0x08, 0x08, 0x08, 0x08},   // -
      {0x00, 0x00, 0x60, 0x60, 0x00, 0x00},   // .
      {0x00, 0x20, 0x10, 0x08, 0x04, 0x02},   // /
      {0x00, 0x3E, 0x51, 0x49, 0x45, 0x3E},   // 0
      {0x00, 0x00, 0x42, 0x7F, 0x40, 0x00},   // 1
      {0x00, 0x42, 0x61, 0x51, 0x49, 0x46},   // 2
      {0x00, 0x21, 0x41, 0x45, 0x4B, 0x31},   // 3
      {0x00, 0x18, 0x14, 0x12, 0x7F, 0x10},   // 4
      {0x00, 0x27, 0x45, 0x45, 0x45, 0x39},   // 5
      {0x00, 0x3C, 0x4A, 0x49, 0x49, 0x30},   // 6
      {0x00, 0x01, 0x71, 0x09, 0x05, 0x03},   // 7
      {0x00, 0x36, 0x49, 0x49, 0x49, 0x36},   // 8
      {0x00, 0x06, 0x49, 0x49, 0x29, 0x1E},   // 9
      {0x00, 0x00, 0x36, 0x36, 0x00, 0x00},   // :
      {0x00, 0x00, 0x56, 0x36, 0x00, 0x00},   // ;
      {0x00, 0x08, 0x14, 0x22, 0x41, 0x00},   // <
      {0x00, 0x14, 0x14, 0x14, 0x14, 0x14},   // =
      {0x00, 0x00, 0x41, 0x22, 0x14, 0x08},   // >
      {0x00, 0x02, 0x01, 0x51, 0x09, 0x06},   // ?
      {0x00, 0x32, 0x49, 0x59, 0x51, 0x3E},   // @
      {0x00, 0x7C, 0x12, 0x11, 0x12, 0x7C},   // A
      {0x00, 0x7F, 0x49, 0x49, 0x49, 0x36},   // B
      {0x00, 0x3E, 0x41, 0x41, 0x41, 0x22},   // C
      {0x00, 0x7F, 0x41, 0x41, 0x22, 0x1C},   // D
      {0x00, 0x7F, 0x49, 0x49, 0x49, 0x41},   // E
      {0x00, 0x7F, 0x09, 0x09, 0x09, 0x01},   // F
      {0x00, 0x3E, 0x41, 0x49, 0x49, 0x7A},   // G
      {0x00, 0x7F, 0x08, 0x08, 0x08, 0x7F},   // H
      {0x00, 0x00, 0x41, 0x7F, 0x41, 0x00},   // I
      {0x00, 0x20, 0x40, 0x41, 0x3F, 0x01},   // J
      {0x00, 0x7F, 0x08, 0x14, 0x22, 0x41},   // K
      {0x00, 0x7F, 0x40, 0x40, 0x40, 0x40},   // L
      {0x00, 0x7F, 0x02, 0x0C, 0x02, 0x7F},   // M
      {0x00, 0x7F, 0x04, 0x08, 0x10, 0x7F},   // N
      {0x00, 0x3E, 0x41, 0x41, 0x41, 0x3E},   // O
      {0x00, 0x7F, 0x09, 0x09, 0x09, 0x06},   // P
      {0x00, 0x3E, 0x41, 0x51, 0x21, 0x5E},   // Q
      {0x00, 0x7F, 0x09, 0x19, 0x29, 0x46},   // R
      {0x00, 0x46, 0x49, 0x49, 0x49, 0x31},   // S
      {0x00, 0x01, 0x01, 0x7F, 0x01, 0x01},   // T
      {0x00, 0x3F, 0x40, 0x40, 0x40, 0x3F},   // U
      {0x00, 0x1F, 0x20, 0x40, 0x20, 0x1F},   // V
      {0x00, 0x3F, 0x40, 0x38, 0x40, 0x3F},   // W
      {0x00, 0x63, 0x14, 0x08, 0x14, 0x63},   // X
      {0x00, 0x07, 0x08, 0x70, 0x08, 0x07},   // Y
      {0x00, 0x61, 0x51, 0x49, 0x45, 0x43},   // Z
      {0x00, 0x00, 0x7F, 0x41, 0x41, 0x00},   // [
      {0x00, 0x02, 0x04, 0x08, 0x10, 0x20},   //
      {0x00, 0x00, 0x41, 0x41, 0x7F, 0x00},   // ]
      {0x00, 0x04, 0x02, 0x01, 0x02, 0x04},   // ^
      {0x00, 0x40, 0x40, 0x40, 0x40, 0x40},   // _
      {0x00, 0x00, 0x01, 0x02, 0x04, 0x00},   // '
      {0x00, 0x20, 0x54, 0x54, 0x54, 0x78},   // a
      {0x00, 0x7F, 0x48, 0x44, 0x44, 0x38},   // b
      {0x00, 0x38, 0x44, 0x44, 0x44, 0x20},   // c
      {0x00, 0x38, 0x44, 0x44, 0x48, 0x7F},   // d
      {0x00, 0x38, 0x54, 0x54, 0x54, 0x18},   // e
      {0x00, 0x08, 0x7E, 0x09, 0x01, 0x02},   // f
      {0x00, 0x18, 0xA4, 0xA4, 0xA4, 0x7C},   // g
      {0x00, 0x7F, 0x08, 0x04, 0x04, 0x78},   // h
      {0x00, 0x00, 0x44, 0x7D, 0x40, 0x00},   // i
      {0x00, 0x40, 0x80, 0x84, 0x7D, 0x00},   // j
      {0x00, 0x7F, 0x10, 0x28, 0x44, 0x00},   // k
      {0x00, 0x00, 0x41, 0x7F, 0x40, 0x00},   // l
      {0x00, 0x7C, 0x04, 0x18, 0x04, 0x78},   // m
      {0x00, 0x7C, 0x08, 0x04, 0x04, 0x78},   // n
      {0x00, 0x38, 0x44, 0x44, 0x44, 0x38},   // o
      {0x00, 0xFC, 0x24, 0x24, 0x24, 0x18},   // p
      {0x00, 0x18, 0x24, 0x24, 0x18, 0xFC},   // q
      {0x00, 0x7C, 0x08, 0x04, 0x04, 0x08},   // r
      {0x00, 0x48, 0x54, 0x54, 0x54, 0x20},   // s
      {0x00, 0x04, 0x3F, 0x44, 0x40, 0x20},   // t
      {0x00, 0x3C, 0x40, 0x40, 0x20, 0x7C},   // u
      {0x00, 0x1C, 0x20, 0x40, 0x20, 0x1C},   // v
      {0x00, 0x3C, 0x40, 0x30, 0x40, 0x3C},   // w
      {0x00, 0x44, 0x28, 0x10, 0x28, 0x44},   // x
      {0x00, 0x1C, 0xA0, 0xA0, 0xA0, 0x7C},   // y
      {0x00, 0x44, 0x64, 0x54, 0x4C, 0x44},   // z
      {0x14, 0x14, 0x14, 0x14, 0x14, 0x14}    // horizontal lines
    };

    void write_byte(uint8_t value, uint8_t control_byte);
};