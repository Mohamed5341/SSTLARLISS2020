#include "MotorDriver.h"

/* Default constructor which takes three inputs 
 * speedPin: PWM pin used in controlling speed
 * forwardPin & reversePin: control motor rotation direction
 */
Motor::Motor(PinName forwardPin, PinName reversePin, PinName speedPin): fow(forwardPin), rev(reversePin), speed(speedPin){
    speed.period(0.001);

    fow = 0;
    rev = 0;
    speed = 0;
}

/* This function takes speed of motor [-1:1]
 * for speedVal = 1;//this means motor runs with full speed
 * for speedVal = 0.5;//this means motor run with half speed
 * for speedVal = -1;//this means motor run with full speed in other direction
 */
void Motor::setSpeed(float speedVal){
    fow = (speedVal>0);
    rev = (speedVal<0);

    speed = fabs(speedVal);
}


/* This function is used to stop motor
 */
void Motor::stop(){
    fow = 0;
    rev = 0;
    speed = 0;
}
