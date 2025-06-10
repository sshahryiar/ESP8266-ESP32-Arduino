#include "SHT3x.h"


SHT3x::SHT3x()
{
}


void SHT3x::begin(void)
{
	init();
}


void SHT3x::read_status(uint8_t *buffer)
{
	uint8_t i = 0;

    write_command(SHT3x_READ_STATUS);

    Wire.requestFrom(SHT3x_I2C_Address, 3);    

  	if(Wire.available() <= 3) 
  	{
  		for(i = 0; i < 3; i++)
	    {
	      buffer[i] = Wire.read();
	    }
  	}
}


uint8_t SHT3x::read_sensor(float *t, float *rh)
{
	uint8_t data_buffer[6] = {0, 0, 0, 0, 0, 0};
    uint8_t i = 0;
    int32_t t_tmp = 0;
    int32_t h_tmp = 0;
    float t_value = 0.0;
    float h_value = 0.0;

    write_command(SHT3x_MEAS_HIGH_REP);
    delay(20);

    Wire.requestFrom(SHT3x_I2C_Address, 6);    

    for(i = 0; i < 6; i++)
    {
        data_buffer[i] = Wire.read();
    }

    Wire.endTransmission(); 

  	if((data_buffer[2] != calculate_crc8(data_buffer)) || (data_buffer[5] != calculate_crc8((data_buffer + 0x03))))
    {
        return SHT3x_ERROR;
    }

    t_tmp = make_word(data_buffer[0], data_buffer[1]);
    t_tmp = (((4375 * t_tmp) >> 14) - 4500);
    t_value = (((float)t_tmp) / 100.0);
    *t = t_value;

    h_tmp = make_word(data_buffer[3], data_buffer[4]);
    h_tmp = ((625 * h_tmp) >> 12);
    h_value = (((float)h_tmp) / 100.0);
    
    *rh = h_value;
    
    return SHT3x_NO_ERROR;
}


void SHT3x::init(void)
{
	Wire.begin();
  	Wire.setClock(20000);
  	delay(100);

  	reset();
  	heater_state(SHT3x_heater_on);
  	delay(1000);
  	heater_state(SHT3x_heater_off);
}


void SHT3x::reset(void)
{
	write_command(SHT3x_SOFT_RESET);
	delay(20);
}


void SHT3x::heater_state(uint8_t state)
{
    switch(state)
    {
        case SHT3x_heater_on:
        {
            write_command(SHT3x_ENABLE_INTERNAL_HEATER);
            break;
        }

        default:
        {
            write_command(SHT3x_DISABLE_INTERNAL_HEATER);
            break;
        }
    };
}


void SHT3x::write_command(unsigned int value)
{
	uint8_t hb = 0;
	uint8_t lb = 0;

	lb = (value & 0x00FF);
    hb = ((value & 0xFF00) >> 8);

    Wire.beginTransmission(SHT3x_I2C_Address); 
  	Wire.write(hb);      
  	Wire.write(lb);    
  	Wire.endTransmission(); 
}


uint8_t SHT3x::calculate_crc8(uint8_t *value)
{
    int8_t i = 0;
    int8_t j = 0;
    uint8_t crc_value = 0xFF;

    for(i = 0; i < 2; i++)
    {
        crc_value ^= value[i];

        for(j = 8; j > 0; --j)
        {
           if(crc_value & 0x80)
           {
               crc_value = ((crc_value << 1) ^ 0x131);
           }
           else
           {
               crc_value <<= 1;
           }
        };
    };

    return crc_value;
}


uint16_t SHT3x::make_word(uint8_t hb, uint8_t lb)
{
	uint16_t value = 0;

    value = hb;
    value <<= 8;
    value |= lb;

    return value;
}