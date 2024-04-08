#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

#include "hal/segDisplay.h"
#include "hal/sharedUtil.h"

#define I2CDRV_LINUX_BUS0 "/dev/i2c-0"
#define I2CDRV_LINUX_BUS1 "/dev/i2c-1"
#define I2CDRV_LINUX_BUS2 "/dev/i2c-2"


#define GPIO_CONFIG_1 "config-pin P9_18 i2c"
#define GPIO_CONFIG_2 "config-pin P9_17 i2c"

#define GPIO1 "/sys/class/gpio/gpio61/value"
#define GPIO2 "/sys/class/gpio/gpio44/value"

#define GPIO_EXPORT "/sys/class/gpio/export"
#define GPIO_EXPORT_1NUM "61"
#define GPIO_EXPORT_2NUM "44"
#define GPIO_DIRECTION_1 "/sys/class/gpio/gpio61/direction"
#define GPIO_DIRECTION_2 "/sys/class/gpio/gpio44/direction"
#define GPIO_DIR_OUT "out"

#define I2C_DEVICE_ADDRESS 0x20

#define REG_DIRA 0x02
#define REG_DIRB 0x03
#define REG_OUTA 0x00
#define REG_OUTB 0x01

#define NUM0_PT1 0xD0
#define NUM0_PT2 0xA1

#define NUM1_PT1 0x02
#define NUM1_PT2 0x08

#define NUM2_PT1 0x98
#define NUM2_PT2 0x83

#define NUM3_PT1 0xD8
#define NUM3_PT2 0x03

#define NUM4_PT1 0xC8
#define NUM4_PT2 0x22

#define NUM5_PT1 0x58
#define NUM5_PT2 0x23

#define NUM6_PT1 0x58
#define NUM6_PT2 0xA3

#define NUM7_PT1 0xC0
#define NUM7_PT2 0x01

#define NUM8_PT1 0xD8
#define NUM8_PT2 0xA3

#define NUM9_PT1 0xC8
#define NUM9_PT2 0x23


static int i2cFileDesc;



int initI2cBus(char* bus, int address)
{
	runCommand(GPIO_CONFIG_1);
	runCommand(GPIO_CONFIG_2);
	writeToFile( GPIO_EXPORT, GPIO_EXPORT_1NUM);
writeToFile( GPIO_EXPORT, GPIO_EXPORT_2NUM);
writeToFile(GPIO_DIRECTION_1 , GPIO_DIR_OUT );
writeToFile(GPIO_DIRECTION_2, GPIO_DIR_OUT );
	int i2cFileDesc = open(bus, O_RDWR);
	if (i2cFileDesc < 0) {
		printf("I2C DRV: Unable to open bus for read/write (%s)\n", bus);
		perror("Error is:");
		exit(-1);
	}

	int result = ioctl(i2cFileDesc, I2C_SLAVE, address);
	if (result < 0) {
		perror("Unable to set I2C device to slave address.");
		exit(-1);
	}
	return i2cFileDesc;
}


void writeI2cReg(int i2cFileDesc, unsigned char regAddr, unsigned char value)
{
	unsigned char buff[2];
	buff[0] = regAddr;
	buff[1] = value;
	int res = write(i2cFileDesc, buff, 2);
	if (res != 2) {
		perror("Unable to write i2c register");
		exit(-1);
	}
}

unsigned char readI2cReg(int i2cFileDesc, unsigned char regAddr)
{
	// To read a register, must first write the address
	int res = write(i2cFileDesc, &regAddr, sizeof(regAddr));
	if (res != sizeof(regAddr)) {
		perror("Unable to write i2c register.");
		exit(-1);
	}

	// Now read the value and return it
	char value = 0;
	res = read(i2cFileDesc, &value, sizeof(value));
	if (res != sizeof(value)) {
		perror("Unable to read i2c register");
		exit(-1);
	}
	return value;
}

void InitializeI2C(){
	i2cFileDesc = initI2cBus(I2CDRV_LINUX_BUS1, I2C_DEVICE_ADDRESS);
	writeI2cReg(i2cFileDesc, REG_DIRA, 0x00);
	writeI2cReg(i2cFileDesc, REG_DIRB, 0x00);	
	writeI2cReg(i2cFileDesc, REG_OUTA, NUM0_PT1);
	writeI2cReg(i2cFileDesc, REG_OUTB, NUM0_PT2);
}

void WriteNewNumberI2C(int value){

    writeToFile(GPIO2, "0");
	writeToFile(GPIO2, "1");
	switch (value){
		case 0:
			writeI2cReg(i2cFileDesc, REG_OUTA, NUM0_PT1);
			writeI2cReg(i2cFileDesc, REG_OUTB, NUM0_PT2);
			break;
		
		case 1:
			writeI2cReg(i2cFileDesc, REG_OUTA, NUM1_PT1);
			writeI2cReg(i2cFileDesc, REG_OUTB, NUM1_PT2);
			break;
			
		case 2:
			writeI2cReg(i2cFileDesc, REG_OUTA, NUM2_PT1);
			writeI2cReg(i2cFileDesc, REG_OUTB, NUM2_PT2);
			break;
			
		case 3:
			writeI2cReg(i2cFileDesc, REG_OUTA, NUM3_PT1);
			writeI2cReg(i2cFileDesc, REG_OUTB, NUM3_PT2);
			break;
			
		case 4:
			writeI2cReg(i2cFileDesc, REG_OUTA, NUM4_PT1);
			writeI2cReg(i2cFileDesc, REG_OUTB, NUM4_PT2);
			break;
			
		case 5:
			writeI2cReg(i2cFileDesc, REG_OUTA, NUM5_PT1);
			writeI2cReg(i2cFileDesc, REG_OUTB, NUM5_PT2);
			break;
			
		case 6:
			writeI2cReg(i2cFileDesc, REG_OUTA, NUM6_PT1);
			writeI2cReg(i2cFileDesc, REG_OUTB, NUM6_PT2);
			break;
			
		case 7:
			writeI2cReg(i2cFileDesc, REG_OUTA, NUM7_PT1);
			writeI2cReg(i2cFileDesc, REG_OUTB, NUM7_PT2);
			
			break;
			
		case 8:
			writeI2cReg(i2cFileDesc, REG_OUTA, NUM8_PT1);
			writeI2cReg(i2cFileDesc, REG_OUTB, NUM8_PT2);
			break;
	
		default:
			writeI2cReg(i2cFileDesc, REG_OUTA, NUM9_PT1);
			writeI2cReg(i2cFileDesc, REG_OUTB, NUM9_PT2);
			break;
	}
	
}

void CleanI2C(){
	writeI2cReg(i2cFileDesc, REG_DIRA, 0x00);
    writeI2cReg(i2cFileDesc, REG_DIRB, 0x00);
	writeToFile(GPIO1, "0");
    writeToFile(GPIO2, "0");
	close(i2cFileDesc);
}