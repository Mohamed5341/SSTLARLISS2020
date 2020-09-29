#include "mbed.h"

#define MAXIMUM_BUFFER_SIZE 32


uint8_t buf[MAXIMUM_BUFFER_SIZE];

static BufferedSerial RF(p13, p14);
static BufferedSerial PC(USBTX, USBRX);
static DigitalOut led(LED1);

void RXInterrupt();

int main()
{
    RF.set_baud(9600);
    PC.set_baud(9600);

    while (true) {
        if(uint32_t num = RF.read(buf, 4)){
            led = !led;

            PC.write(buf, num);
        }
    }
}

