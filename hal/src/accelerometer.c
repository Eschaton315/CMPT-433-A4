
#include "hal/accelerometer.h"
#include "hal/sharedUtil.h"


#define I2C_DEVICE "/dev/i2c-1"  // Use the appropriate I2C device
#define ACCEL_ADDRESS 0x18       // Address of the LIS331DLH accelerometer
#define STATUS_ADDRESS 0x27
#define CTRL_REG1_ADDRESS 0x20
#define MAX_ACCEL 17
#define MAX_CONVERTED 10
//#define ACCEL_ADDR_GREEN 0x1C

bool stopListen = false;
int file;
int accelValue[3] = {0,0,0};
pthread_t accelerometerThread;

static pthread_mutex_t accelerometerMutex = PTHREAD_MUTEX_INITIALIZER;

static void lock(){
    pthread_mutex_lock(&accelerometerMutex);
}
static void unlock(){
    pthread_mutex_unlock(&accelerometerMutex);
}


static void *accelerometerListener();

void accelerometer_init(){
    printf("initializing acc\n");
    runCommand("config-pin p9.17 i2c");
    runCommand("config-pin p9.18 i2c");

     if ((file = open(I2C_DEVICE, O_RDWR)) < 0) {
        printf("Failed to open the bus.\n");
        close(file);
        exit(1);
    }

    // Set the I2C slave address
    if (ioctl(file, I2C_SLAVE, ACCEL_ADDRESS) < 0) {
        printf("Failed to acquire bus access and/or talk to slave.\n");
        close(file);
        exit(1);
    }
    unsigned char buffer[2];

    buffer[0] = CTRL_REG1_ADDRESS;
    buffer[1] = 0x27;
    write (file,buffer,sizeof(buffer));

    if (write(file, buffer, sizeof(buffer)) != sizeof(buffer)) {
        perror("Failed to write to the I2C device");
        close(file);
        exit(1);
    }


    pthread_create(&accelerometerThread, NULL, &accelerometerListener,NULL);

    return;
}

// Function to read acceleration values by opening the I2C addr
void readAccelerometer() {
    
    //u_int8_t configReg1 = 0x27;  // Configure register 1 (CTRL_REG1)
    //u_int8_t configReg4 = 0x80;  // Configure register 4 (CTRL_REG4) for 8g measurement range
        // Open the I2C device
   
/*
        // Configure control register 1 (CTRL_REG1) to enable X, Y, Z axes and set output data rate (ODR)
    if (write(file, &configReg1, 1) != 1) {
        perror("Failed to write to the I2C device");
        close(file);
        exit(1);
    }

    // Configure control register 4 (CTRL_REG4) to set measurement range (8g in this case)
    if (write(file, &configReg4, 1) != 1) {
        perror("Failed to write to the I2C device");
        close(file);
        exit(1);
    }
    */


   u_int8_t data[7];
   data [0] = STATUS_ADDRESS;
   u_int8_t addr = 0x28;
   u_int8_t buffer[6];

   for(int i =0 ; i<6; i++){
       if (write(file, &addr, 1) != 1) {
        printf("Error writing to accelerometer\n");
        return;
        }

        if (read(file, data, 1) != 1) {
        printf("Error reading from accelerometer\n");
        return;
        }

        buffer[i] = data[0];
        addr++; 

   }

    // Write the register address to initiate reading
    if (write(file, data, 1) != 1) {
        printf("Error writing to accelerometer\n");
        return;
    }

    // Read the X, Y, and Z values
    
    if (read(file, data, sizeof(data)) != sizeof(data)) {
        printf("Error reading from accelerometer\n");
        return;
    } 

    // Combine the low and high bytes for each axis
    int16_t x = ((buffer[1] << 8) | buffer[0]);
    int16_t y = ((buffer[3] << 8) | buffer[2]);
    int16_t z = ((buffer[5] << 8) | buffer[4]);
    lock();
    accelValue[0] = x/1000;
    accelValue[1] = y/1000;
    accelValue[2] = z/1000;
	accelValue[0] = accelerometer_value_convert(accelValue[0]);
	accelValue[1] = accelerometer_value_convert(accelValue[1]);
	accelValue[2] = accelerometer_value_convert(accelValue[2]);
    unlock();

    //printf("X: %d, Y: %d, Z: %d\n", x, y, z);

    /*
    for(int i =0;i<6;i++){
        printf("data %d = 0x%02X, ", i+1, buffer[i]);
    }
    printf("\n\n");
    */

   
}

//acclelerometer thread that runs until the cleanup function is called.
static void *accelerometerListener(){
    while(!stopListen){
        readAccelerometer();
    }
    return NULL;
}

void accelerometer_cleanup(){
    stopListen = true;
    pthread_join(accelerometerThread, NULL);
    close(file);
    return;
}


int accelerometer_value_convert(int value){
    float newValue = (float)value * MAX_CONVERTED / MAX_ACCEL;    
    value = (int)newValue;
    return value;
	
}

void accelerometer_getValues(int* x, int* y, int* z){
    lock();
    *x = accelValue[0];
   *y = accelValue[1];  
   *z = accelValue[2];      
    unlock();
}



