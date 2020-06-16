/* using unbufered in this example so it prints all received
 * data (doesn't wait for certain amount of data)
 */
#include "mbed.h"

// UnbufferedSerial(TX,RX)
static UnbufferedSerial GPS(p9,p10);
static UnbufferedSerial PC(USBTX,USBRX);

int main(void){
    char c;

    //spicify communication details
    GPS.baud(9600);
    GPS.format(8,SerialBase::None,1);//8bits, no parity, 1 stop bit

    PC.baud(9600);
    PC.format(8,SerialBase::None,1);//8bits, no parity, 1 stop bit

    while(true){
        if(GPS.read(&c, 1)){
            PC.write(&c, 1);
        }
    }
}