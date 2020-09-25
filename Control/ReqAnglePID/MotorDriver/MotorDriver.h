/* This library is used to control motor speed and direction
 * It is tested with LPC1768 and L298N motor driver
 */

#ifndef MOTOR_LIB_H
#define MOTOR_LIB_H


#include "mbed.h"

class Motor{
    public:

        Motor(PinName forwardPin, PinName reversePin, PinName speedPin,float min, float max);
        void setSpeed(float speedVal);
        float getSpeed();
        void stop();

    private:
        DigitalOut fow, rev;
        PwmOut speed;
        float minSpeed,maxSpeed;
};

#endif

