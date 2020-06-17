#include "mbed.h"


DigitalOut In1(p17);
DigitalOut In2(p18);
DigitalOut In3(p19);
DigitalOut In4(p20);

//use pin 23,24 as 21,22 doesn't work
PwmOut EnA(p23);
PwmOut EnB(p24);


int main()
{
    EnA.period(0.001);
    EnB.period(0.001);

    In1 = 1;
    In2 = 0;
    In3 = 1;
    In4 = 0;

    EnA = 0.5;
    EnB = 0.5;

    while (true) {
       
    }
}

