#include "mbed.h"
#include "MPULib/MyMPU6050.h"

MyMPU6050 mpu(p28,p27,p10);


int main()
{
    float angle;
    
    mpu.setup();

    while (true) {
        mpu.loop();

        angle = mpu.get_angle()*180/M_PI;

        printf("IMU,%d.%d\n",(int)angle,(int)(abs(angle - (int)angle) * 10));

        ThisThread::sleep_for(500ms);
    }
}

