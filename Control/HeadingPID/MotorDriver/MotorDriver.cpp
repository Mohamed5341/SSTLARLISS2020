#include "MotorDriver.h"

/* Default constructor which takes three inputs 
 * speedPin: PWM pin used in controlling speed
 * forwardPin & reversePin: control motor rotation direction
 */
Motor::Motor(PinName forwardPin, PinName reversePin, PinName speedPin, float min, float max): fow(forwardPin), rev(reversePin), speed(speedPin), minSpeed(min), maxSpeed(max){
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
    float r;
    fow = (speedVal>0);
    rev = (speedVal<0);

    r = fabs(speedVal);

    if(r >= minSpeed && r <= maxSpeed){
        speed = r;
    }else if(r < minSpeed) {
        speed = minSpeed;
    }else if(r > maxSpeed){
        speed = maxSpeed;
    }else {
        stop();
    }
}


/* This function is used to stop motor
 */
void Motor::stop(){
    fow = 0;
    rev = 0;
    speed = 0;
}

/* This function is used to stop motor
 */
 float Motor::getSpeed(){
     return speed.read() * (rev>0 ? -1.0:1.0);
}
