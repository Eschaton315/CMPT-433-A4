#ifndef _SHAREDMEMORY_H_
#define _SHAREDMEMORY_H_

#include <stdbool.h>
#include <stdint.h>

#define STR_LEN         8       // # LEDs in our string

typedef struct {
    uint32_t NeopixelColor[STR_LEN];
    bool joystickRightPressed;
    bool joystickDownPressed;
} sharedMemStruct_t;

#endif
