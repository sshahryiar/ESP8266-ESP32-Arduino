#pragma once


#include <Arduino.h>
#include <Wire.h>


#define BMP180_I2C_Address                  		0x77

#define BMP180_AC1_reg                          0xAA
#define BMP180_AC2_reg                          0xAC
#define BMP180_AC3_reg                          0xAE
#define BMP180_AC4_reg                          0xB0
#define BMP180_AC5_reg                          0xB2
#define BMP180_AC6_reg                          0xB4
#define BMP180_B1_reg                           0xB6
#define BMP180_B2_reg                           0xB8
#define BMP180_MB_reg                           0xBA
#define BMP180_MC_reg                           0xBC
#define BMP180_MD_reg                           0xBE
#define BMP180_CHIP_ID_reg		 								  0xD0  
#define BMP180_VERSION_reg			 								0xD1
#define BMP180_SOFT_RESET_reg			 							0xE0
#define BMP180_control_reg                      0xF4
#define BMP180_MSB                              0xF6
#define BMP180_LSB                              0xF7
#define BMP180_XLSB                             0xF8
#define BMP180_temp_conv_cmd                    0x2E
#define BMP180_pres_conv_cmd                    0x34
#define BMP180_OSS_value                        0x03

class BMP180
{
	public:
		uint8_t OSS = BMP180_OSS_value;

		BMP180();
		void begin(void);
		float get_T(void);
		int32_t get_P(void);

	private:
		int8_t init(void);
		void read_calibration(void);
		int32_t calculate_b5(int32_t value);
		uint16_t make_word(uint8_t *buffer);
		void write_byte(uint8_t reg_address, uint8_t value);
		uint16_t read_byte(uint8_t reg_address);
		uint16_t read_word(uint8_t reg_address);
		void read_multibyte(uint8_t *buffer, uint8_t reg_address, uint8_t length);
		uint16_t read_uncompensated_temperature(void);
		uint32_t read_uncompensated_pressure(void);
};
