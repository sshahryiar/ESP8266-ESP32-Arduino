#include "font.h"
#include "ST7735.h"


static uint8_t width = ST7735_TFTWIDTH;
static uint8_t length = ST7735_TFTLENGTH;


ST7735::ST7735(int CS_pin, int RS_pin)
{
  _CS_pin = CS_pin;
  _RS_pin = RS_pin;
}


void ST7735::begin(void)
{
  pinMode(_CS_pin, OUTPUT);
  pinMode(_RS_pin, OUTPUT);

  digitalWrite(_CS_pin, HIGH);
  digitalWrite(_RS_pin, HIGH);

  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV2);
  SPI.setDataMode(SPI_MODE0);

  init();
}


void ST7735::init(void)
{  
  digitalWrite(_CS_pin, LOW);
  Write(ST7735_SWRESET, CMD);

  delay(1);
  Write(ST7735_SLPOUT, CMD);
  delay(120);

  Write(ST7735_FRMCTR1, CMD);
  Write(0x05, DAT);
  Write(0x3A, DAT);
  Write(0x3A, DAT);

  Write(ST7735_FRMCTR2, CMD);
  Write(0x05, DAT);
  Write(0x3A, DAT);
  Write(0x3A, DAT);

  Write(ST7735_FRMCTR3, CMD);
  Write(0x05, DAT);
  Write(0x3A, DAT);
  Write(0x3A, DAT);
  Write(0x05, DAT);
  Write(0x3A, DAT);
  Write(0x3A, DAT);

  Write(ST7735_INVCTR, CMD);
  Write(0x03, DAT);

  Write(ST7735_PWCTR1, CMD);
  Write(0x62, DAT);
  Write(0x02, DAT); 
  Write(0x04, DAT);

  Write(ST7735_PWCTR2, CMD);
  Write(0xC0, DAT);

  Write(ST7735_PWCTR3, CMD);
  Write(0x0D, DAT);
  Write(0x00, DAT);

  Write(ST7735_PWCTR4, CMD);
  Write(0x8D, DAT);
  Write(0x6A, DAT);

  Write(ST7735_PWCTR5, CMD);
  Write(0x8D, DAT);
  Write(0xEE, DAT);

  Write(ST7735_VMCTR1, CMD);
  Write(0x0E, DAT);

  Write(ST7735_GMCTRP1, CMD);
  Write(0x10, DAT);
  Write(0x0E, DAT);
  Write(0x02, DAT);
  Write(0x03, DAT);
  Write(0x0E, DAT);
  Write(0x07, DAT);
  Write(0x02, DAT);
  Write(0x07, DAT);
  Write(0x0A, DAT);
  Write(0x12, DAT);
  Write(0x27, DAT);
  Write(0x37, DAT);
  Write(0x00, DAT);
  Write(0x0D, DAT);
  Write(0x0E, DAT);
  Write(0x10, DAT);

  Write(ST7735_GMCTRN1, CMD);
  Write(0x10, DAT);
  Write(0x0E, DAT);
  Write(0x03, DAT);
  Write(0x03, DAT);
  Write(0x0F, DAT);
  Write(0x06, DAT);
  Write(0x02, DAT);
  Write(0x08, DAT);
  Write(0x0A, DAT);
  Write(0x13, DAT);
  Write(0x26, DAT);
  Write(0x36, DAT);
  Write(0x00, DAT);
  Write(0x0D, DAT);
  Write(0x0E, DAT);
  Write(0x10, DAT);

  Write(ST7735_COLMOD, CMD);
  Write(0x05, DAT);

  digitalWrite(_CS_pin, HIGH);

  Set_Rotation(1);

  digitalWrite(_CS_pin, LOW);
  Write(ST7735_NORON, CMD);
  delay(10);

  Write(ST7735_DISPON, CMD);
  delay(100);
  digitalWrite(_CS_pin, HIGH);
}


void ST7735::swap(int16_t *a, int16_t *b)
{
  int16_t temp = *b;
  *b = *a;
  *a = temp;
}


void ST7735::Write(uint8_t value, uint8_t mode)
{
  digitalWrite(_RS_pin, mode);
  SPI.transfer(value);
}

void ST7735::Write_Word(uint16_t value)
{
  SPI.transfer16(value);
}


void ST7735::Set_Addr_Window(int16_t xs, int16_t ys, int16_t xe, int16_t ye)
{
  digitalWrite(_CS_pin, LOW);
  xs += WINDOW_OFFSET_Xs;
  xe += WINDOW_OFFSET_Xe;
  ys += WINDOW_OFFSET_Ys;
  ye += WINDOW_OFFSET_Ye;

  Write(ST7735_CASET, CMD);
  Write(0x00, DAT);
  Write(xs, DAT);
  Write(0x00, DAT);
  Write(xe, DAT);

  Write(ST7735_RASET, CMD);
  Write(0x00, DAT);
  Write(ys, DAT);
  Write(0x00, DAT);
  Write(ye, DAT);

  Write(ST7735_RAMWR, CMD); 
  digitalWrite(_CS_pin, HIGH);
}


void ST7735::Invert_Display(uint8_t state)
{
  digitalWrite(_CS_pin, LOW);

  switch(state)
  {
    case ST7735_INVON:
    {
      Write(ST7735_INVON, CMD);
      break;
    }
    case ST7735_INVOFF:
    {
      Write(ST7735_INVOFF, CMD);
      break;
    }
  }

  digitalWrite(_CS_pin, HIGH);
}


uint16_t ST7735::Swap_Colour(uint16_t colour)
{
  return ((colour << 0x000B) | (colour & 0x07E0) | (colour >> 0x000B));
}


uint16_t ST7735::Color565(uint8_t r, uint8_t g, uint8_t b)
{
  return (((r & 0xF8) << 0x08) | ((g & 0xFC) << 0x03) | (b >> 0x03));
}


void ST7735::Set_Rotation(uint8_t rotation)
{
  digitalWrite(_CS_pin, LOW);

  Write(ST7735_MADCTL, CMD);

  switch(rotation)
  {
    case 0:
    {
      Write((ST7735_MADCTL_MX | ST7735_MADCTL_MY | ST7735_MADCTL_RGB), DAT);
      width = ST7735_TFTWIDTH;
      length = ST7735_TFTLENGTH;
      break;
    }
    case 1:
    {
      Write((ST7735_MADCTL_MY | ST7735_MADCTL_MV | ST7735_MADCTL_RGB), DAT);
      width = ST7735_TFTLENGTH;
      length = ST7735_TFTWIDTH;
      break;
    }
    case 2:
    {
      Write((ST7735_MADCTL_RGB), DAT);
      width = ST7735_TFTWIDTH;
      length = ST7735_TFTLENGTH;
      break;
    }
    case 3:
    {
      Write((ST7735_MADCTL_MX | ST7735_MADCTL_MV | ST7735_MADCTL_RGB), DAT);
      width = ST7735_TFTLENGTH;
      length = ST7735_TFTWIDTH;
      break;
    }
  };

  digitalWrite(_CS_pin, HIGH);
}


void ST7735::Fill(uint16_t colour)
{
  Fill_Area(0, 0, width, length, colour);
}


void ST7735::Fill_Area(int16_t x1 , int16_t y1, int16_t x2, int16_t y2, uint16_t colour)
{
  uint32_t index = 0;

  if(x1 > x2)
  {
    swap(&x1, &x2);
  }
  if(y1 > y2)
  {
    swap(&y1, &y2);
  }

  index = (x2 - x1);
  index *= (y2 - y1);

  Set_Addr_Window(x1, y1, (x2 - 1), (y2 - 1));
  digitalWrite(_RS_pin, DAT);
  digitalWrite(_CS_pin, LOW);

  while(index)
  {
    Write_Word(colour);
    index--;
  };

  digitalWrite(_CS_pin, HIGH);
}


void ST7735::Pixel(int16_t x_pos, int16_t y_pos, uint16_t colour)
{
  Set_Addr_Window(x_pos, y_pos, (1 + x_pos), (1 + y_pos));
  digitalWrite(_RS_pin, DAT);
  digitalWrite(_CS_pin, LOW);
  Write_Word(colour);
  digitalWrite(_CS_pin, HIGH);
}


void ST7735::Line(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t colour)
{
  int16_t dx = 0;
  int16_t dy = 0;
  int16_t stepx = 0;
  int16_t stepy = 0;
  int16_t fraction = 0;

  dy = (y2 - y1);
  dx = (x2 - x1);

  if(dy < 0)
  {
    dy = -dy;
    stepy = -1;
  }
  else
  {
    stepy = 1;
  }

  if(dx < 0)
  {
    dx = -dx;
    stepx = -1;
  }
  else
  {
    stepx = 1;
  }

  dx <<= 0x01;
  dy <<= 0x01;

  Pixel(x1, y1, colour);

  if(dx > dy)
  {
    fraction = (dy - (dx >> 1));
    while(x1 != x2)
    {
      if(fraction >= 0)
      {
        y1 += stepy;
        fraction -= dx;
      }
      x1 += stepx;
      fraction += dy;

      Pixel(x1, y1, colour);
    }
  }
  else
  {
    fraction = (dx - (dy >> 1));

    while(y1 != y2)
    {
      if(fraction >= 0)
      {
        x1 += stepx;
        fraction -= dy;
      }

      y1 += stepy;
      fraction += dx;
      Pixel(x1, y1, colour);
    }
  }
}


void ST7735::Rectangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t fill, uint8_t type, uint16_t colour, uint16_t back_colour)
{
   if(fill)
   {
      Fill_Area(x1, y1, x2, y2, colour);
   }

   else
   {
     Line(x1, y1, x2, y1, colour);
     Line(x1, y2, x2, y2, colour);
     Line(x1, y1, x1, y2, colour);
     Line(x2, y1, x2, y2, colour);
   }

   if(type != SQUARE)
   {
     Pixel(x1, y1, back_colour);
     Pixel(x1, y2, back_colour);
     Pixel(x2, y1, back_colour);
     Pixel(x2, y2, back_colour);
   }
}


void ST7735::Arc(int16_t xc, int16_t yc, int16_t radius, uint8_t fill, uint8_t circle_type, uint16_t colour)
{
  int16_t a = 0;
  int16_t b = 0;
  int16_t p = 0;

  b = radius;
  p = (1 - b);

  do
  {
    if(fill)
    {
      if((circle_type == Upper) || (circle_type == Full_Circle))
      {
        Line((xc - a), (yc - b), (xc + a), (yc - b), colour);
        Line((xc - b), (yc - a), (xc + b), (yc - a), colour); 
      }
      if((circle_type == Lower) || (circle_type == Full_Circle))
      {
        Line((xc - a), (yc + b), (xc + a), (yc + b), colour);
        Line((xc - b), (yc + a), (xc + b), (yc + a), colour);
      }
    }
    else
    {
      if((circle_type == Upper) || (circle_type == Full_Circle))
      {
        Pixel((xc + b), (yc - a), colour);
        Pixel((xc + a), (yc - b), colour);
        Pixel((xc - a), (yc - b), colour);
        Pixel((xc - b), (yc - a), colour);
      }
      if((circle_type == Lower) || (circle_type == Full_Circle))
      {
        Pixel((xc + a), (yc + b), colour);
        Pixel((xc + b), (yc + a), colour);
        Pixel((xc - a), (yc + b), colour);
        Pixel((xc - b), (yc + a), colour);
      }
    }

    if(p < 0)
    {
      p += (3 + (2 * a++));
    }
    else
    {
      p += (5 + (2 * ((a++) - (b--))));
    }
  }while(a <= b);
}


void ST7735::Circle(int16_t xc, int16_t yc, int16_t radius, uint8_t fill, uint16_t colour)
{
  Arc(xc, yc, radius, fill, Full_Circle, colour);
}


void ST7735::Point(int16_t xc, int16_t yc, int16_t size, uint16_t colour)
{
  Circle(xc, yc, size, YES, colour);
}


void ST7735::Gauge(int16_t xc, int16_t yc, int16_t radius, uint8_t fill, uint8_t U_L, uint16_t colour)
{
  Arc(xc, yc, radius, fill, U_L, colour);
}


void ST7735::Triangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t x3, int16_t y3, uint8_t fill, uint16_t colour)
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

  switch(fill)
  {
    case YES:
    {
      if(y1 > y2)
      {
        swap(&x1, &x2);
        swap(&y1, &y2);
      }

      if(y2 > y3)
      {
        swap(&x2, &x3);
        swap(&y2, &y3);
      }

      if(y1 > y2)
      {
        swap(&x1, &x2);
        swap(&y1, &y2);
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

          H_Line(a, (a + (b - (a + 1))), y1, colour);
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
          swap(&a, &b);
        }

        H_Line(a, (a + (b - (a + 1))), yp, colour);
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
          swap(&a, &b);
        }

        H_Line(a, (a + (b - (a + 1))), yp, colour);
      }

      break;
    }
    default:
    {
      Line(x1, y1, x2, y2, colour);
      Line(x2, y2, x3, y3, colour);
      Line(x3, y3, x1, y1, colour);
      break;
    }
  }
}


void ST7735::V_Line(int16_t x1, int16_t y1, int16_t y2, uint16_t colour)
{
  if(y1 > y2)
  {
    swap(&y1, &y2);
  }

  while(y2 > (y1 - 1))
  {
    Pixel(x1, y2, colour);
    y2--;
  }
}


void ST7735::H_Line(int16_t x1, int16_t x2, int16_t y1, uint16_t colour)
{
  if(x1 > x2)
  {
    swap(&x1, &x2);
  }

  while(x2 > (x1 - 1))
  {
    Pixel(x2, y1, colour);
    x2--;
  }
}


void ST7735::H_Bar(int16_t x1, int16_t x2, int16_t y1, int16_t bar_width, int16_t bar_value, uint16_t border_colour, uint16_t bar_colour, uint16_t back_colour, uint8_t border)
{
  switch(border)
  {
    case YES:
    {
      Rectangle((x1 + 1), (y1 + 1), (x1 + bar_value), (y1 + bar_width - 1), YES, SQUARE, bar_colour, back_colour);
      Rectangle((x2 - 1), (y1 + 1), (x1 + bar_value + 1), (y1 + bar_width - 1), YES, SQUARE, back_colour, back_colour);
      Rectangle(x1, y1, x2, (y1 + bar_width), NO, SQUARE, border_colour, back_colour);
      break;
    }
    default:
    {
      Rectangle(x1, y1, (x1 + bar_value), (y1 + bar_width), YES, SQUARE, bar_colour, back_colour);
      Rectangle(x2, y1, (x1 + bar_value), (y1 + bar_width), YES, SQUARE, back_colour, back_colour);
      break;
    }
  }
}


void ST7735::V_Bar(int16_t x1, int16_t y1, int16_t y2, int16_t bar_width, int16_t bar_value, uint16_t border_colour, uint16_t bar_colour, uint16_t back_colour, uint8_t border)
{
  switch(border)
  {
    case YES:
    {
      Rectangle((x1 + 1), (y2 - 1), (x1 + bar_width - 1), (y2 - bar_value), YES, SQUARE, bar_colour, back_colour);
      Rectangle((x1 + 1), (y2 - bar_value - 1), (x1 + bar_width - 1), (y1 + 1), YES, SQUARE, back_colour, back_colour);
      Rectangle(x1, y1, (x1 + bar_width), y2, NO, SQUARE, border_colour, back_colour);
      break;
    }
    default:
    {
      Rectangle(x1, y2, (x1 + bar_width), (y2 - bar_value), YES, SQUARE, bar_colour, back_colour);
      Rectangle(x1, (y2 - bar_value), (x1 + bar_width), y1, YES, SQUARE, back_colour, back_colour);
      break;
    }
  }
}


void ST7735::BMP(int16_t x_pos1, int16_t y_pos1, int16_t x_pos2, int16_t y_pos2, uint16_t *bitmap)
{
  int16_t size = 0;
  int16_t index = 0;

  if(x_pos1 > x_pos2)
  {
    swap(&x_pos1, &x_pos2);
  }

  if(y_pos1 > y_pos2)
  {
    swap(&y_pos1, &y_pos2);
  }

  size = (y_pos2 - y_pos1);
  size *= (x_pos2 - x_pos1);
  size <<= 1;

  Set_Addr_Window(x_pos1, y_pos1, (x_pos2 - 1), (y_pos2 - 1));
  digitalWrite(_RS_pin, DAT);
  digitalWrite(_CS_pin, LOW);

  for(index = 0; index < size; index++)
  {
    Write_Word(*bitmap++);
  };
  digitalWrite(_CS_pin, HIGH);
}


void ST7735::Font_Pixel(int16_t x_pos, int16_t y_pos, uint16_t colour, uint8_t pixel_size)
{
  Set_Addr_Window(x_pos, y_pos, (x_pos + pixel_size - 1), (y_pos + pixel_size - 1));
  digitalWrite(_RS_pin, DAT);
  digitalWrite(_CS_pin, LOW);

  for(uint8_t i = 0; i < (pixel_size * pixel_size); i++)
  {
    Write_Word(colour);
  }

  digitalWrite(_CS_pin, HIGH);
}


void ST7735::put_chr(int16_t x_pos, int16_t y_pos, uint8_t font_size, uint16_t colour, uint16_t back_colour, char ch)
{
   uint8_t i = 0;
   uint8_t j = 0;
   uint8_t value = 0;

   if(font_size < 1) 
   {
      font_size = 1;
   }

   if(x_pos < font_size) 
   {
      x_pos = font_size;
   }

   for(i = 0; i < 5; i++)
   {
     value = pgm_read_byte(&fonts[ch - 0x20][i]);  

     for(j = 0; j < 8; j++)
     {
       if((value >> j) & 0x01)
       {
          Font_Pixel(x_pos, y_pos, colour, font_size);
       }
       else
       {
          Font_Pixel(x_pos, y_pos, back_colour, font_size);
       }

       y_pos += font_size; 
     }

     y_pos -= (font_size << 3);  
     x_pos += font_size;  
   }

   x_pos += font_size;  

   if(x_pos > width)  
   {
     x_pos = (font_size + 1);
     y_pos += (font_size << 3);
   }
}



void ST7735::text(int16_t x_pos, int16_t y_pos, uint8_t font_size, uint16_t colour, uint16_t back_colour, const char *ch)
{
   do
   {
     put_chr(x_pos, y_pos, font_size, colour, back_colour, *ch++);
     x_pos += (font_size * 6);
   }while(*ch != '\0');
}

