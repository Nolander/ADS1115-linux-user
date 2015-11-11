#include "ADS1115-linux-user.h"
#include <stdio.h>

using namespace std;


static ADS1115_config myconfig;

int main(){
	int err;
	ADS1115_ADC_Val val = 0;
	uint16_t configval = ADS1115_COMPARE_AIN3toGND | ADS1115_DATARATE_128_SPS | ADS1115_CONTINUOUS_CONVERSION | ADS1115_RANGEmV_FS6144 | ADS1115_COMP_QUE_DISABLE;

	printf("here1\n");

	if(err =  ADS1115_init_and_open_i2c(&myconfig, 2, 0x90)){
		printf("err is %d\n", err);
	}

	printf("here2\n");

	if(err = ADS1115_writeReg(ADS1115_REG_CONFIGURATION, configval , &myconfig, 0xffff)){
		printf("err is %d\n", err);
	}

	if(err = ADS1115_readReg(ADS1115_REG_CONFIGURATION, (uint16_t*)&val, &myconfig)){
		printf("err is %d\n", err);
	}

	printf("%d\n", (uint16_t)val);

	printf("here3\n");

	while(1){
		if(err =  ADS1115_readConversion_miliVolts(&myconfig, &val, FS_6144)){
			printf("err is %d\n", err);
		}

		printf("it reads %d milivolts \n", val);

	}
}
