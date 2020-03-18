/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include "MyMPU6050.h"


MyMPU6050 mpu(p28,p27,p10);


float yprd[3]={0,0,0};

float convertAngle(float x){
    x = fmod(x,360);
    if (x < 0)
        x += 360;
    return x;
}

int main()
{
    //setup mpu6050
    mpu.enable();
    
    //disable interrupt
    mpu.disableInterrupt();
    
    while (true) {
        
        //
        mpu.loop();
        
        yprd[0]=convertAngle(mpu.ypr[0]*180.0/M_PI);
        yprd[1]=convertAngle(mpu.ypr[1]*180.0/M_PI);
        yprd[2]=convertAngle(mpu.ypr[2]*180.0/M_PI);
        
        printf("%f\t%f\t%f\n",yprd[0],yprd[1],yprd[2]);
    }
}
