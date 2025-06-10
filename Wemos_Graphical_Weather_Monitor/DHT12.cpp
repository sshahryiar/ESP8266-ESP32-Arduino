#include "core_esp8266_features.h"
#include "DHT12.h"


DHT12::DHT12()
{
}


void DHT12::begin(void)
{
  Wire.begin();
  Wire.flush();
  Wire.setClock(100000);
  delay(100);
}


uint8_t DHT12::get_data(float *_RH, float *_T)
{
	int8_t i = DHT12_no_of_bytes_to_read;
	uint8_t values[i] = {0, 0, 0, 0, 0};

	while(i > 0)
	{
		values[(DHT12_no_of_bytes_to_read - i)] = read_byte((DHT12_no_of_bytes_to_read - i));
		i--;	
	};

	if(values[DHT12_CRC_BYTE] == get_CRC(values))
	{
		*_RH = ((values[DHT12_RH_HB]) + ((values[DHT12_RH_LB]) * 0.1));
		*_T = ((values[DHT12_T_HB]) + ((values[DHT12_T_LB]) * 0.1));

		return DHT12_no_error;
	}

	else
	{
		return DHT12_CRC_error;
	}
}


uint8_t DHT12::get_CRC(uint8_t *array_values)
{
	int8_t i = 3;
	uint8_t crc_result = 0;

	while(i > -1)
	{
		crc_result += array_values[i];
		i--;
	};

	return crc_result;
}


uint8_t DHT12::read_byte(uint8_t reg_address)
{
  uint8_t value = 0;
	
	Wire.beginTransmission(DHT12_I2C_Address); 
  Wire.write(reg_address);        
  Wire.endTransmission(); 

  Wire.requestFrom(DHT12_I2C_Address, 1);  

  if(Wire.available())
  {
  	value = Wire.read();
  } 

	Wire.endTransmission(); 

	return value;
}