#include <ESP8266WiFi.h>
#include <WiFiManager.h>  
#include <ESP8266WebServer.h>
#include <Adafruit_NeoPixel.h>
#include "page.h"


#define LED_PIN    15   
#define NUM_LEDS   12   

#define dly1       45
#define dly2       90 
#define dly3       600
#define dly4       60 


WiFiManager WM;
ESP8266WebServer server(80);
Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_RGB + NEO_KHZ800);


const uint8_t LUT1[16] = {0, 49, 97, 141, 180, 212, 235, 250, 254, 250, 234, 211, 181, 140, 97, 50};
const uint8_t LUT2[16] = {235, 250, 254, 250, 234, 211, 181, 140, 97, 50, 0, 49, 97, 141, 180, 212};
const uint8_t LUT3[16] = {211, 181, 140, 97, 50, 0, 49, 97, 141, 180, 212, 235, 250, 254, 250, 234};

String mode;


void handleRoot(void);
void handleSet(void);
void handleNotFound(void);
void setColor(uint32_t color); 


void setup(void) 
{
  Serial.begin(115200);
  Serial.flush();
  delay(100);
  Serial.println();
  Serial.println("ESP RGB Webserver starting...");

  strip.begin();
  strip.show();

  WiFi.mode(WIFI_STA);

  WM.autoConnect("MyDeviceAP");
  if(!WM.autoConnect("MyDeviceAP")) 
  {
    Serial.println("Failed to connect with stored SSID. Starting fallback...");
    WM.resetSettings();
    WM.startConfigPortal("FallbackAP");
  }

  Serial.println("Connected.");
  WM.setConnectTimeout(180);
  WM.setConnectRetries(10);

  server.on("/", HTTP_GET, handleRoot);
  server.on("/set", HTTP_GET, handleSet);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");

  strip.setBrightness(25);
  setColor(0xFFFF);
}

void loop(void) 
{
  if(WiFi.status() != WL_CONNECTED) 
  {
    WM.setEnableConfigPortal(false);
    WM.autoConnect("AP");
    Serial.println("Reconnected.");
    delay(1000);
  }
  else 
  {
    server.handleClient();
  }
}

void handleRoot(void) 
{
  server.send_P(200, "text/html", MAIN_page);
}


void handleSet(void) 
{
  uint8_t r = 0;
  uint8_t g = 0;
  uint8_t b = 0;
  int16_t c = 0; 
  int16_t brightness = 0;
  uint32_t number = 0;

  String hex;
  
  if(server.hasArg("power")) 
  {
    if(server.arg("power") == "on") 
    {
      setColor(strip.Color(255, 255, 255));
    } 
    else 
    {
      setColor(strip.Color(0, 0, 0));
    }
  }

  if(server.hasArg("brightness")) 
  {
    brightness = server.arg("brightness").toInt();

    if(brightness <= 0) 
    {
      brightness = 0;
    }
    if(brightness >= 255) 
    {
      brightness = 255;
    }

    strip.setBrightness(brightness);
    strip.show();
  }

  if(server.hasArg("color")) 
  {
    hex = server.arg("color");
    number = strtol(hex.c_str(), NULL, 16);

    r = ((number >> 16) & 0xFF);
    g = ((number >> 8) & 0xFF);
    b = (number & 0xFF);

    setColor(strip.Color(r, g, b));
    strip.show();
  }


  if(server.hasArg("r") && server.hasArg("g") && server.hasArg("b")) 
  {
    r = server.arg("r").toInt();
    g = server.arg("g").toInt();
    b = server.arg("b").toInt();
    
    setColor(strip.Color(r, g, b));
    strip.show();
  }

  if(server.hasArg("mode")) 
  {
    mode = server.arg("mode");
  
    if(mode == "flash") 
    {
      for(c = 0; c < 10; c++) 
      {
        setColor(strip.Color(255, 255, 255)); 
        delay(dly4);

        setColor(strip.Color(0, 0, 0)); 
        delay(dly4);
      }
    } 
    
    else if(mode == "strobe") 
    {
      strip.setBrightness(175);

      for(c = 0; c < 4; c++) 
      {
        setColor(strip.Color(0, 0, 255)); 
        delay(dly3);

        setColor(strip.Color(0, 255, 255)); 
        delay(dly3);

        setColor(strip.Color(0, 255, 0)); 
        delay(dly3);

        setColor(strip.Color(255, 255, 0)); 
        delay(dly3);

        setColor(strip.Color(255, 0, 0)); 
        delay(dly3);

        setColor(strip.Color(255, 0, 255)); 
        delay(dly3);
      }
    } 
    
    else if(mode == "fade") 
    {
      for(c = 0; c < 16; c++) 
      {
        setColor(strip.Color(LUT1[c], LUT2[c], LUT3[c])); 
        delay(dly2);
      }

      for(c = 0; c < 16; c++) 
      {
        setColor(strip.Color(LUT3[c], LUT1[c], LUT2[c])); 
        delay(dly2);
      }

      for(c = 0; c < 16; c++) 
      {
        setColor(strip.Color(LUT2[c], LUT3[c], LUT1[c])); 
        delay(dly2);
      }
    } 
    
    else if(mode == "smooth") 
    {
      for(c = 0; c < 255; c += 25)   
      {
        setColor(strip.Color(c, 0, 0)); 
        delay(dly1);
      }

      for(c = 255; c > 0; c -= 25) 
      {
        setColor(strip.Color(c, 0, 0)); 
        delay(dly1);
      }

      for(c = 0; c < 255; c += 25)  
      {
        setColor(strip.Color(0, c, 0)); 
        delay(dly1);
      }

      for(c = 255; c > 0; c -= 25) 
      {
        setColor(strip.Color(0, c, 0)); 
        delay(dly1);
      }

      for(c = 0; c < 255; c += 25)  
      {
        setColor(strip.Color(0, 0, c)); 
        delay(dly1);
      }

      for(c = 255; c > 0; c -= 25) 
      {
        setColor(strip.Color(0, 0, c)); 
        delay(dly1);
      }

      for(c = 0; c < 255; c += 25) 
      {
        setColor(strip.Color(c, c, c)); 
        delay(dly1);
      }

      for(c = 255; c > 0; c -= 25) 
      {
        setColor(strip.Color(c, c, c)); 
        delay(dly1);
      }
    }
  }

  server.send(200, "text/plain", "OK");
}


void handleNotFound(void) 
{
  server.send(404, "text/plain", "Not found");
}


void setColor(uint32_t color) 
{
  uint8_t i = 0;

  for(i = 0; i < NUM_LEDS; i++) 
  {
    strip.setPixelColor(i, color);
  }
  strip.show();
}
