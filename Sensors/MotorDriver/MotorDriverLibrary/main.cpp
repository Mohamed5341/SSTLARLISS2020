#include "mbed.h"
#include "MotorDriver.h"

Motor motorA(p17,p18,p23);
Motor motorB(p19,p20,p24);

int main()
{

    while (true) {
        motorA.setSpeed(0.5);
        motorB.setSpeed(0.5);

        wait_us(3000*1000);

        motorA.setSpeed(0.75);
        motorB.setSpeed(0.75);

        wait_us(3000*1000);

        motorA.stop();
        motorB.stop();

        wait_us(3000*1000);
    }
}

