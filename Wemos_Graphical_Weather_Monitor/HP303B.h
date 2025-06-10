#pragma once


#include <Arduino.h>
#include <Wire.h>


#define HP303B_I2C_Address                0x77

#define HP303B_PSR_B2_REG                 0x00
#define HP303B_PSR_B1_REG                 0x01
#define HP303B_PSR_B0_REG                 0x02
#define HP303B_TMP_B2_REG                 0x03
#define HP303B_TMP_B1_REG                 0x04
#define HP303B_TMP_B0_REG                 0x05
#define HP303B_PRS_CFG_REG                0x06
#define HP303B_TMP_CFG_REG                0x07
#define HP303B_MEAS_CFG_REG               0x08
#define HP303B_CFG_REG                    0x09
#define HP303B_INT_STS_REG                0x0A
#define HP303B_FIFO_STS_REG               0x0B
#define HP303B_RESET_REG                  0x0C
#define HP303B_ID_REG                     0x0D
#define HP303B_COEF_1_REG                 0x10
#define HP303B_COEF_2_REG                 0x11
#define HP303B_COEF_3_REG                 0x12
#define HP303B_COEF_4_REG                 0x13
#define HP303B_COEF_5_REG                 0x14
#define HP303B_COEF_6_REG                 0x15
#define HP303B_COEF_7_REG                 0x16
#define HP303B_COEF_8_REG                 0x17
#define HP303B_COEF_9_REG                 0x18
#define HP303B_COEF_10_REG                0x19
#define HP303B_COEF_11_REG                0x1A
#define HP303B_COEF_12_REG                0x1B
#define HP303B_COEF_13_REG                0x1C
#define HP303B_COEF_14_REG                0x1D
#define HP303B_COEF_15_REG                0x1E
#define HP303B_COEF_16_REG                0x1F
#define HP303B_COEF_17_REG                0x20
#define HP303B_COEF_18_REG                0x21
#define HP303B_COEF_SRC_REG               0x28


class HP303B
{
  public:
    HP303B();
    void begin(void);
    uint8_t get_product_ID(void);
    uint8_t get_revision_ID(void);
    uint8_t get_coeff_source(void);
    float get_T(void);
    float get_P(void);

  private:
    int32_t C0 = 0;
    int32_t C1 = 0;
    int32_t C00 = 0;
    int32_t C01 = 0;
    int32_t C10 = 0;
    int32_t C11 = 0;
    int32_t C20 = 0;
    int32_t C21 = 0;
    int32_t C30 = 0;

    const int32_t scaling_factor[8] = {524288, 
                                       1572864, 
                                       3670016, 
                                       7864320, 
                                       253952, 
                                       516096, 
                                       1040384, 
                                       2088960};

    void init(void);
    void write_byte(int8_t reg, int8_t value);
    int32_t read_bytes(int8_t reg_address, uint8_t bytes_length);
    void get_coefficients(void);
    int32_t read_P_raw(void);
    int32_t read_T_raw(void);
    float get_P_scaled(uint8_t scale_value);
    float get_T_scaled(uint8_t scale_value);
    int32_t twos_complement(int32_t value, int8_t bits);
    int32_t make_word(int32_t *values, uint8_t length);
};
