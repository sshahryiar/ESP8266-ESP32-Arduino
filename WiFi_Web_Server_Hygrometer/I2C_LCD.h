#include <Wire.h>

#define PCF8574_address                         0x27

#define clear_display                           0x01
#define goto_home                               0x02

#define cursor_direction_inc                    (0x04 | 0x02)
#define cursor_direction_dec                    (0x04 | 0x00)
#define display_shift                           (0x04 | 0x01)
#define display_no_shift                        (0x04 | 0x00)

#define display_on                              (0x08 | 0x04)
#define display_off                             (0x08 | 0x02)
#define cursor_on                               (0x08 | 0x02)
#define cursor_off                              (0x08 | 0x00)
#define blink_on                                (0x08 | 0x01)
#define blink_off                               (0x08 | 0x00)

#define _8_pin_interface                        (0x20 | 0x10)
#define _4_pin_interface                        (0x20 | 0x00)
#define _2_row_display                          (0x20 | 0x08)
#define _1_row_display                          (0x20 | 0x00)
#define _5x10_dots                              (0x20 | 0x40)
#define _5x7_dots                               (0x20 | 0x00)

#define dly                                     2

#define CMD                                     0
#define DAT                                     1

#define BL_ON                                   1
#define BL_OFF                                  0

unsigned char bl_state;
unsigned char data_value;

void PCF8574_init(void)
{
     Wire.begin();
     delay(100);
}

void PCF8574_write(unsigned char data_byte)
{
    Wire.beginTransmission(PCF8574_address);
    Wire.write(data_byte);   
    Wire.endTransmission();
}

void LCD_4bit_send(unsigned char lcd_data)
{
    unsigned char temp = 0x00;

    temp = (lcd_data & 0xF0);
    data_value &= 0x0F;
    data_value |= temp;
    PCF8574_write(data_value);

    data_value |= 0x04;
    PCF8574_write(data_value);
    delay(dly);
    data_value &= 0xF9;
    PCF8574_write(data_value);
    delay(dly);

    temp = (lcd_data & 0x0F);
    temp <<= 0x04;
    data_value &= 0x0F;
    data_value |= temp;
    PCF8574_write(data_value);

    data_value |= 0x04;
    PCF8574_write(data_value);
    delay(dly);
    data_value &= 0xF9;
    PCF8574_write(data_value);
    delay(dly);
}

void LCD_send(unsigned char value, unsigned char control_type)
{
    switch(control_type)
    {
       case CMD:
       {
            data_value &= 0xF4;
            break;
       }
       case DAT:
       {
           data_value |= 0x01;
           break;
       }
    }
    
    switch(bl_state)
    {
       case BL_ON:
       {
          data_value |= 0x08;
          break;
       }
       case BL_OFF:
       {
          data_value &= 0xF7;
          break;
       }
    }
    
    PCF8574_write(data_value);
    LCD_4bit_send(value);
    delay(10);
}

void LCD_init()
{
    PCF8574_init();

    bl_state = BL_ON;
    data_value = 0x04;
    PCF8574_write(data_value);
    delay(10);

    data_value = 0x30;
    PCF8574_write(data_value);

    data_value |= 0x04;
    PCF8574_write(data_value);
    delay(dly);
    data_value &= 0xF1;
    PCF8574_write(data_value);
    delay(dly);

    data_value = 0x30;
    PCF8574_write(data_value);

    data_value |= 0x04;
    PCF8574_write(data_value);
    delay(dly);
    data_value &= 0xF1;
    PCF8574_write(data_value);
    delay(dly);

    data_value = 0x30;
    PCF8574_write(data_value);

    data_value |= 0x04;
    PCF8574_write(data_value);
    delay(dly);
    data_value &= 0xF1;
    PCF8574_write(data_value);
    delay(dly);

    data_value = 0x20;
    PCF8574_write(data_value);

    data_value |= 0x04;
    PCF8574_write(data_value);
    delay(dly);
    data_value &= 0xF1;
    PCF8574_write(data_value);
    delay(dly);

    LCD_send((_4_pin_interface | _2_row_display | _5x7_dots), CMD);
    LCD_send((display_on | cursor_off | blink_off | display_no_shift), CMD);
    LCD_send((clear_display), CMD);
    LCD_send((cursor_direction_inc | display_no_shift), CMD);
}

void LCD_putstr(char *lcd_string)
{
    while(*lcd_string != '\0')
    {
        LCD_send(*lcd_string++, DAT);
    };
}

void LCD_putchar(char char_data)
{
    if((char_data >= 0x20) && (char_data <= 0x7F))
    {
       LCD_send(char_data, DAT);
    }
}

void LCD_clear_home(void)
{
    LCD_send(clear_display, CMD);
    LCD_send(goto_home, CMD);
}

void LCD_goto(unsigned char x_pos,unsigned char y_pos)
{
   if(y_pos == 0)
    {
        LCD_send((0x80 | x_pos), CMD);
    }
    else
    {
        LCD_send((0x80 | 0x40 | x_pos), CMD);
    }
}
