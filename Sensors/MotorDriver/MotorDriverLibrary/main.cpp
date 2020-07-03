#include "mbed.h"
#include "MotorDriver.h"

Motor motorA(p16,p17,p23);
Motor motorB(p19,p20,p24);

int main()
{
    
    for(int i=5;i>=0;i--){
        motorA.setSpeed(i/10.0);
        motorB.setSpeed(i/10.0);
        printf("%d\n",i);
        ThisThread::sleep_for(10s);
    }

    
    while (true) {
        
    }
}

