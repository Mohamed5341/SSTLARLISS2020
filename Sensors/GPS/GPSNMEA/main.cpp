#include "mbed.h"


char MSGID[6];
char UTCTIM[11];
char Latitude[10];
char NSInd[2];
char Longitude[11];
char EWInd[2];
char PosFix[2];
char Satellites[3];
char Altitude[6];


// UnbufferedSerial(TX,RX)
static UnbufferedSerial GPS(p9,p10);


void readBytesUntil(char container[], char req, int size);

int main()
{
    char c;

    //communication details
    GPS.baud(9600);
    GPS.format(8,SerialBase::None,1);//8bits data, no parity, 1 stop bit
    
    while (true) {
        if(GPS.readable()){//check data available

            GPS.read(&c, 1);//check message beginning
            //printf("%c",c);

            if(c == '$'){//check line beginning
                
                readBytesUntil(MSGID, ',', 5);


                if(MSGID[2] == 'G' && MSGID[3] == 'G' && MSGID[4] == 'A'){//check message ID
                    readBytesUntil(UTCTIM, ',', 10);
                    readBytesUntil(Latitude, ',', 9);
                    readBytesUntil(NSInd, ',', 1);
                    readBytesUntil(Longitude, ',', 10);
                    readBytesUntil(EWInd, ',', 1);
                    readBytesUntil(PosFix, ',', 2);
                    readBytesUntil(Altitude, ',', 3);
                    readBytesUntil(Altitude, ',', 5);

                    printf("\n>>>>>>UTC:%s\nLat:%s\t%s\nLong:%s\t%s\nPosFix:%s\nAlt:%s\n",UTCTIM,Latitude,NSInd,Longitude,EWInd,PosFix,Altitude);
                }
            }
        }
    }
}


/* This function is used to read data to certian char in text
 * for example:
 * coming string = "ABCDEFGH"
 * readBytesUntil(cont, 'D', 5);
 * cont = ['A', 'B', 'C', '\0',..]
 * Another example:
 * coming string = "ABCDEFGH"
 * readBytesUntil(cont, 'H', 5);
 * cont = ['A', 'B', 'C', 'D', '\0',..]
 */
void readBytesUntil(char container[], char req, int size){
    char c;

    for(int i=0; i<=size; i++){
        GPS.read(&c, 1);
        
        if(c == req){
            container[i] = '\0';//add null char to final location
            
            return;
        }

        container[i] = c;//add null char to container
    }

    container[size - 1] = '\0';//make sure to add null at the end if it can't find the required char
}
