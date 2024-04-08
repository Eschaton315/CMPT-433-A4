#ifndef _SEGDISPNEW_H_
#define _SEGDISPNEW_H_

int initI2cBus(char* bus, int address);
void writeI2cReg(int i2cFileDesc, unsigned char regAddr, unsigned char value);
unsigned char readI2cReg(int i2cFileDesc, unsigned char regAddr);
void InitializeI2C();
void WriteNewNumberI2C(int value);
void CleanI2C();
#endif