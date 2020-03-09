#include "mbed.h"

#define MPU_ADDRESS 0x68<<1

#define ACC_XOUT_H 0x3B
#define ACC_XOUT_L 0x3C
#define ACC_YOUT_H 0x3D
#define ACC_YOUT_L 0x3E
#define ACC_ZOUT_H 0x3F
#define ACC_ZOUT_L 0x40
#define TEMP_OUT_H 0x41
#define TEMP_OUT_L 0x42
#define GYRO_XOUT_H 0x43
#define GYRO_XOUT_L 0x44
#define GYRO_YOUT_H 0x45
#define GYRO_YOUT_L 0x46
#define GYRO_ZOUT_H 0x47
#define GYRO_ZOUT_L 0x48

#define PWR_MGMT_1 0x6B
#define CONFIG 0x1A
#define SMPLRT_DIV 0x19
#define GYRO_CONFIG 0x1B
#define ACCL_CONFIG 0x1C


I2C i2c(p28,p27);


void init(void);//initialize function
void write_byte(uint8_t address, uint8_t reg_address, uint8_t data);//write byte to register using I2C
char read_byte(uint8_t address, uint8_t reg_address);//read byte from register
void read_bytes(uint8_t address, uint8_t reg_address, uint8_t num, uint8_t *out);//read nuber of bytes

int main()
{
    uint8_t data[14];
    int16_t acc_x,acc_y,acc_z,t,gyro_x,gyro_y,gyro_z;
    
    // first you need to initialize to get output data
    init();

    while (true) {
        //read all data once
        read_bytes(MPU_ADDRESS, ACC_XOUT_H, 14, &data[0]);


        acc_x=(int16_t)(((int16_t)data[0]<<8)|data[1]);
        acc_y=(int16_t)(((int16_t)data[2]<<8)|data[3]);
        acc_z=(int16_t)(((int16_t)data[4]<<8)|data[5]);
        t=(int16_t)(((int16_t)data[6]<<8)|data[7]);
        gyro_x=(int16_t)(((int16_t)data[8]<<8)|data[9]);
        gyro_y=(int16_t)(((int16_t)data[10]<<8)|data[11]);
        gyro_z=(int16_t)(((int16_t)data[12]<<8)|data[13]);


        printf("ACC X = %d ACC Y = %d ACC Z = %d ",acc_x,acc_y,acc_z);
        printf("Temperature = %d ",t);
        printf("GYRO X = %d GYRO Y = %d GYRO Z = %d\n",gyro_x,gyro_y,gyro_z);

        thread_sleep_for(1000);
    }
}

void init(){
    // set I2C frequency to 400Khz
    i2c.frequency(400000);

    // write 0 ate sleep bit to prevent device from getting into sleep mode
    write_byte(MPU_ADDRESS, PWR_MGMT_1, 0x00);
    wait_ms(100);// wait till it stabilize

    //select clock source to be PLL with X
    write_byte(MPU_ADDRESS, PWR_MGMT_1, 0x01);

    // configure Low Pass Filter
    write_byte(MPU_ADDRESS, CONFIG, 0x03);

    // specify sample rate
    write_byte(MPU_ADDRESS, SMPLRT_DIV, 0x04);
    
    // configure Accelerometer
    uint8_t temp=read_byte(MPU_ADDRESS, ACCL_CONFIG);
    write_byte(MPU_ADDRESS, ACCL_CONFIG, temp & ~0xE0);// clear self test bits
    write_byte(MPU_ADDRESS, ACCL_CONFIG, temp & ~0x18);// clear full scale range
    temp=read_byte(MPU_ADDRESS, ACCL_CONFIG);// read new register content
    write_byte(MPU_ADDRESS, ACCL_CONFIG, temp | 0<<3);// select range to be 2g


    // configure gyro
    temp=read_byte(MPU_ADDRESS, GYRO_CONFIG);
    write_byte(MPU_ADDRESS, GYRO_CONFIG, temp & ~0xE0);// clear self test bits
    write_byte(MPU_ADDRESS, GYRO_CONFIG, temp & ~0x18);// clear full scale range
    temp=read_byte(MPU_ADDRESS, GYRO_CONFIG);
    write_byte(MPU_ADDRESS, GYRO_CONFIG, temp | 0<<3);// select range to be 200deg/sec
}

void write_byte(uint8_t address, uint8_t reg_address, uint8_t data){
    char out[2];
    out[0]=reg_address;
    out[1]=data;
    i2c.write(address, out, 2);
}


char read_byte(uint8_t address, uint8_t reg_address){
    char data_read[1];
    char data_write[1];

    data_write[0]=reg_address;

    i2c.write(address, data_write, 1);
    i2c.read(address, data_read, 1);

    return data_read[0];
}


void read_bytes(uint8_t address, uint8_t reg_address, uint8_t num, uint8_t *out){
    char data[14],data_write[1];

    data_write[0]=reg_address;

    i2c.write(address, data_write, 1);
    i2c.read(address, data, num);

    for(int i=0; i<num; i++){
        out[i]=data[i];
    }
}


