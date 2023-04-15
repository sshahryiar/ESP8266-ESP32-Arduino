#include <ESP8266WiFi.h>
#include "pinmap.h"
#include "ST7735S.h"


#define LED           DIO4
#define SERVER_PORT   80


const char* server_ip = "api.openweathermap.org";   

String city = "dhaka,bd";
String str_get1 = "GET /data/2.5/weather?q=";      
String str_get2 = "&mode=xml&units=metric&type=accurate&APPID=a6fa3492bd3d13a0ccd88783caf6e927 HTTP/1.1\r\n";
String str_host = "Host: api.openweathermap.org\r\n\r\n";


WiFiServer server(SERVER_PORT);
WiFiClient client; 


void SSID_IP_show(unsigned char y_pos)
{
  char temp[12];
  
  print_str(0, y_pos, 1, white, black, "SSID:");
  print_str(0, (11 + y_pos), 1, white, black, "IP:");
  
  memset(temp, 0x00, sizeof(temp));
  sprintf(temp, "%c%c%c%c%c%c%c%c%c%c", WiFi.SSID()[0], WiFi.SSID()[1], WiFi.SSID()[2], WiFi.SSID()[3], WiFi.SSID()[4], WiFi.SSID()[5], WiFi.SSID()[6], WiFi.SSID()[7], WiFi.SSID()[8], WiFi.SSID()[9]);
  print_str(36, y_pos, 1, white, black, temp);

  memset(temp, 0x00, sizeof(temp));
  sprintf(temp, "%d.%d.%d.%d", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3]);
  print_str(20, (11 + y_pos), 1, white, black, temp);
}


void WiFi_details(void)
{
  print_str(0, 33, 1, white, black, "WiFi connected.");
 
  server.begin();
  delay(2000);
  
  SSID_IP_show(55);
  delay(4000);
}


void WiFi_setup(void)
{
  signed char s = 15;

  digitalWrite(LED, HIGH);
  WiFi.mode(WIFI_STA);
  delay(1000);

  WiFi.begin("","");

  digitalWrite(LED, LOW);
  TFT_fill(black);

  while(s > 0)
  {
      print_str(0, 1, 1, white, black, "Connecting WiFi....");

      digitalWrite(LED, HIGH);
      delay(200);
      digitalWrite(LED, LOW);
      delay(200);
       
      s--;
  };

  
  if(WiFi.status() != WL_CONNECTED) 
  {
      digitalWrite(LED, HIGH);
      print_str(0, 11, 1, white, black, "Attempting WPS.");
      
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
        print_str(0, 22, 1, white, black, "Connection Failed!");
        delay(1000);

        print_str(0, 33, 1, white, black, "Retrying...."); 
        delay(1000);    
        ESP.restart();
      }
  }
  else 
  {
    digitalWrite(LED, LOW);
    print_str(0, 11, 1, white, black, "Connected....");
    delay(1600);
    
    WiFi_details(); 
  }

  delay(4000);
  TFT_fill(black);
}


void send_request(void)
{
    unsigned char timeout = 0;

    print_str(0, 36, 1, white, black, "Connecting TCP Server");    
    
    while(!client.connect(server_ip, SERVER_PORT))
    {
      delay(100);
      timeout++;
      if(timeout > 60)
      {       
        return;
      }
    } 

    print_str(0, 46, 1, white, black, "Server Connected....");  
    delay(400);
    client.print(str_get1 + city+str_get2 + str_host);
    delay(100);
}

 
String crop_string(String input, String header, String get_string)
{
    signed int num_get = 0;
    signed int stop_val = 0;
    signed int start_val = 0;
    
    if(input.indexOf(header) != -1)    
    {
      num_get = input.indexOf(get_string);  
      if (num_get != -1)     
      {               
        start_val = input.indexOf("\"",num_get);
        start_val++;
        stop_val = input.indexOf("\"",start_val);   
        return(input.substring(start_val, stop_val));    
      }
      else
      {
        return("NULL");
      }
    }
     
    return("NULL");
}


void setup(void) 
{
  pinMode(LED, OUTPUT); 
  digitalWrite(LED, LOW);

  ST7735_init();
  ST7735_Set_Rotation(0x00);
  TFT_fill(black);

  WiFi_setup();

  SSID_IP_show(1);
}


void loop(void) 
{
  const char *temp;
        
  String line;       
  String get_data;
  
  while(client.available())              
  {
    digitalWrite(LED, HIGH);
    line = client.readStringUntil('\n');     
    
    get_data = crop_string(line, "city", "name"); 
    temp = get_data.c_str();
    
    if(get_data != "NULL")            
    {
      print_str(48, 64, 1, white, black, temp);
    }

    get_data = crop_string(line,"temperature","temperature value");    
    temp = get_data.c_str();
    
    if(get_data != "NULL")
    {
      print_str(0, 80, 1, white, black, "Temperature ('C)");
      print_str(100, 80, 1, white, black, temp);   
    }

    get_data = crop_string(line,"humidity","humidity value");  
    temp = get_data.c_str();
    
    if(get_data != "NULL")
    {
      print_str(0, 90, 1, white, black, "R.Humidity (%)");
      print_str(100, 90, 1, white, black, temp);
    }

    get_data = crop_string(line,"pressure","pressure value"); 
    temp = get_data.c_str();
    
    if(get_data != "NULL")
    {
      print_str(0, 100, 1, white, black, "Pressure (mbar)");
      print_str(100, 100, 1, white, black, temp);
    }

    get_data = crop_string(line,"speed","speed value"); 
    temp = get_data.c_str();
    
    if(get_data != "NULL")
    {
      print_str(0, 110, 1, white, black, "Wind Speed (m/s)");
      print_str(100, 110, 1, white, black, temp);
    }

    get_data = crop_string(line,"direction","direction value");
    temp = get_data.c_str();
    
    if(get_data != "NULL")
    {
      print_str(0, 120, 1, white, black, "Wind Direction");
      print_str(100, 120, 1, white, black, temp);
    }

    get_data = crop_string(line,"visibility","visibility value"); 
    temp = get_data.c_str();
    
    if(get_data != "NULL")
    {
      print_str(0, 130, 1, white, black, "Visibility (m)");
      print_str(100, 130, 1, white, black, temp);
    }

    get_data = crop_string(line,"lastupdate","lastupdate value");      
    temp = get_data.c_str();
    
    if(get_data != "NULL")
    {
      print_str(0, 140, 1, white, black, "Updated on:");
      print_str(0, 150, 1, white, black, temp);
    }
  }

  delay(6000);
  
  Draw_Rectangle(0, 36, 127, 60, YES, SQUARE, black, black); 
  digitalWrite(LED, LOW);
  delay(4000);
  send_request();
}
