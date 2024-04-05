#include <stdint.h>
#include <pru_cfg.h>

#include "sharedMemory.h"
#include "resource_table_empty.h"


// GPIO Configuration
// ----------------------------------------------------------
volatile register uint32_t __R30;   // output GPIO register
volatile register uint32_t __R31;   // input GPIO register


// GPIO Input: P8_15 = pru0_pru_r31_15 
//   = JSRT (Joystick Right) on Zen Cape
// GPIO Input: P8_16 = pru0_pru_r31_14 
//   = JSDN (Jpystick Down)
#define JOYSTICK_RIGHT_MASK (1 << 15)
#define JOYSTICK_DOWN_MASK (1 << 14)

// # LEDs in our string
#define STR_LEN         8
#define oneCyclesOn     700/5   // Stay on 700ns
#define oneCyclesOff    800/5
#define zeroCyclesOn    350/5
#define zeroCyclesOff   600/5
#define resetCycles     60000/5 // Must be at least 50u, use 60u

// P8_11 for output (on R30), PRU0
#define DATA_PIN 15       // Bit number to output on

//Shared Memory Configuration
#define THIS_PRU_DRAM 0x00000
#define OFFSET 0x200
#define THIS_PRU_DRAM_USABLE (THIS_PRU_DRAM + OFFSET)

volatile sharedMemStruct_t *pSharedMemStruct = (volatile void *)THIS_PRU_DRAM_USABLE;

static void setLED(){
    __delay_cycles(resetCycles);

    for(int j = 0; j < STR_LEN; j++) {
        for(int i = 31; i >= 0; i--) {
            if(pSharedMemStruct->neopixelColor[j] & ((uint32_t)0x1 << i)) {
                __R30 |= 0x1<<DATA_PIN;      // Set the GPIO pin to 1
                __delay_cycles(oneCyclesOn-1);
                __R30 &= ~(0x1<<DATA_PIN);   // Clear the GPIO pin
                __delay_cycles(oneCyclesOff-2);
            } else {
                __R30 |= 0x1<<DATA_PIN;      // Set the GPIO pin to 1
                __delay_cycles(zeroCyclesOn-1);
                __R30 &= ~(0x1<<DATA_PIN);   // Clear the GPIO pin
                __delay_cycles(zeroCyclesOff-2);
            }
        }
    }

        __R30 &= ~(0x1<<DATA_PIN);   // Clear the GPIO pin
}

static void checkJoystick(){
    if (!(__R31 & JOYSTICK_RIGHT_MASK)) {
        pSharedMemStruct->terminate = 1;
    }else{
        pSharedMemStruct->terminate = 0;
    }
    if (!(__R31 & JOYSTICK_DOWN_MASK)){
        pSharedMemStruct->joystickDownPressed = 1;
    }else{
        pSharedMemStruct->joystickDownPressed = 0;
    }
}

void main(void){
    //initalize the struct
    for(int i = 0; i < STR_LEN; i++){
        pSharedMemStruct->neopixelColor[i] = 0x0f0f0000; //ylw
        
    }
    pSharedMemStruct->joystickDownPressed = false;  
    pSharedMemStruct->terminate = false;
    pSharedMemStruct->colorReady = true;
    while(!pSharedMemStruct->terminate){
        checkJoystick();
        if(pSharedMemStruct->colorReady){
            pSharedMemStruct->colorReady = false;
            setLED();

        } 
    }

    //shut down led set
    
        for(int i = 0; i < STR_LEN; i++){
            pSharedMemStruct->neopixelColor[i] = 0x00000000; //clear
        }
            pSharedMemStruct->colorReady = false;
            setLED();

}
