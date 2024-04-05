
#include "sharedMemLinux.h"



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

void sharedMem_init(){
    bool alter = true;
    printf("Sharing memory with PRU\n");
    volatile void *pPruBase = getPruMmapAddr();
    pSharedPru0 = PRU0_MEM_FROM_BASE(pPruBase);
    pSharedPru0->terminate = false;

    //Initialize GPIO 

    runCommand("config-pin P8.11 pruout");
    runCommand("config-pin p8_15 pruin");
    runCommand("config-pin p8.16 pruin");
    printf("Terminate = %d\n",pSharedPru0->terminate);
   
    while(!pSharedPru0->terminate){
         printf("fire = %d\n",pSharedPru0->joystickDownPressed);

         if(pSharedPru0->joystickDownPressed){
            for(int i = 0; i < STR_LEN; i++){
            pSharedPru0->neopixelColor[i] = 0x000f0f00; // purp
        } 

         }else{
        //do stuff
        if(alter){

        for(int i = 0; i < STR_LEN; i++){
            pSharedPru0->neopixelColor[i] = 0x0f000000; //blue
        }  
        alter = false;
    }else{
        alter = true;
        for(int i = 0; i < STR_LEN; i++){
            if(i%2==0){
                pSharedPru0->neopixelColor[i] = 0x00000f00;//green?
            }else{
             pSharedPru0->neopixelColor[i] = 0x000f0000; //red
            }
            
        }  
        
    }
    }
    pSharedPru0->colorReady=true;

        sleepForMs(500);
    }

    //cleanup
    printf("Terminating\n");
        for(int i = 0; i < STR_LEN; i++){
            pSharedPru0->neopixelColor[i] = 0x00000000; //clear
        }
    pSharedPru0->colorReady=true;  
    sleepForMs(1000);
    freePruMmapAddr(pPruBase);
    return;
}