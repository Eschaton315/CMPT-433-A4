
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
    //int side = 0;
    int xTarget = 0;
    int yTarget = 0;
    int xRaw,yRaw,zRaw;
    int yOffset=0;
    u_int32_t colorBright;
    u_int32_t colorDim;
   // float x, y;
    accelerometer_init();
    //int colorDim;
    //int colorBright;

    while(!pSharedPru0->terminate){

        accelerometer_getValues(&xRaw,&yRaw,&zRaw);

        if(!newDotGenerated&&!joystickHold){
            int dotRange = 5 - (-5)+1;
            srand(time(NULL));
            xTarget = (rand() % dotRange) + (-5);
            yTarget = (rand() % dotRange) + (-5);
           //set new Dot
           newDotGenerated = true;
        }
        
      // printf("X: %d, Y: %d\n", xRaw, yRaw);

        if(pSharedPru0->joystickDownPressed){
            if(!joystickHold){
                joystickHold = true;
                //printf("FIRE!\n");
            if(xTarget == xRaw && yTarget == yRaw){
                printf("HIT!\n");
                newDotGenerated = false;
            }else{
                printf("MISS\n");
            }                
            //implement hit effect or effect
                for(int i = 0; i < STR_LEN; i++){
                    pSharedPru0->neopixelColor[i] = 0x000f0f00; // purp
                }
            }
 

        
    
        }else{
            joystickHold = false;
            
            //set color depending on x value;
            if(xRaw>xTarget){
                //need to tilt more left
                colorDim = LED_RED;
                colorBright = LED_RED_BRIGHT;
            }else if(xRaw<xTarget){
                //need ro tilt more right
                colorDim = LED_GREEN;
                colorBright = LED_GREEN_BRIGHT;
            }else{
                //correct postition
                colorDim = LED_BLUE;
                colorBright = LED_BLUE_BRIGHT;
            }

            //set postion depending on y value;

            if(yTarget>0){
                yOffset = yTarget - yRaw;  //if y is 5 and raw is 7 tilt down by 2
            }else{
                yOffset = yRaw - yTarget;
            }
              //if y is -5 and raw is 7 tilt down by 12
            printf("yRaw = %d,yTarget = %d, yOffset = %d\n",yRaw,yTarget,yOffset);
            printf("xRaw = %d,xTarget = %d\n",xRaw,xTarget);
            switch (yOffset)
            {
            case -4:
                for(int i = 7; i > 1; i--){
                    pSharedPru0->neopixelColor[i] = LED_OFF;
                }            
                pSharedPru0->neopixelColor[1] = colorDim;
                pSharedPru0->neopixelColor[0] = colorBright;                
                break;
            case -3:
                for(int i = 7; i > 2; i--){
                    pSharedPru0->neopixelColor[i] = LED_OFF;
                }            
                pSharedPru0->neopixelColor[2] = colorDim;
                pSharedPru0->neopixelColor[1] = colorBright;
                pSharedPru0->neopixelColor[0] = colorDim;
                break;
            case -2:
                for(int i = 7; i > 3; i--){
                    pSharedPru0->neopixelColor[i] = LED_OFF;
                }            
                pSharedPru0->neopixelColor[3] = colorDim;
                pSharedPru0->neopixelColor[2] = colorBright;
                pSharedPru0->neopixelColor[1] = colorDim;
                pSharedPru0->neopixelColor[0] = LED_OFF;                /* code */
                break;
            case -1:
                for(int i = 7; i > 4; i--){
                    pSharedPru0->neopixelColor[i] = LED_OFF;
                }            
                pSharedPru0->neopixelColor[4] = colorDim;
                pSharedPru0->neopixelColor[3] = colorBright;
                pSharedPru0->neopixelColor[2] = colorDim;
                for(int i = 0; i < 2; i++){
                    pSharedPru0->neopixelColor[i] = LED_OFF;
                }
                break;
            case 0:
                for(int i = 0; i < 8; i++){
                    pSharedPru0->neopixelColor[i] = colorBright;
                } 
                break;
            case 1:
                pSharedPru0->neopixelColor[7] = LED_OFF;
                pSharedPru0->neopixelColor[6] = colorDim;
                pSharedPru0->neopixelColor[5] = colorBright;
                pSharedPru0->neopixelColor[4] = colorDim;
                for(int i = 0; i < 4; i++){
                    pSharedPru0->neopixelColor[i] = LED_OFF;
                }
                break;
            case 2:
                pSharedPru0->neopixelColor[7] = colorDim;
                pSharedPru0->neopixelColor[6] = colorBright;
                pSharedPru0->neopixelColor[5] = colorDim;
                for(int i = 0; i < 5; i++){
                    pSharedPru0->neopixelColor[i] = LED_OFF;
                }
                break;
            case 3:
                pSharedPru0->neopixelColor[7] = colorBright;
                pSharedPru0->neopixelColor[6] = colorDim;
                for(int i = 0; i < 6; i++){
                    pSharedPru0->neopixelColor[i] = LED_OFF;
                }
                break;
            case 4:
                pSharedPru0->neopixelColor[7] = colorDim;
                for(int i = 0; i < 7; i++){
                    pSharedPru0->neopixelColor[i] = LED_OFF;
                }
                break;            
            default:
                if(yOffset<0){

                    for(int i = 7; i > 0; i--){
                        pSharedPru0->neopixelColor[i] = LED_OFF;
                    }            
                    pSharedPru0->neopixelColor[0] = colorDim;
                }else{
                    pSharedPru0->neopixelColor[7] = colorDim;
                    for(int i = 0; i < 7; i++){
                        pSharedPru0->neopixelColor[i] = LED_OFF;
                    }
                }
                break;
            }







            //temp program to check that the neopixel works
            /*
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
            */
        }
        pSharedPru0->colorReady=true;
        sleepForMs(100);
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