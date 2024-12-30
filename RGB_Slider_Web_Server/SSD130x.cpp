#include "SSD130x.h"


OLED::OLED()
{
}


void OLED::begin(void)
{
  Wire.begin();
  delay(100);
  write_byte((SSD1306_Set_Display_ON_or_OFF_CMD | SSD1306_Display_OFF), SSD1306_CMD);  
  write_byte(SSD1306_Set_Display_Clock_CMD, SSD1306_CMD);
  write_byte(0x80, SSD1306_CMD);
  write_byte(SSD1306_Set_Multiplex_Ratio_CMD, SSD1306_CMD);
  write_byte(0x3F, SSD1306_CMD);
  write_byte(SSD1306_Set_Display_Offset_CMD, SSD1306_CMD);
  write_byte(0x00, SSD1306_CMD);
  write_byte(SSD1306_Set_Display_Start_Line_CMD, SSD1306_CMD);
  write_byte((SSD1306_Set_Segment_Remap_CMD | SSD1306_Column_Address_0_Mapped_to_SEG127), SSD1306_CMD);
  write_byte((SSD1306_Set_COM_Output_Scan_Direction_CMD | SSD1306_Scan_from_COM63_to_0), SSD1306_CMD);
  write_byte(SSD1306_Set_Common_HW_Config_CMD, SSD1306_CMD);  
  write_byte(0x12, SSD1306_CMD);
  write_byte(SSD1306_Set_Contrast_Control_CMD, SSD1306_CMD);
  write_byte(0xFF, SSD1306_CMD);
  write_byte(SSD1306_Set_Entire_Display_ON_CMD, SSD1306_CMD);
  write_byte(SSD1306_Set_Normal_or_Inverse_Display_CMD, SSD1306_CMD);
  write_byte(SSD1306_Set_Pre_charge_Period_CMD, SSD1306_CMD);
  write_byte(0x25, SSD1306_CMD);
  write_byte(SSD1306_Set_Charge_Pump_CMD, SSD1306_CMD);
  write_byte(0x14, SSD1306_CMD);
  write_byte(SSD1306_Set_VCOMH_Level_CMD, SSD1306_CMD);
  write_byte(0x20, SSD1306_CMD);
  write_byte(SSD1306_Set_Page_Address_CMD, SSD1306_CMD);
  write_byte(0x00, SSD1306_CMD);
  write_byte(0x07, SSD1306_CMD);
  write_byte(SSD1306_Set_Page_Start_Address_CMD , SSD1306_CMD);
  write_byte(SSD1306_Set_Higher_Column_Start_Address_CMD, SSD1306_CMD);
  write_byte(SSD1306_Set_Lower_Column_Start_Address_CMD, SSD1306_CMD);
  write_byte(SSD1306_Set_Memory_Addressing_Mode_CMD, SSD1306_CMD);
  write_byte(0x02, SSD1306_CMD);  
  write_byte((SSD1306_Set_Display_ON_or_OFF_CMD | SSD1306_Display_ON), SSD1306_CMD);
}



void OLED::write_byte(uint8_t value, uint8_t control_byte)
{
  Wire.beginTransmission(SSD1306_I2C_Address); 
  Wire.write(control_byte);  
  Wire.write(value);           
  Wire.endTransmission(); 
}


void OLED::gotoxy(uint8_t x_pos, uint8_t y_pos)
{
  x_pos += 32;
  y_pos += 2;
  write_byte((SSD1306_Set_Page_Start_Address_CMD + y_pos), SSD1306_CMD);
  write_byte(((x_pos & 0x0F) | SSD1306_Set_Lower_Column_Start_Address_CMD), SSD1306_CMD);
  write_byte((((x_pos & 0xF0) >> 0x04) | SSD1306_Set_Higher_Column_Start_Address_CMD), SSD1306_CMD);
}


void OLED::fill(uint8_t bmp_data)
{
  uint8_t page = 0;
  uint8_t x_pos = 0;

  for(page = 0; page < y_max; page++)
  {
    gotoxy(x_min, page);
  
    for(x_pos = x_min; x_pos < x_max; x_pos++)
    {
      write_byte(bmp_data, SSD1306_DAT);
    };
  };
}


void OLED::clear_screen(void)
{
  fill(0x00);
}


void OLED::clear_buffer(void)
{
  uint16_t s = 0;

  for(s = 0; s < buffer_size; s++)
  {
    buffer[s] = 0x00;
  }
}


void OLED::cursor(uint8_t x_pos, uint8_t y_pos)
{
  uint8_t s = 0;

  if(y_pos != 0)
  {
    if(x_pos == 1)
    {
      gotoxy(0, (y_pos + 0x02));
    }
    else
    {
      gotoxy((0x50 + ((x_pos - 2) * 6)), (y_pos + 2));
    }

    for(s = 0; s < 6; s++)
    {
      write_byte(0xFF, SSD1306_DAT);
    }
  }
}


void OLED::print_image(const uint8_t* bmp, uint8_t pixel)
{
  uint8_t x_pos = 0;
  uint8_t page = 0;
  uint8_t itmp[1];

  if(pixel != OFF)
  {
    pixel = 0xFF;
  }
  else
  {
    pixel = 0x00;
  }

  for(page = 0; page < y_max; page++)
  {
     gotoxy(x_min, page);

     for(x_pos = x_min; x_pos < x_max; x_pos++)
     {
        itmp[0] = ((*bmp++) ^ pixel);
        write_byte(itmp[0], SSD1306_DAT);
     }
  }
}


void OLED::print_char(uint8_t x_pos, uint8_t y_pos, char ch)
{
  uint8_t s = 0;
  uint8_t chr = 0;

  chr = (ch - 0x20);

  if(x_pos > (x_max - 0x06))
  {
    x_pos = 0x00;
    y_pos++;
  }
  gotoxy(x_pos, y_pos);

  for(s = 0; s < 6; s++)
  {
    write_byte(font_regular[chr][s], SSD1306_DAT);
  }
}


void OLED::print_string(uint8_t x_pos, uint8_t y_pos, char *ch)
{
  uint8_t s = 0;

  do
  {
    print_char(x_pos, y_pos, ch[s++]);
    x_pos += 0x06;
  }while((ch[s] >= 0x20) && (ch[s] <= 0x7F));
}


void OLED::Draw_Bitmap(uint8_t xb, uint8_t yb, uint8_t xe, uint8_t ye, uint8_t *bmp_img)
{
	uint16_t s = 0;

  uint8_t x_pos = 0;
  uint8_t y_pos = 0;

  for(y_pos = yb; y_pos <= ye; y_pos++)
  {
    gotoxy(xb, y_pos);
    for(x_pos = xb; x_pos < xe; x_pos++)
    {
      write_byte(bmp_img[s], SSD1306_DAT);
      s++;
    }
  }
}


void OLED::Draw_Pixel(uint8_t x_pos, uint8_t y_pos, uint8_t colour)
{
  uint8_t value = 0;
  uint8_t page = 0;
  uint8_t bit_pos = 0;

  page = (y_pos / y_max);
  bit_pos = (y_pos - (page * y_max));
  value = buffer[((page * x_max) + x_pos)];

  if((colour & YES) != NO)
  {
    value |= (1 << bit_pos);
  }
  else
  {
    value &= (~(1 << bit_pos));
  }

  buffer[((page * x_max) + x_pos)] = value;
  gotoxy(x_pos, page);
  write_byte(value, SSD1306_DAT);
}


void OLED::Draw_Line(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t colour)
{
  int16_t dx = 0;
  int16_t dy = 0;
  int16_t stepx = 0;
  int16_t stepy = 0;
  int16_t fraction = 0;

  dy = (y2 - y1);
  dx = (x2 - x1);

  if (dy < 0)
  {
    dy = -dy;
    stepy = -1;
  }
  else
  {
    stepy = 1;
  }

  if (dx < 0)
  {
    dx = -dx;
    stepx = -1;
  }
  else
  {
    stepx = 1;
  }

  dx <<= 1;
  dy <<= 1;

  Draw_Pixel(x1, y1, colour);

  if(dx > dy)
  {
    fraction = (dy - (dx >> 1));
    while (x1 != x2)
    {
      if(fraction >= 0)
      {
        y1 += stepy;
        fraction -= dx;
      }

      x1 += stepx;
      fraction += dy;

      Draw_Pixel(x1, y1, colour);
    }
  }
  else
  {
    fraction = (dx - (dy >> 1));
    while (y1 != y2)
    {
      if (fraction >= 0)
      {
        x1 += stepx;
        fraction -= dy;
      }

      y1 += stepy;
      fraction += dx;

      Draw_Pixel(x1, y1, colour);
    }
  }
}


void OLED::Draw_V_Line(int16_t x1, int16_t y1, int16_t y2, uint8_t colour)
{
  int16_t temp = 0;

  if(y1 > y2)
  {
    temp = y1;
    y1 = y2;
    y2 = temp;
  }

  while(y2 > (y1 - 1))
  {
    Draw_Pixel(x1, y2, colour);
    y2--;
  };
}


void OLED::Draw_H_Line(int16_t x1, int16_t x2, int16_t y1, uint8_t colour)
{
  int16_t temp = 0;

  if(x1 > x2)
  {
    temp = x1;
    x1 = x2;
    x2 = temp;
  }

  while(x2 > (x1 - 1))
  {
    Draw_Pixel(x2, y1, colour);
    x2--;
  };
}


void OLED::Draw_Rectangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t fill, uint8_t colour, uint8_t type)
{
  uint8_t i = 0x00;
  uint8_t xmin = 0x00;
  uint8_t xmax = 0x00;
  uint8_t ymin = 0x00;
  uint8_t ymax = 0x00;

  if(fill != NO)
  {
   if(x1 < x2)
   {
     xmin = x1;
     xmax = x2;
   }
   else
   {
     xmin = x2;
     xmax = x1;
   }

   if(y1 < y2)
   {
     ymin = y1;
     ymax = y2;
   }
   else
   {
     ymin = y2;
     ymax = y1;
   }

   for(; xmin <= xmax; ++xmin)
   {
     for(i = ymin; i <= ymax; ++i)
     {
       Draw_Pixel(xmin, i, colour);
     }
   }
  }

  else
  {
    Draw_Line(x1, y1, x2, y1, colour);
    Draw_Line(x1, y2, x2, y2, colour);
    Draw_Line(x1, y1, x1, y2, colour);
    Draw_Line(x2, y1, x2, y2, colour);
  }

  if(type != SQUARE)
  {
    Draw_Pixel(x1, y1, ~colour);
    Draw_Pixel(x1, y2, ~colour);
    Draw_Pixel(x2, y1, ~colour);
    Draw_Pixel(x2, y2, ~colour);
  }
}


void OLED::Draw_Circle(int16_t xc, int16_t yc, int16_t radius, uint8_t fill, uint8_t colour)
{
  int16_t a = 0x0000;
  int16_t b = 0x0000;
  int16_t P = 0x0000;

  b = radius;
  P = (1 - b);

  do
  {
    if(fill != NO)
    {
      Draw_Line((xc - a), (yc + b), (xc + a), (yc + b), colour);
      Draw_Line((xc - a), (yc - b), (xc + a), (yc - b), colour);
      Draw_Line((xc - b), (yc + a), (xc + b), (yc + a), colour);
      Draw_Line((xc - b), (yc - a), (xc + b), (yc - a), colour);
    }
    else
    {
      Draw_Pixel((xc + a), (yc + b), colour);
      Draw_Pixel((xc + b), (yc + a), colour);
      Draw_Pixel((xc - a), (yc + b), colour);
      Draw_Pixel((xc - b), (yc + a), colour);
      Draw_Pixel((xc + b), (yc - a), colour);
      Draw_Pixel((xc + a), (yc - b), colour);
      Draw_Pixel((xc - a), (yc - b), colour);
      Draw_Pixel((xc - b), (yc - a), colour);
    }

    if(P < 0)
    {
      P += (3 + (2 * a++));
    }
    else
    {
      P += (5 + (2 * ((a++) - (b--))));
    }

  }while(a <= b);
}


void OLED::Draw_Triangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, uint8_t fill, int8_t colour)
{
  int16_t a = 0;
  int16_t b = 0;
  int16_t sa = 0;
  int16_t sb = 0;
  int16_t yp = 0;
  int16_t last = 0;
  int16_t dx12 = 0;
  int16_t dx23 = 0;
  int16_t dx13 = 0;
  int16_t dy12 = 0;
  int16_t dy23 = 0;
  int16_t dy13 = 0;
  int16_t temp = 0;

  switch(fill)
  {
    case YES:
    {
      if(y1 > y2)
      {
        temp = y1;
        y1 = y2;
        y2 = temp;

        temp = x1;
        x1 = x2;
        x2 = temp;
      }

      if(y2 > y3)
      {
        temp = y3;
        y3 = y2;
        y2 = temp;

        temp = x3;
        x3 = x2;
        x2 = temp;
      }

      if(y1 > y2)
      {
        temp = y1;
        y1 = y2;
        y2 = temp;

        temp = x1;
        x1 = x2;
        x2 = temp;
      }

      if(y1 == y3)
      {
        a = b = x1;

        if(x2 < a)
        {
          a = x2;
        }
        else if(x2 > b)
        {
          b = x2;
        }
        if(x2 < a)
        {
          a = x3;
        }
        else if(x3 > b)
        {
          b = x3;
        }

        Draw_H_Line(a, (a + (b - (a + 1))), y1, colour);
        return;
      }

      dx12 = (x2 - x1);
      dy12 = (y2 - y1);
      dx13 = (x3 - x1);
      dy13 = (y3 - y1);
      dx23 = (x3 - x2);
      dy23 = (y3 - y2);
      sa = 0;
      sb = 0;

      if(y2 == y3)
      {
        last = y2;
      }
      else
      {
        last = (y2 - 1);
      }

      for(yp = y1; yp <= last; yp++)
      {
        a = (x1 + (sa / dy12));
        b = (x1 + (sb / dy13));
        sa += dx12;
        sb += dx13;

        if(a > b)
        {
          temp = a;
          a = b;
          b = temp;

        }

        Draw_H_Line(a, (a + (b - (a + 1))), yp, colour);
      }

      sa = (dx23 * (yp - y2));
      sb = (dx13 * (yp - y1));

      for(; yp <= y3; yp++)
      {
        a = (x2 + (sa / dy23));
        b = (x1 + (sb / dy13));
        sa += dx23;
        sb += dx13;

        if(a > b)
        {
          temp = a;
          a = b;
          b = temp;
        }

        Draw_H_Line(a, (a + (b - (a + 1))), yp, colour);
      }

      break;
    }

    default:
    {
      Draw_Line(x1, y1, x2, y2, colour);
      Draw_Line(x2, y2, x3, y3, colour);
      Draw_Line(x3, y3, x1, y1, colour);
      break;
    }
  }
}