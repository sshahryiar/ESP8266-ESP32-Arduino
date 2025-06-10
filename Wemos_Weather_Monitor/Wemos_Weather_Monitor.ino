#include <stdio.h>
#include "SHT3x.h"
#include "BMP180.h"
#include "SSD130x.h"


SHT3x hygrometer;
BMP180 barometer;
OLED display;


char buffer[40];

float t = 0;
float rh = 0;
float prs = 0;


void print_value(uint16_t x_pos, uint16_t y_pos, float value);


void setup(void) 
{
  Serial.begin(115200);
  pinMode(2, OUTPUT);
  display.begin();
  barometer.begin();
  hygrometer.begin();
  display.clear_screen();
  display.Text(32, 2, "ESP Weather");
  display.Text(35, 4, "T/'C:");
  display.Text(35, 5, "R.H/%:");
  display.Text(35, 6, "P/kPa:");
}

void loop(void) 
{
  digitalWrite(2, HIGH);

  prs = barometer.get_P();
  hygrometer.read_sensor(&t, &rh);
  Serial.print("RH/%: ");
  Serial.println(rh);
  Serial.print("T/'C: ");
  Serial.println(t);
  Serial.print("P/KpA: ");
  Serial.println(prs);
  print_value(70, 4, t);
  print_value(72, 5, rh);
  print_value(77, 6, prs);
  delay(600);
  digitalWrite(2, LOW);
  delay(600);
}


void print_value(uint16_t x_pos, uint16_t y_pos, float value)
{
  char buffer[40];
  char floatStr[20];

  dtostrf(value, 3, 1, floatStr);
  sprintf(buffer, "%s ", floatStr);
  display.Text(x_pos, y_pos, buffer);
}