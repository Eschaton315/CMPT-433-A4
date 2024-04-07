
#include "sharedMemLinux.h"

#define LED_OFF 0x00000000
#define LED_RED 0x000f0000
#define LED_GREEN 0x0f000000
#define LED_BLUE 0x00000f00
#define LED_RED_BRIGHT 0x008f0000
#define LED_GREEN_BRIGHT 0x8f000000
#define LED_BLUE_BRIGHT 0x00008f00


//General PRU Memory Sharing Routine from examplecode from course page

volatile sharedMemStruct_t *pSharedPru0;

// Return the address of the PRU's base memory
volatile void* getPruMmapAddr(void)
{
    int fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (fd == -1) {
        perror("ERROR: could not open /dev/mem");
        exit(EXIT_FAILURE);
    }

    // Points to start of PRU memory.
    volatile void* pPruBase = mmap(0, PRU_LEN, PROT_READ | PROT_WRITE, MAP_SHARED, fd, PRU_ADDR);
    if (pPruBase == MAP_FAILED) {
        perror("ERROR: could not map memory");
        exit(EXIT_FAILURE);
    }
    close(fd);

    return pPruBase;
}

void freePruMmapAddr(volatile void* pPruBase)
{
    if (munmap((void*) pPruBase, PRU_LEN)) {
        perror("PRU munmap failed");
        exit(EXIT_FAILURE);
    }
}

static void pruApp(){
    bool newDotGenerated = false;
    bool joystickHold = false;
    int side = 0;
    int x,y,z;
    accelerometer_init();
    //int colorDim;
    //int colorBright;

    while(!pSharedPru0->terminate){

        accelerometer_getValues(&x,&y,&z);

        if(!newDotGenerated){
           //set new Dot
           newDotGenerated = true;
        }
        
       //printf("X: %d, Y: %d, Z: %d\n", x, y, z);

        if(pSharedPru0->joystickDownPressed){
            if(!joystickHold){
                joystickHold = true;
                printf("FIRE!\n");
            //implement hit effect or effect
                for(int i = 0; i < STR_LEN; i++){
                    pSharedPru0->neopixelColor[i] = 0x000f0f00; // purp
                }
            }
 
        //if hit
        newDotGenerated = false;
    
        }else{
            joystickHold = false;
            //temp program to check that the neopixel works

            switch (side)
            {
            case 0:
                pSharedPru0->neopixelColor[7] = 0x00000f00;
                for(int i = 0; i < 7; i++){
                    pSharedPru0->neopixelColor[i] = LED_OFF;
                }
                side ++;                 
                break;
            
            case 1:
                pSharedPru0->neopixelColor[7] = 0x000f0000;
                pSharedPru0->neopixelColor[6] = 0x00000f00;
                for(int i = 0; i < 6; i++){
                    pSharedPru0->neopixelColor[i] = LED_OFF;
                }
                side ++; 
                break;
            case 2:
                pSharedPru0->neopixelColor[7] = 0x00000f00;
                pSharedPru0->neopixelColor[6] = 0x000f0000;
                pSharedPru0->neopixelColor[5] = 0x00000f00;
                for(int i = 0; i < 5; i++){
                    pSharedPru0->neopixelColor[i] = LED_OFF;
                }
                side ++; 
                break;
            case 3:
                pSharedPru0->neopixelColor[7] = 0x00000000;
                pSharedPru0->neopixelColor[6] = 0x00000f00;
                pSharedPru0->neopixelColor[5] = 0x000f0000;
                pSharedPru0->neopixelColor[4] = 0x00000f00;
                for(int i = 0; i < 4; i++){
                    pSharedPru0->neopixelColor[i] = LED_OFF;
                }
                side ++;
                break;
            case 4:
                for(int i = 7; i > 5; i--){
                    pSharedPru0->neopixelColor[i] = LED_OFF;
                }            
                pSharedPru0->neopixelColor[5] = 0x00000f00;
                pSharedPru0->neopixelColor[4] = 0x000f0000;
                pSharedPru0->neopixelColor[3] = 0x00000f00;
                for(int i = 0; i < 3; i++){
                    pSharedPru0->neopixelColor[i] = LED_OFF;
                }
                side ++; 
                break;
            case 5:
                for(int i = 7; i > 4; i--){
                    pSharedPru0->neopixelColor[i] = LED_OFF;
                }            
                pSharedPru0->neopixelColor[4] = 0x00000f00;
                pSharedPru0->neopixelColor[3] = 0x000f0000;
                pSharedPru0->neopixelColor[2] = 0x00000f00;
                for(int i = 0; i < 2; i++){
                    pSharedPru0->neopixelColor[i] = LED_OFF;
                }
                side ++; 
                break;
            case 6:
                for(int i = 7; i > 3; i--){
                    pSharedPru0->neopixelColor[i] = LED_OFF;
                }            
                pSharedPru0->neopixelColor[3] = 0x00000f00;
                pSharedPru0->neopixelColor[2] = 0x000f0000;
                pSharedPru0->neopixelColor[1] = 0x00000f00;
                pSharedPru0->neopixelColor[0] = LED_OFF;
                side ++;   
                break;
            case 7:
                for(int i = 7; i > 2; i--){
                    pSharedPru0->neopixelColor[i] = LED_OFF;
                }            
                pSharedPru0->neopixelColor[2] = 0x00000f00;
                pSharedPru0->neopixelColor[1] = 0x000f0000;
                pSharedPru0->neopixelColor[0] = 0x00000f00;
                side ++; 
                break;
            case 8:
                for(int i = 7; i > 1; i--){
                    pSharedPru0->neopixelColor[i] = LED_OFF;
                }            
                pSharedPru0->neopixelColor[1] = 0x00000f00;
                pSharedPru0->neopixelColor[0] = 0x000f0000;
                side ++; 
                break;
            case 9:
                for(int i = 7; i > 0; i--){
                    pSharedPru0->neopixelColor[i] = LED_OFF;
                }            
                pSharedPru0->neopixelColor[0] = 0x00000f00;
                side = 0;
                break;            
            
            }
        }
        pSharedPru0->colorReady=true;
        sleepForMs(200);
    }


}

void sharedMem_init(){
    
    printf("Sharing memory with PRU\n");
    volatile void *pPruBase = getPruMmapAddr();
    pSharedPru0 = PRU0_MEM_FROM_BASE(pPruBase);
    pSharedPru0->terminate = false;

    //Initialize GPIO 

    runCommand("config-pin P8.11 pruout");
    runCommand("config-pin p8_15 pruin");
    runCommand("config-pin p8.16 pruin");

    pruApp();

    //cleanup
    printf("Terminating\n");
    freePruMmapAddr(pPruBase);
    return;
}