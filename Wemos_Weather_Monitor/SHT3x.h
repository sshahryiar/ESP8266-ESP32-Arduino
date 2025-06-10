#pragma once


#include <Arduino.h>
#include <Wire.h>


#define SHT3x_I2C_Address                				0x45

#define SHT3x_MEAS_HIGH_REP_CLK_STRETCH         0x2C06 
#define SHT3x_MEAS_MED_REP_CLK_STRETCH          0x2C0D
#define SHT3x_MEAS_LOW_REP_CLK_STRETCH          0x2C10
#define SHT3x_MEAS_HIGH_REP                     0x2400
#define SHT3x_MEAS_MED_REP                      0x240B
#define SHT3x_MEAS_LOW_REP                      0x2416 
#define SHT3x_READ_STATUS                       0xF32D  
#define SHT3x_CLEAR_STATUS                      0x3041
#define SHT3x_SOFT_RESET                        0x30A2
#define SHT3x_ENABLE_INTERNAL_HEATER            0x306D    
#define SHT3x_DISABLE_INTERNAL_HEATER           0x3066 

#define SHT3x_heater_on                         0x00
#define SHT3x_heater_off                        0x01

#define SHT3x_ERROR                             0x01
#define SHT3x_NO_ERROR                          0x00


class SHT3x
{
	public:
		SHT3x();
		void begin(void);
		void read_status(uint8_t *buffer);
		uint8_t read_sensor(float *t, float *rh);

	private:
		void init(void);
		void reset(void);
		void heater_state(uint8_t state);
		void write_command(unsigned int value);
		uint8_t calculate_crc8(uint8_t *value);
		uint16_t make_word(uint8_t hb, uint8_t lb);
};