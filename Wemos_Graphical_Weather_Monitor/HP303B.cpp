#include <stdint.h>
#include "core_esp8266_features.h"
#include "HP303B.h"


HP303B::HP303B()
{
}


void HP303B::begin(void)
{
  init();
}


void HP303B::init(void)
{  
  uint8_t ts = 0;
  Wire.begin();
  Wire.flush();
  Wire.setClock(100000);

  write_byte(HP303B_RESET_REG, 0x89);
  delay(100);

  while(!(read_bytes(HP303B_MEAS_CFG_REG, 1) & 0x40));

  if(read_bytes(HP303B_COEF_SRC_REG, 1))
  {
    ts = 0x80;
  }

  write_byte(HP303B_PRS_CFG_REG, 0x66);
  write_byte(HP303B_TMP_CFG_REG, (ts | 0x66));
  write_byte(HP303B_MEAS_CFG_REG, 0x00);
  write_byte(HP303B_CFG_REG, 0x0C);
/*
  write_byte(0x0E, 0xA5);
	write_byte(0x0F, 0x96);
	write_byte(0x62, 0x02);
	write_byte(0x0E, 0x00);
	write_byte(0x0F, 0x00);
  read_T_raw();
*/
  while(!(read_bytes(HP303B_MEAS_CFG_REG, 1) & 0x80));
  get_coefficients();
}


void HP303B::write_byte(int8_t reg, int8_t value)
{
  Wire.beginTransmission(HP303B_I2C_Address); 
  Wire.write(reg);  
  Wire.write(value);           
  Wire.endTransmission(); 
}


int32_t HP303B::read_bytes(int8_t reg_address, uint8_t bytes_length)
{
  uint8_t i = 0;
  int32_t value = 0;
  
  Wire.beginTransmission(HP303B_I2C_Address); 
  Wire.write(reg_address);        
  Wire.endTransmission(); 

  Wire.requestFrom(HP303B_I2C_Address, bytes_length);    

  if(Wire.available() <= bytes_length) 
  { 
    for(i = 0; i < bytes_length; i++)
    {
      value <<= 8;
      value |= Wire.read();
    }
  }

  return value;
}


uint8_t HP303B::get_product_ID(void)
{
  return(0x0F & read_bytes(HP303B_ID_REG, 1));
}


uint8_t HP303B::get_revision_ID(void)
{
  return((0xF0 & read_bytes(HP303B_ID_REG, 1)) >> 4);
}


uint8_t HP303B::get_coeff_source(void)
{
  return((0x80 & read_bytes(HP303B_COEF_SRC_REG, 1)) >> 7);
}


void HP303B::get_coefficients(void)
{
  int8_t i = 0; 
  int32_t c_bytes[18];

  Wire.beginTransmission(HP303B_I2C_Address); 
  Wire.write(HP303B_COEF_1_REG);        
  Wire.endTransmission(); 

  Wire.requestFrom(HP303B_I2C_Address, 18);    

  if(Wire.available() <= 18) 
  { 
    for(i = 0; i < 18; i++)
    {
      c_bytes[i] = Wire.read();
    }
  }

  C0 = make_word(&c_bytes[0], 2);
  C0 &= 0xFFF0;
  C0 >>= 4;
  C0 = twos_complement(C0, 12);

  C1 = make_word(&c_bytes[1], 2);
  C1 &= 0x0FFF;
  C1 = twos_complement(C1, 12);

  C00 = make_word(&c_bytes[3], 3);
  C00 &= 0xFFFFF0;
  C00 >>= 4;
  C00 = twos_complement(C00, 20);

  C10 = make_word(&c_bytes[5], 3);
  C10 &= 0x0FFFFF;
  C10 = twos_complement(C10, 20);
 
  C01 = make_word(&c_bytes[8], 2);
  C01 = twos_complement(C01, 16);

  C11 = make_word(&c_bytes[10], 2);
  C11 = twos_complement(C11, 16);

  C20 = make_word(&c_bytes[12], 2);
  C20 = twos_complement(C20, 16);

  C21 = make_word(&c_bytes[14], 2);
  C21 = twos_complement(C21, 16);

  C30 = make_word(&c_bytes[16], 2);
  C30 = twos_complement(C30, 16);
}


int32_t HP303B::read_P_raw(void)
{
  int32_t value = 0;
   
  write_byte(HP303B_MEAS_CFG_REG, 0x01);
  while(!(read_bytes(HP303B_MEAS_CFG_REG, 1) & 0x10));
  write_byte(HP303B_MEAS_CFG_REG, 0x00);
  value = read_bytes(HP303B_PSR_B2_REG, 3);
  value &= 0x00FFFFFF;
  value = twos_complement(value, 24);

  return value;
}


int32_t HP303B::read_T_raw(void)
{
  int32_t value = 0;
  
  write_byte(HP303B_MEAS_CFG_REG, 0x02);
  while(!(read_bytes(HP303B_MEAS_CFG_REG, 1) & 0x20));
  write_byte(HP303B_MEAS_CFG_REG, 0x00);
  value = read_bytes(HP303B_TMP_B2_REG, 3);
  value &= 0x00FFFFFF;
  value = twos_complement(value, 24);

  return value;
}


float HP303B::get_P_scaled(uint8_t scale_value)
{  
  return ((float)read_P_raw() / scaling_factor[scale_value]); 
}


float HP303B::get_T_scaled(uint8_t scale_value)
{ 
  return ((float)read_T_raw() / scaling_factor[scale_value]);
}


float HP303B::get_T(void)
{
  return((C0 * 0.5) + (C1 * get_T_scaled(6)));
}


float HP303B::get_P(void)
{  
  float temp = 0.0;
  float P_comp = 0.0;
  float P_rsc = get_P_scaled(6);
  float T_rsc = get_T_scaled(6);

  temp = ((T_rsc * C01) + (T_rsc * P_rsc * (C11 + (C21 * P_rsc))));
  P_comp = (C00 + P_rsc * (C10 + P_rsc * (C20 + (C30 * P_rsc))));
  P_comp += temp;
  P_comp /= 1000.0;

  return P_comp;
}


int32_t HP303B::twos_complement(int32_t value, int8_t bits)
{
  if(value & (1 << (bits - 1)))
  {
    value -= (1 << bits);
  }
 
  return value;
}


int32_t HP303B::make_word(int32_t *values, uint8_t length)
{
  uint8_t i = 0;
  int32_t value = 0;

  for(i = 0; i < length; i++)
  {
    value <<= 8;
    value |= values[i];
  }

  return value;
}
