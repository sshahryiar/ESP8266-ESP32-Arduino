#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include "page.h"
#include "pinmap.h"
#include "I2C_LCD.h"


#define RLY1        DIO3
#define RLY2        DIO5
#define RLY3        DIO6
#define RLY4        DIO7

#define LED         DIO4


String req;

unsigned char states = 0;

unsigned int time_count = 0;


WiFiServer server(80);


void SSID_IP_show(void)
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
  SSID_IP_show();
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

   
void setup() 
{
  pinMode(LED, OUTPUT);
  pinMode(RLY1, OUTPUT);
  pinMode(RLY2, OUTPUT);
  pinMode(RLY3, OUTPUT);
  pinMode(RLY4, OUTPUT);
   
  digitalWrite(LED, LOW);
  digitalWrite(RLY1, LOW);
  digitalWrite(RLY2, LOW);
  digitalWrite(RLY3, LOW);
  digitalWrite(RLY4, LOW);
  
  LCD_init();
  LCD_clear_home();

  WiFi_setup();
}

void loop() 
{
  WiFiClient client = server.available();
  
  if(client == 0) 
  { 
     return;
  }
  
  while(!client.available())
  {
    digitalWrite(LED, LOW);
    
    time_count++;
    delay(1);
    
    
  }

  req = client.readStringUntil('\r');
  digitalWrite(LED, HIGH);
  
  client.flush();

  if(req.indexOf("ON0") != -1)
  {
    digitalWrite(RLY1, HIGH);
    states |= 0x01;
  }
  else if(req.indexOf("OFF0") != -1) 
  {
    digitalWrite(RLY1, LOW);
    states &= ~0x01;
  }
  else if(req.indexOf("ON1") != -1)
  {
    digitalWrite(RLY2, HIGH);
    states |= 0x02;
  }
  else if(req.indexOf("OFF1") != -1) 
  {
    digitalWrite(RLY2, LOW);
    states &= ~0x02;
  }
  else if(req.indexOf("ON2") != -1)
  {
    digitalWrite(RLY3, HIGH);
    states |= 0x04;
  }
  else if(req.indexOf("OFF2") != -1) 
  {
    digitalWrite(RLY3, LOW);
    states &= ~0x04;
  }
  else if(req.indexOf("ON3") != -1)
  {
    digitalWrite(RLY4, HIGH);
    states |= 0x08;
  }
  else if(req.indexOf("OFF3") != -1) 
  {
    digitalWrite(RLY4, LOW);
    states &= ~0x08;
  }

  client.flush();
  client.print(file);
 
  LCD_clear_home();
  LCD_goto(1, 0);
  LCD_putstr("R1  R2  R3  R4");
  LCD_goto(2, 1);
  LCD_putchar(0x30 + (states & 0x01));
  LCD_goto(6, 1);
  LCD_putchar(0x30 + ((states & 0x02) >> 1));
  LCD_goto(10, 1);
  LCD_putchar(0x30 + ((states & 0x04) >> 2));
  LCD_goto(14, 1);
  LCD_putchar(0x30 + ((states & 0x08) >> 3));
  delay(1000);

  LCD_clear_home(); 
  SSID_IP_show();
}
