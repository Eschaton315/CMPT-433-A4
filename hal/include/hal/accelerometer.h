#ifndef _ACC_H_
#define _ACC_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>
#include <pthread.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>


//Initialize the accelerometer
void accelerometer_init();

//Clean up the accelerometer
void accelerometer_cleanup();

//Gets current value of the accelerometer
void accelerometer_getValues(int *x, int *y, int *z);






#endif