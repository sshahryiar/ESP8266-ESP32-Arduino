#include <SPI.h>
#include "font.h"
#include "pinmap.h"


#define CS_pin                                                                           DIO8
#define RS_pin                                                                           DIO2
#define RST_pin                                                                          DIO3
#define SDA_pin                                                                          DIO7
#define SCK_pin                                                                          DIO5

#define ST7735_NOP                                                                       0x00
#define ST7735_SWRESET                                                                   0x01
#define ST7735_RDDID                                                                     0x04
#define ST7735_RDDST                                                                     0x09
#define ST7735_RDDPM                                                                     0x0A
#define ST7735_RDD_MADCTL                                                                0x0B
#define STT7735_RDD_COLMOD                                                               0x0C
#define ST7735_RDDIM                                                                     0x0D
#define ST7735_RDDSM                                                                     0x0E

#define ST7735_SLPIN                                                                     0x10
#define ST7735_SLPOUT                                                                    0x11
#define ST7735_PTLON                                                                     0x12
#define ST7735_NORON                                                                     0x13

#define ST7735_INVOFF                                                                    0x20
#define ST7735_INVON                                                                     0x21
#define ST7735_GAMSET                                                                    0x26
#define ST7735_DISPOFF                                                                   0x28
#define ST7735_DISPON                                                                    0x29
#define ST7735_CASET                                                                     0x2A
#define ST7735_RASET                                                                     0x2B
#define ST7735_RAMWR                                                                     0x2C
#define ST7735_RAMRD                                                                     0x2E

#define ST7735_PTLAR                                                                     0x30
#define ST7735_TEOFF                                                                     0x34
#define ST7735_TEON                                                                      0x35
#define ST7735_MADCTL                                                                    0x36
#define ST7735_IDMOFF                                                                    0x38
#define ST7735_IDMON                                                                     0x39
#define ST7735_COLMOD                                                                    0x3A

#define ST7735_RDID1                                                                     0xDA
#define ST7735_RDID2                                                                     0xDB
#define ST7735_RDID3                                                                     0xDC
#define ST7735_RDID4                                                                     0xDD

#define ST7735_FRMCTR1                                                                   0xB1
#define ST7735_FRMCTR2                                                                   0xB2
#define ST7735_FRMCTR3                                                                   0xB3
#define ST7735_INVCTR                                                                    0xB4
#define ST7735_DISSET5                                                                   0xB6

#define ST7735_PWCTR1                                                                    0xC0
#define ST7735_PWCTR2                                                                    0xC1
#define ST7735_PWCTR3                                                                    0xC2
#define ST7735_PWCTR4                                                                    0xC3
#define ST7735_PWCTR5                                                                    0xC4
#define ST7735_VMCTR1                                                                    0xC5

#define ST7735_RDID1                                                                     0xDA
#define ST7735_RDID2                                                                     0xDB
#define ST7735_RDID3                                                                     0xDC
#define ST7735_RDID4                                                                     0xDD

#define ST7735_PWCTR6                                                                    0xFC

#define ST7735_GMCTRP1                                                                   0xE0
#define ST7735_GMCTRN1                                                                   0xE1

#define   black                                                                          0x0000
#define   blue                                                                           0x001F
#define   red                                                                            0xF800
#define   green                                                                          0x07E0
#define   cyan                                                                           0x07FF
#define   magenta                                                                        0xF81F
#define   yellow                                                                         0xFFE0
#define   white                                                                          0xFFFF

#define   MADCTL_MY                                                                      0x80
#define   MADCTL_MX                                                                      0x40
#define   MADCTL_MV                                                                      0x20
#define   MADCTL_ML                                                                      0x10
#define   MADCTL_RGB                                                                     0x08
#define   MADCTL_MH                                                                      0x04

#define ST7735_TFTWIDTH                                                                  128
#define ST7735_TFTLENGTH                                                                 160

#define CMD                                                                              0x0
#define DAT                                                                              0x1

#define SQUARE                                                                           0x00
#define ROUND                                                                            0x01

#define NO                                                                               0x00
#define YES                                                                              0x01


unsigned char w = ST7735_TFTWIDTH;
unsigned char l = ST7735_TFTLENGTH;


void swap_values(signed int *a, signed int *b)
{
	signed int temp = 0x0000;

	temp = *b;
	*b = *a;
	*a = temp;
}


unsigned int swap_colour(unsigned int colour)
{
	return ((colour << 0x000B) | (colour & 0x07E0) | (colour >> 0x000B));
}


unsigned int Color565(unsigned char r, unsigned char g, unsigned char b)
{
	return (((r & 0xF8) << 0x08) | ((g & 0xFC) << 0x03) | (b >> 0x03));
}


void ST7735_Write(unsigned char value, unsigned char RS_mode)
{
	digitalWrite(CS_pin, LOW);    
	digitalWrite(RS_pin, (RS_mode & 0x01));
	SPI.transfer(value);
	digitalWrite(CS_pin, HIGH);
}


void ST7735_Word_Write(unsigned int value)
{
	ST7735_Write(((value & 0xFF00) >> 0x08), DAT);
	ST7735_Write((value & 0x00FF), DAT);
}


void ST7735_Data_Write(unsigned char DataH, unsigned char DataL)
{
	ST7735_Write(DataH, DAT);
	ST7735_Write(DataL, DAT);
}


void ST7735_Data_Write_4k(unsigned char DataH, unsigned char DataM, unsigned char DataL)
{
	ST7735_Write(DataH, DAT);
	ST7735_Write(DataM, DAT);
	ST7735_Write(DataL, DAT);
}


void ST7735_Reset()
{
	digitalWrite(RST_pin, LOW);
	delay(2);
	digitalWrite(RST_pin, HIGH);
	delay(2);
}


void ST7735_Set_Addr_Window(unsigned char xs, unsigned char ys, unsigned char xe, unsigned char ye)
{
	ST7735_Write(ST7735_CASET, CMD);
	ST7735_Write(0x00, DAT);
	ST7735_Write(xs, DAT);
	ST7735_Write(0x00, DAT);
	ST7735_Write(xe, DAT);

	ST7735_Write(ST7735_RASET, CMD);
	ST7735_Write(0x00, DAT);
	ST7735_Write(ys, DAT);
	ST7735_Write(0x00, DAT);
	ST7735_Write(ye, DAT);

	ST7735_Write(ST7735_RAMWR, CMD);
}


void ST7735_RAM_Address_Set()
{
	ST7735_Set_Addr_Window(0x00, 0x00, 0x7F, 0x9F);
}


void Invert_Display(unsigned char i)
{
	 if(i == ST7735_INVON)
	 {
		 ST7735_Write(ST7735_INVON, CMD);
	 }
	 else
	 {
		 ST7735_Write(ST7735_INVOFF, CMD);
	 }
}


void ST7735_init()
{
	pinMode(CS_pin, OUTPUT);
	pinMode(RS_pin, OUTPUT);
	pinMode(SCK_pin, OUTPUT);
	pinMode(SDA_pin, OUTPUT);
	pinMode(RST_pin, OUTPUT);
	
	digitalWrite(CS_pin, HIGH);
	digitalWrite(RS_pin, LOW);
	digitalWrite(RST_pin, LOW);
	digitalWrite(SDA_pin, LOW);
	digitalWrite(SCK_pin, LOW);

	delay(20);

  SPI.setDataMode(SPI_MODE1);
  SPI.setClockDivider(SPI_CLOCK_DIV2);
  SPI.setBitOrder(MSBFIRST);
	SPI.begin();
  delay(20);


	ST7735_Reset();
	
	ST7735_Write(ST7735_SWRESET, CMD);
	delayMicroseconds(150);
	ST7735_Write(ST7735_SLPOUT, CMD);
	delayMicroseconds(150);

	ST7735_Write(ST7735_FRMCTR1, CMD);
	ST7735_Write(0x01, DAT);
	ST7735_Write(0x2C, DAT);
	ST7735_Write(0x2D, DAT);
	
	ST7735_Write(ST7735_FRMCTR2, CMD);
	ST7735_Write(0x01, DAT);
	ST7735_Write(0x2C, DAT);
	ST7735_Write(0x2D, DAT);
	
	ST7735_Write(ST7735_FRMCTR3, CMD);
	ST7735_Write(0x01, DAT);
	ST7735_Write(0x2C, DAT); 
	ST7735_Write(0x2D, DAT);
	ST7735_Write(0x01, DAT);
	ST7735_Write(0x2C, DAT);
	ST7735_Write(0x2D, DAT);

	ST7735_Write(ST7735_INVCTR, CMD);
	ST7735_Write(0x07, DAT);

	ST7735_Write(ST7735_PWCTR1, CMD);
	ST7735_Write(0xA2, DAT); 
	ST7735_Write(0x02, DAT); 
	ST7735_Write(0x84, DAT);

	ST7735_Write(ST7735_PWCTR1, CMD);
	ST7735_Write(0xC5, DAT);

	ST7735_Write(ST7735_PWCTR2, CMD);
	ST7735_Write(0x0A, DAT);
	ST7735_Write(0x00, DAT);

	ST7735_Write(ST7735_PWCTR3, CMD);
	ST7735_Write(0x8A, DAT);
	ST7735_Write(0x2A, DAT);

	ST7735_Write(ST7735_PWCTR4, CMD);
	ST7735_Write(0x8A, DAT);
	ST7735_Write(0xEE, DAT);

	ST7735_Write(ST7735_PWCTR5, CMD);
	ST7735_Write(0x0E, DAT);

	ST7735_Write(ST7735_VMCTR1, CMD);
	ST7735_Write(0x00, DAT);

	ST7735_Write(ST7735_COLMOD, CMD);
	ST7735_Write(0x05, DAT);

	ST7735_Write(ST7735_MADCTL, CMD);
	ST7735_Write(0xC8, DAT);
	
	ST7735_RAM_Address_Set();
	
	ST7735_Write(ST7735_GMCTRP1, CMD);
	ST7735_Write(0x02, DAT);
	ST7735_Write(0x1C, DAT);
	ST7735_Write(0x07, DAT);
	ST7735_Write(0x12, DAT);
	ST7735_Write(0x37, DAT);
	ST7735_Write(0x32, DAT);
	ST7735_Write(0x29, DAT);
	ST7735_Write(0x2D, DAT);
	ST7735_Write(0x29, DAT);
	ST7735_Write(0x25, DAT);
	ST7735_Write(0x2B, DAT);
	ST7735_Write(0x39, DAT);
	ST7735_Write(0x00, DAT);
	ST7735_Write(0x01, DAT);
	ST7735_Write(0x03, DAT);
	ST7735_Write(0x10, DAT);
	
	ST7735_Write(ST7735_GMCTRN1, CMD);
	ST7735_Write(0x03, DAT);
	ST7735_Write(0x1D, DAT);
	ST7735_Write(0x07, DAT);
	ST7735_Write(0x06, DAT);
	ST7735_Write(0x2E, DAT);
	ST7735_Write(0x2C, DAT);
	ST7735_Write(0x29, DAT);
	ST7735_Write(0x2D, DAT);
	ST7735_Write(0x2E, DAT);
	ST7735_Write(0x2E, DAT);
	ST7735_Write(0x37, DAT);
	ST7735_Write(0x3F, DAT);
	ST7735_Write(0x00, DAT);
	ST7735_Write(0x00, DAT);
	ST7735_Write(0x02, DAT);
	ST7735_Write(0x10, DAT);

	ST7735_Write(ST7735_NORON, CMD);
	delay(10);

	ST7735_Write(ST7735_DISPON, CMD);
	delay(100);
	
	ST7735_Write(ST7735_RAMWR, CMD);
	delay(100);
}


void ST7735_Set_Rotation(unsigned char m)
{
	unsigned char rotation = 0x00;

	ST7735_Write(ST7735_MADCTL, CMD);
	rotation = (m % 4);
	
	switch(rotation)
	{
		case 1:
		{
			ST7735_Write((MADCTL_MY | MADCTL_MV | MADCTL_RGB), DAT);
			w = ST7735_TFTLENGTH;
			l = ST7735_TFTWIDTH;
			break;
		}
   
		case 2:
		{
			ST7735_Write((MADCTL_RGB), DAT);
			w = ST7735_TFTWIDTH;
			l = ST7735_TFTLENGTH;
			break;
		}
   
		case 3:
		{
			ST7735_Write((MADCTL_MX | MADCTL_MV | MADCTL_RGB), DAT);
			w = ST7735_TFTLENGTH;
			l = ST7735_TFTWIDTH;
			break;
		}
   
    default:
    {
      ST7735_Write((MADCTL_MX | MADCTL_MY | MADCTL_RGB), DAT);
      w = ST7735_TFTWIDTH;
      l = ST7735_TFTLENGTH;
      break;
    }
	}
}


void TFT_fill(unsigned int colour)
{
    unsigned char i = w;
    unsigned char j = l;

    ST7735_Set_Addr_Window(0, 0, (w - 1), (l - 1));

    while(j > 0)
    {
        i = w;
        while(i > 0)
        {
            ST7735_Word_Write(colour);
            i--;
        }

        j--;
    }
}


void Draw_Pixel(unsigned char x_pos, unsigned char y_pos, unsigned int colour)
{
	ST7735_Set_Addr_Window(x_pos, y_pos, (1 + x_pos), (1 + y_pos));
	ST7735_Word_Write(colour);
}


void Draw_Line(signed int x1, signed int y1, signed int x2, signed int y2, unsigned int colour)
{
	signed int dx = 0x0000;
	signed int dy = 0x0000;
	signed int stepx = 0x0000;
	signed int stepy = 0x0000;
	signed int fraction = 0x0000;

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

	dx <<= 0x01;
	dy <<= 0x01;

	Draw_Pixel(x1, y1, colour);

	if (dx > dy)
	{
		fraction = (dy - (dx >> 1));
		while (x1 != x2)
		{
			if (fraction >= 0)
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


void Draw_V_Line(signed int x1, signed int y1, signed int y2, unsigned colour)
{
	signed int pos = 0;
	signed int temp = 0;

	if(y1 > y2)
	{
	   swap_values(&y1, &y2);
	}

	while(y2 > (y1 - 1))
	{
		Draw_Pixel(x1, y2, colour);
		y2--;
	}
}


void Draw_H_Line(signed int x1, signed int x2, signed int y1, unsigned colour)
{
	signed int pos = 0;
	signed int temp = 0;

	if(x1 > x2)
	{
	   swap_values(&x1, &x2);
	}

	while(x2 > (x1 - 1))
	{
		Draw_Pixel(x2, y1, colour);
		x2--;
	}
}


void Draw_Rectangle(signed int x1, signed int y1, signed int x2, signed int y2, unsigned char fill, unsigned char type, unsigned int colour, unsigned int back_colour)
{
	 unsigned char i = 0x00;
	 unsigned char xmin = 0x00;
	 unsigned char xmax = 0x00;
	 unsigned char ymin = 0x00;
	 unsigned char ymax = 0x00;

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
		 Draw_Pixel(x1, y1, back_colour);
		 Draw_Pixel(x1, y2, back_colour);
		 Draw_Pixel(x2, y1, back_colour);
		 Draw_Pixel(x2, y2, back_colour);
	 }
}


void Draw_Triangle(signed int x1, signed int y1, signed int x2, signed int y2, signed int x3, signed int y3, unsigned char fill, unsigned int colour)
{
	signed int a = 0;
	signed int b = 0;
	signed int sa = 0;
	signed int sb = 0;
	signed int yp = 0;
	signed int last = 0;
	signed int dx12 = 0;
	signed int dx23 = 0;
	signed int dx13 = 0;
	signed int dy12 = 0;
	signed int dy23 = 0;
	signed int dy13 = 0;

	switch(fill)
	{
		case YES:
		{
			if(y1 > y2)
			{
				swap_values(&y1, &y2);
				swap_values(&x1, &x2);
			}
			if(y2 > y3)
			{
				swap_values(&y3, &y2);
				swap_values(&x3, &x2);
			}
			if(y1 > y2)
			{
				swap_values(&y1, &y2);
				swap_values(&x1, &x2);
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
			sa = 0,
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
					swap_values(&a, &b);
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
					swap_values(&a, &b);
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


void Draw_Circle(signed int xc, signed int yc, signed int radius, unsigned char fill, unsigned int colour)
{
   signed int a = 0x0000;
   signed int b = 0x0000;
   signed int p = 0x0000;

   b = radius;
   p = (1 - b);

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

		if(p < 0)
		{
			p += (0x03 + (0x02 * a++));
		}
		else
		{
			p += (0x05 + (0x02 * ((a++) - (b--))));
		}
	}while(a <= b);
}


void Draw_Font_Pixel(unsigned char x_pos, unsigned char y_pos, unsigned int colour, unsigned char pixel_size)
{
	unsigned char i = 0x00;

	ST7735_Set_Addr_Window(x_pos, y_pos, (x_pos + pixel_size - 1), (y_pos + pixel_size - 1));

	for(i = 0x00; i < (pixel_size * pixel_size); i++)
	{
		ST7735_Word_Write(colour);
	}
}


void print_char(unsigned char x_pos, unsigned char y_pos, unsigned char font_size, unsigned int colour, unsigned int back_colour, char ch)
{
	unsigned char i = 0x00;
	unsigned char j = 0x00;

	unsigned char value = 0x00;

	if(font_size < 0)
	{
		font_size = 1;
	}

	if(x_pos < font_size)
	{
		x_pos = font_size;
	}

	for (i = 0x00; i < 0x05; i++)
	{
		for (j = 0x00; j < 0x08; j++)
		{
			value = 0x00;
			value = ((font[((unsigned char)ch) - 0x20][i]));

			if((value >> j)  & 0x01)
			{
				Draw_Font_Pixel(x_pos, y_pos, colour, font_size);
			}
			else
			{
				Draw_Font_Pixel(x_pos, y_pos, back_colour, font_size);
			}

			y_pos += font_size;
		}

		y_pos -= (font_size << 0x03);
		x_pos += font_size;

	}

	x_pos += font_size;

	if(x_pos > w)
	{
	   x_pos = (font_size + 0x01);
	   y_pos += (font_size << 0x03);
	}
}


void print_str(unsigned char x_pos, unsigned char y_pos, unsigned char font_size, unsigned int colour, unsigned int back_colour, const char *ch)
{
	do
	{
		print_char(x_pos, y_pos, font_size, colour, back_colour, *ch++);
		x_pos += (font_size * 0x06);
	}while(*ch != '\0');
}


void print_C(unsigned char x_pos, unsigned char y_pos, unsigned char font_size, unsigned int colour, unsigned int back_colour, signed int value)
{
	char ch[5] = {0x20, 0x20, 0x20, 0x20, '\0'};

	if(value < 0x00)
	{

		ch[0] = 0x2D;
		value = -value;
	}
	else
	{
		ch[0] = 0x20;
	}

	if((value > 99) && (value <= 999))
	{
		ch[1] = ((value / 100) + 0x30);
		ch[2] = (((value % 100) / 10) + 0x30);
		ch[3] = ((value % 10) + 0x30);
	}
	else if((value > 9) && (value <= 99))
	{
		ch[1] = (((value % 100) / 10) + 0x30);
		ch[2] = ((value % 10) + 0x30);
		ch[3] = 0x20;
	}
	else if((value >= 0) && (value <= 9))
	{
		ch[1] = ((value % 10) + 0x30);
		ch[2] = 0x20;
		ch[3] = 0x20;
	}

	print_str(x_pos, y_pos, font_size, colour, back_colour, ch);
}


void print_I(unsigned char x_pos, unsigned char y_pos, unsigned char font_size, unsigned int colour, unsigned int back_colour, signed int value)
{
	char ch[7] = {0x20, 0x20, 0x20, 0x20, 0x20, 0x20, '\0'};

	if(value < 0)
	{
		ch[0] = 0x2D;
		value = -value;
	}
	else
	{
		ch[0] = 0x20;
	}

	if(value > 9999)
	{
		ch[1] = ((value / 10000) + 0x30);
		ch[2] = (((value % 10000)/ 1000) + 0x30);
		ch[3] = (((value % 1000) / 100) + 0x30);
		ch[4] = (((value % 100) / 10) + 0x30);
		ch[5] = ((value % 10) + 0x30);
	}

	else if((value > 999) && (value <= 9999))
	{
		ch[1] = (((value % 10000)/ 1000) + 0x30);
		ch[2] = (((value % 1000) / 100) + 0x30);
		ch[3] = (((value % 100) / 10) + 0x30);
		ch[4] = ((value % 10) + 0x30);
		ch[5] = 0x20;
	}
	else if((value > 99) && (value <= 999))
	{
		ch[1] = (((value % 1000) / 100) + 0x30);
		ch[2] = (((value % 100) / 10) + 0x30);
		ch[3] = ((value % 10) + 0x30);
		ch[4] = 0x20;
		ch[5] = 0x20;
	}
	else if((value > 9) && (value <= 99))
	{
		ch[1] = (((value % 100) / 10) + 0x30);
		ch[2] = ((value % 10) + 0x30);
		ch[3] = 0x20;
		ch[4] = 0x20;
		ch[5] = 0x20;
	}
	else
	{
		ch[1] = ((value % 10) + 0x30);
		ch[2] = 0x20;
		ch[3] = 0x20;
		ch[4] = 0x20;
		ch[5] = 0x20;
	}

	print_str(x_pos, y_pos, font_size, colour, back_colour, ch);
}


void print_D(unsigned char x_pos, unsigned char y_pos, unsigned char font_size, unsigned int colour, unsigned int back_colour, unsigned int value, unsigned char points)
{
	char ch[6] = {0x2E, 0x20, 0x20, 0x20, 0x20, '\0'};

	ch[1] = ((value / 1000) + 0x30);

	if(points > 1)
	{
		ch[2] = (((value % 1000) / 100) + 0x30);

		if(points > 2)
		{
			ch[3] = (((value % 100) / 10) + 0x30);

			if(points > 3)
			{
				ch[4] = ((value % 10) + 0x30);
			}
		}
	}

	print_str(x_pos, y_pos, font_size, colour, back_colour, ch);
}


void print_F(unsigned char x_pos, unsigned char y_pos, unsigned char font_size, unsigned int colour, unsigned int back_colour, float value, unsigned char points)
{
	signed long tmp = 0x0000;

	tmp = value;
	print_I(x_pos, y_pos, font_size, colour, back_colour, tmp);
	tmp = ((value - tmp) * 10000);

	if(tmp < 0)
	{
	   tmp = -tmp;
	}

	if((value >= 10000) && (value < 100000))
	{
		print_D((x_pos + (0x24 * font_size)), y_pos, font_size, colour, back_colour, tmp, points);
	}
	else if((value >= 1000) && (value < 10000))
	{
		print_D((x_pos + (0x1E * font_size)), y_pos, font_size, colour, back_colour, tmp, points);
	}
	else if((value >= 100) && (value < 1000))
	{
		print_D((x_pos + (0x18 * font_size)), y_pos, font_size, colour, back_colour, tmp, points);
	}
	else if((value >= 10) && (value < 100))
	{
		print_D((x_pos + (0x12 * font_size)), y_pos, font_size, colour, back_colour, tmp, points);
	}
	else if(value < 10)
	{
		print_D((x_pos + (0x0C * font_size)), y_pos, font_size, colour, back_colour, tmp, points);

		if(value < 0)
		{
			print_char(x_pos, y_pos, font_size, colour, back_colour, 0x2D);
		}
		else
		{
			print_char(x_pos, y_pos, font_size, colour, back_colour, 0x20);
		}
	}
}
