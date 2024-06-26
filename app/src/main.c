#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>



#include "sharedMemLinux.h"
#include "hal/sharedUtil.h"
#include "hal/segDisplay.h"
#include "hal/pwm.h"
#include "terminate.h"

/*
 Zen Cape's LEDS header:
                Pin 1: DIN (Data): left most pin; beside USB-micro connection, connects to P8.11
                Pin 2: GND (Ground): middle pin
                Pin 3: Unused (it's "5V external power", which is not powered normally on the BBG)
        - Connect NeoPixel "5VDC" to P9.7 or P9.8
*/



int main(){
	configBuzzer();
	InitializeI2C();
	sharedMem_init();
	//printf("cleanup14seg\n");
	CleanI2C();
	
    return 0;
}