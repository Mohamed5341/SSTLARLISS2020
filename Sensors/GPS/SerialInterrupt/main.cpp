#include "mbed.h"

RawSerial PC(USBTX, USBRX);

void receive();

int main()
{
    PC.format(8,SerialBase::None,1);
    PC.baud(9600);

    PC.attach(&receive, Serial::RxIrq);
    
    while (true) {
        
    }
}


void receive(){
    PC.putc(PC.getc());
}