/*
 * ADS115-linux-user.h
 *
 *  Created on: Nov 5, 2015
 *      Author: daslabasus1
 *
 *      We use the AD1115_REG_Val type instead of char[] so that the user doesnt need to know which byte is the MSB in i2c transmission
 *
 */
#include <stdint.h>

#ifndef ADS1115_LINUX_DRIVER_INCLUDE_ADS1115_LINUX_USER_H_
#define ADS1115_LINUX_DRIVER_INCLUDE_ADS1115_LINUX_USER_H_

//////////////////////////////////////////////
///////////////// ERROR LIST /////////////////
//////////////////////////////////////////////

#define ADS1115_ERR_NOERR					 0
#define ADS1115_ERR_NO_CONFIG				-1
#define ADS1115_ERR_DUTY_OVERFLOW			-2
#define ADS1115_ERR_PRESCALE_OVERFLOW		-3
#define ADS1115_ERR_PRESCALE_TOOLOW			-4
#define ADS1115_ERR_I2CfOPEN				-5
#define ADS1115_ERR_SET_SLAVEADDR			-6
#define ADS1115_ERR_WRITING_PRESCALE		-7
#define ADS1115_ERR_I2C_WRITE				-8
#define ADS1115_ERR_I2C_READ				-9
#define ADS1115_ERR_NO_FILE					-10
#define ADS1115_ERR_TRIVIAL_ACTION			-11
#define ADS1115_ERR_BOUNDS					-12

/////////////////////////////////////////////
/////////////// REGISTER LIST ///////////////
/////////////////////////////////////////////

#define ADS1115_REG_CONVERSION 0
#define ADS1115_REG_CONFIGURATION 1
#define ADS1115_REG_LOTHRESH 2
#define ADS1115_REG_HITHRESH 3

/////////////////////////////////////////////
////////////// CONF SETTINGS ////////////////
/////////////////////////////////////////////

#define ADS1115_COMPARE_AIN0toAIN1 ((0b000) << 13)
#define ADS1115_COMPARE_AIN0toAIN3 ((0b001) << 13)
#define ADS1115_COMPARE_AIN1toAIN3 ((0b010) << 13)
#define ADS1115_COMPARE_AIN2toAIN3 ((0b011) << 13)
#define ADS1115_COMPARE_AIN0toGND ((0b100) << 13)
#define ADS1115_COMPARE_AIN1toGND ((0b101) << 13)
#define ADS1115_COMPARE_AIN2toGND ((0b110) << 13)
#define ADS1115_COMPARE_AIN3toGND ((0b111) << 13)

#define ADS1115_GAINmV_FS6144 ((0b000) << 10)
#define ADS1115_GAINmV_FS4096 ((0b001) << 10)
#define ADS1115_GAINmV_FS2048 ((0b010) << 10)
#define ADS1115_GAINmV_FS1024 ((0b011) << 10)
#define ADS1115_GAINmV_FS0512 ((0b100) << 10)
#define ADS1115_GAINmV_FS0256 ((0b101) << 10)

#define ADS1115_CONTINUOUS_CONVERSION (1<<8)

#define ADS1115_DATARATE_008_SPS ((0b000)<<5)
#define ADS1115_DATARATE_016_SPS ((0b001)<<5)
#define ADS1115_DATARATE_032_SPS ((0b010)<<5)
#define ADS1115_DATARATE_064_SPS ((0b011)<<5)
#define ADS1115_DATARATE_128_SPS ((0b100)<<5)
#define ADS1115_DATARATE_250_SPS ((0b101)<<5)
#define ADS1115_DATARATE_475_SPS ((0b110)<<5)
#define ADS1115_DATARATE_860_SPS ((0b111)<<5)

typedef enum{
	FS_6144 = 0,
	FS_4096 = 1,
	FS_2048 = 2,
	FS_1024 = 3,
	FS_0512 = 4,
	FS_0256 = 5,
} ADS1115_FS_range_t;


#ifdef __cplusplus
extern "C"{
#endif

typedef struct ADS1115_config {
	int i2c_file;
	uint8_t dev_i2c_address;
} ADS1115_config;

typedef int16_t ADS1115_ADC_Val;

#ifdef __cplusplus
#define DEFAULT_PARAM(x) =x
#else
#define DEFAULT_PARAM(x)
#endif


int ADS1115_init(ADS1115_config* config, int i2c_file, uint8_t dev_address);
int ADS1115_init_and_open_i2c(ADS1115_config* config, int i2c_bus, uint8_t dev_address);
int ADS1115_readReg(uint8_t reg,
		uint16_t* buf,
		ADS1115_config* config);
int ADS1115_writeReg(uint8_t reg,
		uint16_t val,
		ADS1115_config* config,
		uint16_t mask DEFAULT_PARAM(0xFFFF));
int ADS1115_readConversionValue(ADS1115_config* config, ADS1115_ADC_Val* conversionValue);
int ADS1115_readConversion_miliVolts(ADS1115_config* config, ADS1115_ADC_Val* miliVolts, ADS1115_FS_range_t FSrange);
int ADS1115_close_i2c(ADS1115_config* config);


#ifdef __cplusplus
}
#endif

#endif /* ADS1115_LINUX_DRIVER_INCLUDE_ADS1115_LINUX_USER_H_ */
