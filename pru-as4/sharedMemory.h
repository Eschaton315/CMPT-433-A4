#ifndef _SHAREDMEMORY_H_
#define _SHAREDMEMORY_H_


#include <stdint.h>
#include <stdbool.h>

#define STR_LEN         8       // # LEDs in our string

// Define the struct with alignment

typedef struct {
    bool joystickDownPressed; //fire button
    bool terminate; //check to end program
    bool colorReady;
    char _pad;
    uint32_t neopixelColor[STR_LEN]; // ledcolor
} sharedMemStruct_t;

#endif
