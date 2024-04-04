#include <stdint.h>
#include <stdbool.h>
#include <pru_cfg.h>
#include "resource_table_empty.h"
#include "../app/include/sharedDataStruct.h"

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