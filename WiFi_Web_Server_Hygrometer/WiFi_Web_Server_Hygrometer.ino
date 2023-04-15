#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include "pinmap.h"
#include "I2C_LCD.h"
#include "DHT21.h"


#define LED         DIO4


const char *ssid = "ssid";
const char *key = "pass";

const unsigned char symbol[8] = 
{
   0x00, 0x06, 0x09, 0x09, 0x06, 0x00, 0x00, 0x00
};

unsigned char value[5] = {0x00, 0x00, 0x00, 0x00, 0x00};
unsigned char state = 0x00;
unsigned int RH = 0x0000;
unsigned int t = 0x0000;


WiFiServer server(80);


void show_SSID_IP(void)
{
  unsigned char s = 0; 

  char temp[16];

  memset(temp, 0x00, sizeof(temp));
      
  LCD_goto(0, 0); 
  LCD_putstr("SSID:");

  LCD_goto(0, 1);
  LCD_putstr(" ");
  LCD_goto(15, 1);
  LCD_putstr(" ");
  
  sprintf(temp, "%c%c%c%c%c%c%c%c%c%c", WiFi.SSID()[0], WiFi.SSID()[1], WiFi.SSID()[2], WiFi.SSID()[3], WiFi.SSID()[4], WiFi.SSID()[5], WiFi.SSID()[6], WiFi.SSID()[7], WiFi.SSID()[8], WiFi.SSID()[9]);
  
  for(s = 0; s < 10; s++)
  {
    LCD_goto((6 + s), 0);
    LCD_putchar(temp[s]);
  }
  
  sprintf(temp, "%d.%d.%d.%d", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3]);
  
  for(s = 0; s < 16; s++)
  {
    LCD_goto((1 + s), 1);
    LCD_putchar(temp[s]);
  }

  delay(200);
}


void WiFi_details(void)
{
  LCD_clear_home(); 

  LCD_goto(1, 0); 
  LCD_putstr("WiFi connected");
 
  server.begin();
  
  LCD_goto(1, 1); 
  LCD_putstr("Server Started");
  delay(4000);

  LCD_clear_home(); 
  show_SSID_IP();
  delay(6000);

  WiFi.hostname("espfi");
  MDNS.begin("espfi"); 
}


void WiFi_setup(void)
{
  char s = 15;

  digitalWrite(LED, HIGH);
  WiFi.mode(WIFI_STA);
  delay(1000);

  WiFi.begin("","");

  digitalWrite(LED, LOW);
  LCD_clear_home();

  while(s > 0)
  {
      LCD_goto(0, 0); 
      LCD_putstr("Connecting WiFi.");

      digitalWrite(LED, HIGH);
      delay(200);
      digitalWrite(LED, LOW);
      delay(200);
       
      LCD_goto((15 - s), 1);
      LCD_putstr(".");
      s--;
  };

  
  if(WiFi.status() != WL_CONNECTED) 
  {
      digitalWrite(LED, HIGH);
      LCD_clear_home();
      LCD_goto(0, 0); 
      LCD_putstr("Attempting WPS.");
      
      WiFi.beginWPSConfig();
      digitalWrite(LED, LOW);
      delay(4000);
      
      if (WiFi.status() == WL_CONNECTED) 
      {       
        digitalWrite(LED, HIGH);
        WiFi_details(); 
      }
      else 
      {
        digitalWrite(LED, LOW);
        LCD_clear_home();
        LCD_goto(3, 0); 
        LCD_putstr("Connection");
        LCD_goto(5, 1); 
        LCD_putstr("Failed");
        delay(1000);

        LCD_clear_home();
        LCD_goto(4, 0); 
        LCD_putstr("Retrying");  
        delay(1000);    
        ESP.restart();
      }
  }
  else 
  {
    digitalWrite(LED, LOW);
    LCD_clear_home();
    LCD_goto(3, 0); 
    LCD_putstr("Connection");
    LCD_goto(2, 1); 
    LCD_putstr("Established!");
    delay(1600);
    
    WiFi_details(); 
  }
}


void setup(void) 
{
  pinMode(DIO7, OUTPUT);
  digitalWrite(DIO7, HIGH);
  DHT21_init();
  LCD_init();
  LCD_clear_home();
  lcd_symbol();
  WiFi_setup();
  LCD_clear_home();
}


void loop(void) 
{
  float rh_value = 0.0;
  float at_value = 0.0;
  
  state = get_data(value);

  switch(state)
  {
        case sensor_error_1:
        {
        }
        case sensor_error_2:
        {
           LCD_goto(0, 0); 
           LCD_putstr("No Sensor Found!");
           delay(600);
           LCD_clear_home(); 
           
           break;
        }
        case checksum_error:
        {
           LCD_goto(0, 0); 
           LCD_putstr("Checksum Error!");
           delay(600);
           LCD_clear_home(); 
           
           break;
        }
        default:
        {
           LCD_goto(0, 0); 
           LCD_putstr("R.H/ %:");
           
           RH = make_word(value[0], value[1]);
           lcd_show_value(12, 0, RH);

           rh_value = (RH / 10.0);
           
           LCD_goto(0, 1); 
           LCD_putstr("Tmp/");
           LCD_goto(4, 1); 
           LCD_send(0, DAT);
           LCD_goto(5, 1); 
           LCD_putstr("C:");
                      
           t = make_word(value[2], value[3]);
           if((value[2] & 0x80) == 1)
           {
               LCD_goto(11, 1); 
               LCD_putstr("-");
           }
           else
           {
               LCD_goto(11, 1); 
               LCD_putstr(" ");
           }

           at_value = (t / 10.0); 
            
           lcd_show_value(12, 1, t); 
          
           break;
        }
  }

  WiFiClient client = server.available();
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close"); 
  client.println("Refresh: 15"); 
  client.println();
  client.println("<!DOCTYPE html>");
  client.println("<html lang=en>");
  client.println("<head>");
  client.println("<meta charset='UTF-8'>");
  client.println("<title>ESP8266 Temperature & Humidity DHT11 Sensor</title>");
  client.println("<meta http-equiv=refresh content=15/>");
  client.println("</head>");
  client.println("<body bgcolor=black>");
  client.println("<H2 ALIGN=CENTER> <font color=green> <b> <i> WiFi Hygrometer </b> </i> </font> </H2>");
  client.println("<BR> <BR>");
  client.print("<H4 ALIGN=CENTER> <font color=green> Relative Humidity &nbsp; &nbsp; &nbsp; (%): ");
  if(state != 0)
  {
    client.print("Error!");
  }
  else
  {
    client.print(rh_value, 1); 
  }
  client.println("</font> </H4>");
  client.print("<H4 ALIGN=CENTER> <font color=green> Ambient Temperature (°C): ");
  if(state != 0)
  {
    client.print("Error!");
  }
  else
  {
    client.print(at_value, 1); 
  }
  client.println("</font> </H4>");
  client.println("<BR> <BR>");
  client.println("<H6 align=center> <font color=blue> <a href=https://www.facebook.com/MicroArena> https://www.facebook.com/MicroArena </font> </a> </H6>");
  client.println("</body>");
  client.println("</html>");

  delay(2000);
  LCD_clear_home();
  
  show_SSID_IP();
  delay(2000);

  LCD_clear_home();
}


void lcd_symbol(void)  
{
   unsigned char s = 0;  
               
   LCD_send(0x40, CMD);
    
   for(s = 0; s < 8; s++)
   {
        LCD_send(symbol[s], DAT);
   }
   
   LCD_send(0x80, CMD);
} 


unsigned int make_word(unsigned char HB, unsigned char LB)
{
  unsigned int value = 0x0000;
  
  value = HB;
  value <<= 8;
  value |= LB;
  
  return value;
}


void lcd_show_value(unsigned char x, unsigned char y, unsigned int val)
{
  unsigned int ch = 0x0000;
  
  ch = (val / 100);
  LCD_goto(x, y);
  LCD_putchar((48 + ch));
  
  ch = (val / 10);
  ch = (ch % 10);
  LCD_goto((x + 1), y);
  LCD_putchar((48 + ch));
  
  LCD_goto((x + 2), y);
  LCD_putchar(0x2E);
  
  ch = val;
  ch = (ch % 10);
  LCD_goto((x + 3), y);
  LCD_putchar((48 + ch));
}
