#include "HardwareSerial.h"
#include "c_types.h"
#include "BMP180.h"


static int16_t mb;
static int16_t mc;
static int16_t md;
static int16_t ac1;
static int16_t ac2;
static int16_t ac3;
static int16_t _b1;
static int16_t _b2;
static uint16_t ac4;
static uint16_t ac5;
static uint16_t ac6;


BMP180::BMP180()
{
}


void BMP180::begin(void)
{
	init();
}


float BMP180::get_T(void)
{
	float tmp = 0;
	int32_t ut = read_uncompensated_temperature();
	int32_t b5 = calculate_b5(ut);

	tmp = ((8 + b5) >> 4);
	tmp *= 0.1;

	return (tmp - 180);
}


int32_t BMP180::get_P(void)
{
	uint32_t _b4 = 0;
	uint32_t _b7 = 0;
  int32_t p = 0;
  int32_t x1 = 0;
  int32_t x2 = 0;
  int32_t x3 = 0;
  int32_t _b3 = 0; 
	int32_t _b5 = 0;
  int32_t _b6 = 0;
	int32_t fp = 0;
	int32_t ut = 0;
	int32_t up = 0;

	ut = read_uncompensated_temperature();
	up = read_uncompensated_pressure();
	_b5 = calculate_b5(ut);

  _b6 = (_b5 - 4000);
  x1 = ((_b2 * (_b6 * _b6) >> 12) >> 11);
  x2 = ((ac2 * _b6) >> 11);
  x3 = (x1 + x2);
  _b3 = (((((((int32_t)ac1) * 4) + x3) << OSS) + 2) >> 2);
  x1 = ((ac3 * _b6) >> 13);
  x2 = ((_b1 * ((_b6 * _b6) >> 12)) >> 16);
  x3 = (((x1 + x2) + 2) >> 2);
  _b4 = ((ac4 * (uint32_t)(x3 + 32768)) >> 15);
  _b7 = ((uint32_t)(up - _b3) * (50000 >> OSS));

  if(_b7 < 0x80000000)
  {
		p = ((_b7 << 1) / _b4);
  }
  else
  {
		p = ((_b7 / _b4) << 1);
  }

  x1 = ((p >> 8) * (p >> 8));
  x1 = ((x1 * 3038) >> 16);
  x2 = ((-7357 * p) >> 16);
  fp = (p + ((x1 + x2 + 3791) >> 4));

  return ((fp - 14300) / 1000);
}


int8_t BMP180::init(void)
{
	Wire.begin();
	Wire.setClock(20000);
	delay(100);

	if(read_byte(BMP180_CHIP_ID_reg) != 0x55)
	{
		return -1;
	}

	write_byte(BMP180_SOFT_RESET_reg, 0xB6);

	read_calibration();
	return 0;
}


void BMP180::read_calibration(void)
{
	ac1 = (int16_t)read_word(BMP180_AC1_reg);
	ac2 = (int16_t)read_word(BMP180_AC2_reg);
	ac3 = (int16_t)read_word(BMP180_AC3_reg);
	ac4 = read_word(BMP180_AC4_reg);
	ac5 = read_word(BMP180_AC5_reg);
	ac6 = read_word(BMP180_AC6_reg);
	_b1 = (int16_t)read_word(BMP180_B1_reg);
	_b2 = (int16_t)read_word(BMP180_B2_reg);
	mb = (int16_t)read_word(BMP180_MB_reg);
	mc = (int16_t)read_word(BMP180_MC_reg);
	md = (int16_t)read_word(BMP180_MD_reg);
}


int32_t BMP180::calculate_b5(int32_t value)
{
	int32_t x1 = ((value - (int32_t)ac6) * ((int32_t)ac5) >> 15);
  int32_t x2 = (((int32_t)mc << 11) / (x1 + (int32_t)md));
  
	return (x1 + x2);
}


uint16_t BMP180::make_word(uint8_t *buffer)
{
	uint8_t i = 0;
	uint16_t value = 0;

	for(i = 0; i < 2; i++)
	{
		value <<= 8;
		value |= buffer[i];
	}

	return value;
}


void BMP180::write_byte(uint8_t reg_address, uint8_t value)
{
	Wire.beginTransmission(BMP180_I2C_Address); 
	Wire.write(reg_address);      
	Wire.write(value);    
	Wire.endTransmission(); 
}


uint16_t BMP180::read_byte(uint8_t reg_address)
{
  uint8_t temp[1] = {0};
	
	read_multibyte(temp, reg_address, 1);

	return temp[0];
}


uint16_t BMP180::read_word(uint8_t reg_address)
{
	uint8_t temp[2] = {0, 0};

	read_multibyte(temp, reg_address, 2);

	return make_word(temp);
}


void BMP180::read_multibyte(uint8_t *buffer, uint8_t reg_address, uint8_t length)
{
  uint8_t i = 0;

	if(length != 0)
  {
		Wire.beginTransmission(BMP180_I2C_Address); 
		Wire.write(reg_address);        
		Wire.endTransmission(); 

		Wire.requestFrom((uint8_t)BMP180_I2C_Address, (uint8_t)length);    
		if(Wire.available())
		{
			for(i = 0; i < length; i++)
			{
				buffer[i] = Wire.read();
			}
		}

		Wire.endTransmission(); 
	}
}


uint16_t BMP180::read_uncompensated_temperature(void)
{
	uint32_t ut = 0;

	write_byte(BMP180_control_reg, BMP180_temp_conv_cmd);
	delay(5);
	ut = read_word(BMP180_MSB);
	return ut;
}


uint32_t BMP180::read_uncompensated_pressure(void)
{
	uint32_t up = 0;
	uint8_t temp[3] = {0, 0, 0};

	write_byte(BMP180_control_reg, (BMP180_pres_conv_cmd + (OSS << 6)));

	switch(OSS)
	{
		case 1:
		{
			delay(8);
			break;
		}
		case 2:
		{
			delay(14);
			break;
		}
		case 3:
		{
			delay(26);
			break;
		}
		default:
		{
			delay(5);
			break;
		}
	}

	read_multibyte(temp, BMP180_MSB, 3);

	up = ((((uint32_t)temp[0] << 16) + ((uint32_t)temp[1] << 8) + (uint32_t)temp[2]));
	up >>= (8 - OSS);

	return up;
}