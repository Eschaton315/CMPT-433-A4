
#include "hal/accelerometer.h"
#include "hal/sharedUtil.h"


#define I2C_DEVICE "/dev/i2c-1"  // Use the appropriate I2C device
#define ACCEL_ADDRESS 0x18       // Address of the LIS331DLH accelerometer
//#define ACCEL_ADDR_GREEN 0x1C

bool stopListen = false;
u_int32_t accelValue[3] = {0,0,0};
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
    pthread_create(&accelerometerThread, NULL, &accelerometerListener,NULL);

    return;
}

// Function to read acceleration values by opening the I2C addr
void readAccelerometer() {
    int file;
        // Open the I2C device
    if ((file = open(I2C_DEVICE, O_RDWR)) < 0) {
        printf("Failed to open the bus.\n");
        exit(1);
    }

    // Set the I2C slave address
    if (ioctl(file, I2C_SLAVE, ACCEL_ADDRESS) < 0) {
        printf("Failed to acquire bus access and/or talk to slave.\n");
        exit(1);
    }

    u_int8_t regAddr = 0x28; // Address of X-axis low byte register

    // Write the register address to initiate reading
    if (write(file, &regAddr, 1) != 1) {
        printf("Error writing to accelerometer\n");
        return;
    }

    // Read the X, Y, and Z values
    u_int8_t data[6];
    if (read(file, data, sizeof(data)) != sizeof(data)) {
        printf("Error reading from accelerometer\n");
        return;
    }

    // Combine the low and high bytes for each axis
    u_int8_t x = ((data[1] << 8) | data[0]);
    u_int8_t y = ((data[3] << 8) | data[2]);
    u_int8_t z = ((data[5] << 8) | data[4]);
    lock();
    accelValue[0] = x;
    accelValue[1] = y;
    accelValue[2] = z;
    unlock();

    printf("X: %d, Y: %d, Z: %d\n", x, y, z);

    for(int i =0;i<6;i++){
        printf("data %d = %u, ", i+1, data[i]);
    }
    printf("\n\n");

    close(file);
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
    return;
}


void accelerometer_getValues(int* x, int* y, int* z){
    lock();
    *x = accelValue[0];
   *y = accelValue[1];  
   *z = accelValue[2];      
    unlock();
}



