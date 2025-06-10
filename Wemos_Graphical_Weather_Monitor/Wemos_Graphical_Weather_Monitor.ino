#include <stdio.h>
#include "DHT12.h"
#include "HP303B.h"
#include "ST7735.h"
#include <math.h>


#define pi_by_2       1.571


ST7735 tft;
DHT12 hygrometer;
HP303B barometer;


float t1 = 0;
float t2 = 0;
float rh = 0;
float prs = 0;
float tavg = 0;

float t_new = 0;
float t_old = 0;
float p_new = 0;
float p_old = 0;
float rh_new = 0;
float rh_old = 0;


float map_float_value(float value, float x_min, float x_max, float y_min, float y_max);
void draw_background(void);
void print_value(uint16_t x_pos, uint16_t y_pos, float value);
void draw_pointer(uint16_t x_pos, uint16_t y_pos, uint16_t pointer_size, float present_value, float previous_value, float value_min, float value_max);


void setup(void) 
{
  Serial.begin(115200);
  Serial.flush();
  pinMode(2, OUTPUT);
  barometer.begin();
  hygrometer.begin();
  tft.begin();
  tft.Fill(BLACK);
}


void loop(void) 
{
  digitalWrite(2, HIGH);
  draw_background();
  
  t1 = barometer.get_T();
  prs = barometer.get_P();
  hygrometer.get_data(&rh, &t2);
  tavg = ((t1 + t2) * 0.5);

  t_new = tavg;
  p_new = prs;
  rh_new = rh;

  Serial.print("RH/%: ");
  Serial.println(rh);
  Serial.print("T/'C: ");
  Serial.println(tavg);
  Serial.print("P/KpA: ");
  Serial.println(prs);

  draw_pointer(31, 45, 26, t_new, t_old, 0, 100);
  draw_pointer(31, 85, 26, p_new, p_old, 30, 120);
  draw_pointer(31, 125, 26, rh_new, rh_old, 0, 100);

  t_old = t_new;
  p_old = p_new;
  rh_old = rh_new;
  
  delay(600);
  digitalWrite(2, LOW);
  delay(600);
}


float map_float_value(float value, float x_min, float x_max, float y_min, float y_max)
{
   return (y_min + (((y_max - y_min) / (x_max - x_min)) * (value - x_min)));
}


void draw_background(void)
{
  tft.text(1, 1, 1, CYAN, BLACK, "HP303B DHT12 ESP8266");
  
  tft.Gauge(31, 45, 30, false, Upper, RED);
  tft.H_Line(1, 61, 45, RED);
  tft.Circle(31, 45, 3, true, CYAN);

  tft.Gauge(31, 85, 30, false, Upper, YELLOW);
  tft.H_Line(1, 61, 85, YELLOW);  
  tft.Circle(31, 85, 3, true, CYAN);

  tft.Gauge(31, 125, 30, false, Upper, GREEN);
  tft.H_Line(1, 61, 125, GREEN);
  tft.Circle(31, 125, 3, true, CYAN);

  tft.text(70, 12, 2, WHITE, BLACK, "T/'C");
  tft.text(70, 52, 2, WHITE, BLACK, "P/kPa");
  tft.text(70, 92, 2, WHITE, BLACK, "RH/%");
}


void print_value(uint16_t x_pos, uint16_t y_pos, float value)
{
  char buffer[40];
  char floatStr[20];

  dtostrf(value, 3, 1, floatStr);
  sprintf(buffer, "%s ", floatStr);
  tft.text(x_pos, y_pos, 2, WHITE, BLACK, buffer);
}


void draw_pointer(uint16_t x_pos, uint16_t y_pos, uint16_t pointer_size, float present_value, float previous_value, float value_min, float value_max)
{
  float val_1 = 0.0;
  float val_2 = 0.0;

  val_1 = map_float_value(present_value, value_min, value_max, -(pi_by_2), pi_by_2);
  val_2 = map_float_value(previous_value, value_min, value_max, -(pi_by_2), pi_by_2);

  tft.Line(x_pos, y_pos, (x_pos + (pointer_size * sin(val_2))), (y_pos - (pointer_size * cos(val_2))), BLACK);
  tft.Line(x_pos, y_pos, (x_pos + (pointer_size * sin(val_1))), (y_pos - (pointer_size * cos(val_1))), CYAN);

  print_value((x_pos + 40), (y_pos - 12), present_value);
}
