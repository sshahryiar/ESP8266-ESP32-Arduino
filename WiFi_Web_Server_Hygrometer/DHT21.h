#include "pinmap.h"


#define DHT21_pin                 DIO6

#define no_error                  0
#define sensor_error_1            1
#define sensor_error_2            2
#define checksum_error            3


void DHT21_init()
{
   pinMode(DHT21_pin, INPUT);
   delay(20);
}


unsigned char get_byte()
{
   unsigned char s = 0x08;
   unsigned char value = 0x00;
   
   while(s > 0)
   {
      value <<= 1;
      while(digitalRead(DHT21_pin) == LOW);
      delayMicroseconds(40);
      if(digitalRead(DHT21_pin) == HIGH)
      {
          value |= 1;
      }
      while(digitalRead(DHT21_pin) == HIGH);
      s--;
   }
   return value;
}


unsigned char get_data(unsigned char *DHT21_data)
{
   unsigned char s = 0x00;
   unsigned char check_sum = 0x00;

   pinMode(DHT21_pin, OUTPUT);
   digitalWrite(DHT21_pin, HIGH);
   digitalWrite(DHT21_pin, LOW);
   delay(1);
   digitalWrite(DHT21_pin, HIGH);
   delayMicroseconds(30);

   pinMode(DHT21_pin, INPUT);

   if(digitalRead(DHT21_pin) == HIGH)
   {
      return sensor_error_1;
   }
   delayMicroseconds(80);

   if(digitalRead(DHT21_pin) == LOW)
   {
      return sensor_error_2;
   }
   delayMicroseconds(80);

   for(s = 0; s <= 4; s++)
   {
       DHT21_data[s] = get_byte();
   }

   pinMode(DHT21_pin, OUTPUT);
   digitalWrite(DHT21_pin, HIGH);

   for(s = 0; s < 4; s++)
   {
       check_sum += DHT21_data[s];
   }

   if(check_sum != DHT21_data[4])
   {
      return checksum_error;
   }

   else
   {
      return no_error;
   }
}
