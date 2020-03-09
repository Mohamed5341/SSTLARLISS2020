#define MPU_ADDRESS 0x68<<1 //address of device is 0x68 -> 7 bit address 0x68<<1 -> 8 bit address 
#define WHO_AM_I 0x75 //this register contains 0x68 value

#include "mbed.h"


//define i2c connection
I2C i2c(I2C_SDA,I2C_SCL);

// main() runs in its own thread in the OS
int main()
{
    char add[1];
    //need to define register address in array 
    //and it is character because other data types will give another results
    char subaddress[1];
    subaddress[0]=WHO_AM_I;

    while (true) {
        add[0]=0;
        
        //send to mpu register number
        i2c.write(MPU_ADDRESS, subaddress, 1,1);

        //read outcome form mpu
        i2c.read(MPU_ADDRESS, add, 1, 0);
       
        printf("Device Address is : 0x%x\n", add[0]);

        wait(2);
    }
}

