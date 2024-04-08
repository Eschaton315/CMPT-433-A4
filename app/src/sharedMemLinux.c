
#include "sharedMemLinux.h"


#define LED_OFF 0x00000000
#define LED_RED 0x000f0000
#define LED_GREEN 0x0f000000
#define LED_BLUE 0x00000f00
#define LED_RED_BRIGHT 0x008f0000
#define LED_GREEN_BRIGHT 0x8f000000
#define LED_BLUE_BRIGHT 0x00008f00
#define LED_YELLOW 0x0f0f0000
#define LED_PURPLE 0x000f0f00

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
    bool missHold = false;
    int missTimer = 0;
    int score = 0;
    int xTarget = 0;
    int yTarget = 0;
    int xRaw,yRaw,zRaw;
    int yOffset=0;
    u_int32_t colorBright;
    u_int32_t colorDim;
    u_int32_t colorFire;

    accelerometer_init();
    WriteNewNumberI2C(score);
    //main loop until joystick prompts a terminate
    while(!pSharedPru0->terminate){
        if(missTimer<5){
            missTimer++;
        }else{
            missTimer = 0;
        }
        
        accelerometer_getValues(&xRaw,&yRaw,&zRaw);

        //If there is no active dot, generate a new one 
        if(!newDotGenerated&&!joystickHold){
            int dotRange = 5 - (-5)+1;
            srand(time(NULL));
            xTarget = (rand() % dotRange) + (-5);
            yTarget = (rand() % dotRange) + (-5);
           newDotGenerated = true;
        }else{
        // printf("X: %d, Y: %d\n", xRaw, yRaw);
            if(pSharedPru0->joystickDownPressed){
                if(!joystickHold){
                        joystickHold = true;
                        //printf("FIRE!\n");
                    if(xTarget == xRaw && yTarget == yRaw){
                        printf("HIT!\n");
                        colorFire = LED_YELLOW;
                        BuzzerHitThreadCreate();
                        newDotGenerated = false;
                        if(score<99){
                            score++;
                        }else{
                            score = 0;
                        }

                        //set digit 
                        WriteNewNumberI2C(score);
                        //update14seg(score);
                        //playHitSound();
                    }else{
                        printf("MISS\n");
                        colorFire = LED_PURPLE;
                        //BuzzerMissThreadCreate();
                        missTimer = 0;
                        missHold = true;
                        //playmissSound();
                    }                
                    //implement hit effect or effect
                }
                //if miss keep replaying miss noise
                for(int i = 0; i < STR_LEN; i++){
                    pSharedPru0->neopixelColor[i] = colorFire;
                }                
                if(missHold&&missTimer == 0){
                    BuzzerMissThreadCreate();
                }

            }else{

                //if fire button is not pressed, display color on led via pru 
                joystickHold = false;
                missHold = false;
                
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
                //create offset value to use for a switch case
                if(yTarget>0){
                    yOffset = yTarget - yRaw;  
                }else{
                    yOffset = yRaw - yTarget;
                }

                //printf("yRaw = %d,yTarget = %d, yOffset = %d\n",yRaw,yTarget,yOffset);
                //printf("xRaw = %d,xTarget = %d\n",xRaw,xTarget);
                
                //depending on offset, show different led color and position
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
                //if off my  a significant amount, at least display the info for x axis
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
            }

        }
        pSharedPru0->colorReady=true;
        //update every 100 ms
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

    printf("==>GAME START<==\n");
    pruApp();

    //cleanup
    printf("Terminating\n");
    BuzzerHitThreadJoin();
    BuzzerMissThreadJoin();
    freePruMmapAddr(pPruBase);
    return;
}