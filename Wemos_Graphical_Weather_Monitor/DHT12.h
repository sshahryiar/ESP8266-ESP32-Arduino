#pragma once


#include <Arduino.h>
#include <Wire.h>


#define DHT12_I2C_Address                	0x5C

#define DHT12_no_of_bytes_to_read 				0x05

#define DHT12_RH_HB												0x00
#define DHT12_RH_LB												0x01
#define DHT12_T_HB												0x02
#define DHT12_T_LB												0x03
#define DHT12_CRC_BYTE										0x04

#define DHT12_no_error										0x00
#define DHT12_CRC_error										0x01


class DHT12
{
	public:
		DHT12();
		void begin(void);
		uint8_t get_data(float *_RH, float *_T);

	private:
		uint8_t get_CRC(uint8_t *array_values);
		uint8_t read_byte(uint8_t reg_address);
};