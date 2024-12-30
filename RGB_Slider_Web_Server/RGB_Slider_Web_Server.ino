#include <stdio.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>  
#include <Adafruit_NeoPixel.h>      
#include "SSD130x.h"
#include "page.h"


OLED disp;

#define PIN D2
Adafruit_NeoPixel np = Adafruit_NeoPixel(1, PIN, NEO_GRB + NEO_KHZ800);

WiFiManager WM;
ESP8266WebServer server(80);

int R = 0;
int G = 0;
int B = 0;

void page_change(void)
{
  server.send(200, "text/html", file);
}


void write_colours(void)
{
  R = server.arg("r").toInt();
  G = server.arg("g").toInt();
  B = server.arg("b").toInt();
  np.setPixelColor(0, np.Color(R, G, B));
  np.show();
  page_change();
}


void setup(void) 
{
  char temp[16];
  memset(temp, 0x00, sizeof(temp));

  Serial.begin(115200);

  WM.setConnectTimeout(180);
  WM.setConnectRetries(10);
  WM.autoConnect("AutoConnectAP");
  Serial.println("Connected.");

  disp.begin();
  disp.clear_screen();
  disp.print_string(1, 1, "RGB Slider");
  sprintf(temp, "%d.%d", WiFi.localIP()[0], WiFi.localIP()[1]);
  disp.print_string(1, 4, temp);
  sprintf(temp, "%d.%d", WiFi.localIP()[2], WiFi.localIP()[3]);
  disp.print_string(1, 5, temp);

  np.begin();
  np.setPixelColor(0, np.Color(R, G, B));
  np.show();

  server.on("/", page_change);
  server.on("/write", write_colours);
  server.begin();
}


void loop(void)
{
  if(WiFi.status() != WL_CONNECTED) 
  {
    WM.setEnableConfigPortal(false);
    WM.autoConnect("AP");
    Serial.println("Reconnected.");
  }
  else 
  {
    server.handleClient();
  }
  delay(400);
}