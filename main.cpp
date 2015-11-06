#include "ADS1115-linux-user.h"
#include <stdio.h>

using namespace std;

#define MAX_BUS	64

int check = 0;

/*
int main(int argc, char *argv[]){

	int file;
	char writeBuf[3];
	char namebuf[MAX_BUS];
	int16_t value;
	int I2CAddress = 0x48;
	snprintf(namebuf, sizeof(namebuf), "/dev/i2c-1");
printf("begin\n");
		if ((file = open(namebuf, O_RDWR)) < 0){
			cout << "Failed to open ADS1115 ADC on " << namebuf << " I2C Bus" << endl;
			return(1);
		}
		if (ioctl(file, I2C_SLAVE, 0x90>>1) < 0){
			cout << "I2C_SLAVE address " << I2CAddress << " failed..." << endl;
			return(2);
		}

//write to config register

		writeBuf[0] = 1;
		writeBuf[1] = 0x40;//0100 0000
		writeBuf[2] = 0x83;//1000 0011
		if(write(file, writeBuf, 3) != 3) {
			cout << "Failure to write values to I2C Device address." << endl;
			return(3);
		}
		printf("test\n");

		while(1){
			//read from conversion register
			writeBuf[0] = 0;
			if(write(file, writeBuf, 1) != 1) {
				cout << "Failure to write values to I2C Device address." << endl;
				return(3);
			}

			if(read(file, writeBuf, 2) != 2) {
				cout << "Failure to write values to I2C Device address." << endl;
				return(3);
			}

			value = ((int16_t)writeBuf[0] << 8) | writeBuf[1];
			printf("%d\n", value);
			//printf("%d\t%d\n", writeBuf[0], writeBuf[1]);

		}


		close(file);
}*/

static ADS1115_config myconfig;

int main(){
	int err;
	ADS1115_ADC_Val val = 0;

	printf("here1\n");

	if(err =  ADS1115_init_and_open_i2c(&myconfig, 1, 0x90)){
		printf("err is %d\n", err);
	}

	printf("here2\n");

	if(err = ADS1115_writeReg(ADS1115_REG_CONFIGURATION, 0x4083, &myconfig, 0xffff)){
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

		printf("%d\n", val);
	}
}
