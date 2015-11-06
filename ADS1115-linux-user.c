#include "ADS1115-linux-user.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#define VERIFY(x) if(!x){ \
						return ADS1115_ERR_NO_CONFIG; \
					}

static int __read_reg(uint8_t reg, uint16_t* userbuf, ADS1115_config* config);
static int __write_reg(uint8_t reg, uint16_t val, ADS1115_config* config);

static char buf[4];
static int FS_ranges_mV[6] = {6144, 4094, 2048, 1024, 512, 256};

int ADS1115_init(ADS1115_config* config, int i2c_file, uint8_t dev_address){
	int err = 0;

	if(!config)
		return ADS1115_ERR_NO_CONFIG;

	//TODO: see if defaulting the dev_address messes with the auto increment protocol

	if (ioctl(config->i2c_file, I2C_SLAVE, dev_address>>1) < 0) {
		perror("i2cSetAddress");
		return ADS1115_ERR_I2CfOPEN;
	}


	config->i2c_file = i2c_file;

	return err;

}

int ADS1115_init_and_open_i2c(ADS1115_config* config, int i2c_bus, uint8_t dev_address){
	int err = 0;
	char i2cpath[11] = {0};

	if(!config)
		return ADS1115_ERR_NO_CONFIG;

	config->dev_i2c_address = dev_address;

	//i2cpath = config->i2c_bus == 0 ? (char*)"/dev/i2c-0" : (char*)"/dev/i2c-1";
	snprintf(i2cpath, sizeof(i2cpath), "/dev/i2c-%d", i2c_bus);

	config->i2c_file = open(i2cpath, O_RDWR);

	if (config->i2c_file < 0) {
		perror("i2cOpen");
		return ADS1115_ERR_I2CfOPEN;
	}
	//see if defaulting the dev_address messes with the auto increment protocol

	if (ioctl(config->i2c_file, I2C_SLAVE, dev_address>>1) < 0) {
		perror("i2cSetAddress");
		return ADS1115_ERR_SET_SLAVEADDR;
	}

	return err;
}

int ADS1115_readReg(uint8_t reg,
		uint16_t* userbuf,
		ADS1115_config* config)
{

	VERIFY(config);

	return __read_reg(reg, userbuf, config);

}

int ADS1115_writeReg(uint8_t reg,
		uint16_t val,
		ADS1115_config* config,
		uint16_t mask )
{

	VERIFY(config);

	uint16_t temp;
	int err;

	if(mask == 0)
		return ADS1115_ERR_TRIVIAL_ACTION;

	if(mask != 0xffff){
		if(err = __read_reg(reg, &temp, config)){
			perror("error reading from device");
			return err;
		}
		val = (temp & (~mask)) | val;
	}

	return __write_reg(reg, val, config);

}

int ADS1115_readConversionValue(ADS1115_config* config, ADS1115_ADC_Val* conversionValue){

	VERIFY(config);

	return __read_reg(ADS1115_REG_CONVERSION, (uint16_t*)conversionValue, config);

}

int ADS1115_readConversion_miliVolts(ADS1115_config* config, ADS1115_ADC_Val* miliVolts, ADS1115_FS_range_t FSrange){

	VERIFY(config);
	int err;
	ADS1115_ADC_Val val;

	if( err = __read_reg(ADS1115_REG_CONVERSION, (uint16_t*)&val, config)){
		return err;
	}

	*miliVolts = (ADS1115_ADC_Val)((FS_ranges_mV[FSrange]*(int)val) >> 15);

	return err;
}

int ADS1115_close_i2c(ADS1115_config* config){
	VERIFY(config);

	if(config->i2c_file)
		close(config->i2c_file);
	else
		return ADS1115_ERR_NO_FILE;

	return ADS1115_ERR_NOERR;
}

static int __write_reg(uint8_t reg,
		uint16_t val,
		ADS1115_config* config)
{

	if (ioctl(config->i2c_file, I2C_SLAVE, config->dev_i2c_address>>1) < 0) {
		perror("i2cSetAddress");
		return ADS1115_ERR_I2CfOPEN;
	}

	//data[0] = config->dev_i2c_address | ADS1115_WRITE_BIT;
	buf[0] = reg;
	buf[1] = (char) (val >> 8);
	buf[2] = (char)val;

	if (write(config->i2c_file, buf, 3) != 3) {
		perror("pca9555SetRegister");
		return ADS1115_ERR_I2C_WRITE;
	}

	return 0;
}

static int __read_reg(uint8_t reg,
		uint16_t* userbuf,
		ADS1115_config* config)
{

	if (ioctl(config->i2c_file, I2C_SLAVE, config->dev_i2c_address>>1) < 0) {
		perror("i2cSetAddress");
		return ADS1115_ERR_I2CfOPEN;
	}

	//data[0] = config->dev_i2c_address;
	buf[0] = reg;

	if(write(config->i2c_file, buf, 1) != 1 ){
		perror("ads1115SetRegisterPair");
		return ADS1115_ERR_I2C_WRITE;
	}

	if(read(config->i2c_file, buf, 2) != 2 ){
		perror("ads1115SetRegisterPair");
		return ADS1115_ERR_I2C_READ;
	}

	*userbuf = ((uint16_t)buf[0] << 8) | buf[1];

	return 0;
}


