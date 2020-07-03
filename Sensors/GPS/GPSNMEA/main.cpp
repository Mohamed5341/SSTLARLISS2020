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

int Lat_deg,Lat_min,Long_deg,Long_min;
int posfix;

char buffer[55];

char zeros[6][6] = {"", "0", "00", "000", "0000", "00000"};

// UnbufferedSerial(TX,RX)
static UnbufferedSerial GPS(p9,p10);
static UnbufferedSerial RF(p13,p14);

void readBytesUntil(char container[], char req, int size);
double minutesConvert(char container[], int start);
int degreesConvert(char container[], int end);
int digitsNum(int num);

int main()
{
    char c;
    int size,lon_z,lat_z;

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
                    readBytesUntil(UTCTIM, ',', 11);
                    readBytesUntil(Latitude, ',', 10);
                    readBytesUntil(NSInd, ',', 2);
                    readBytesUntil(Longitude, ',', 11);
                    readBytesUntil(EWInd, ',', 2);
                    readBytesUntil(PosFix, ',', 2);
                    readBytesUntil(Satellites, ',', 3);
                    readBytesUntil(Altitude, ',', 4);
                    readBytesUntil(Altitude, ',', 6);

                    posfix = atoi(PosFix);

                    if(posfix == 1){//check if data is valid
                        Lat_deg = degreesConvert(Latitude, 2);
                        Lat_min = minutesConvert(Latitude, 2)*1000000;
                        Long_deg = degreesConvert(Longitude, 3);
                        Long_min = minutesConvert(Longitude, 3)*1000000;

                        lat_z = 6 - digitsNum(Lat_min);
                        lon_z = 6 - digitsNum(Long_min);

                        size = sprintf(buffer,"GPS,%s,%d.%s%d,%s,%d.%s%d,%s,%s,%s,%s\n",UTCTIM,Lat_deg,zeros[lat_z],Lat_min,NSInd,Long_deg,zeros[lon_z],Long_min,EWInd,PosFix,Satellites,Altitude);
                        RF.write(buffer,size);
                    }else{
                        size = sprintf(buffer,"GPS,Position is not fix\n");
                        RF.write(buffer, size);
                    }
                }
            }
        }
    }
}


/* This function is used toprintf ian char in text
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
    container[size] = '\0';//make sure to add null at the end if it can't find the required char
}


/* This function is used to convert minutes in Lngitude number or Latitude to degrees
 * for example:
 * Lat = 2232.1745;//before conversion
 * container = ['2', '2', '3', '2', '.', '1', '7', '4', '5'];
 * minutesConvert(container, 2); ==> 32.1745/60.0=0.5362416
 */
double minutesConvert(char container[], int start){
    char minutes[8];

    for(int i=start; i< start+7; i++){
        minutes[i-start] = container[i];
    }

    minutes[7] = '\0';

    return atof(minutes)/60.0;
}


/* This function gets longitude degree part of longitude and latitude
 * for example:
 * Lat = 2232.1745;//before conversion
 * container = ['2', '2', '3', '2', '.', '1', '7', '4', '5'];
 * degreesConvert(container, 2); ==> 22
 */
int degreesConvert(char container[], int end){
    char deg[4];

    for(int i = 0; i<end; i++){
        deg[i] = container[i];
    }

    deg[end] = '\0';//add null cahr at the end

    return atoi(deg);

}

int digitsNum(int num){
    int s = 0;

    while(num>0){
        num = num/10;
        s++;
    }

    return s;
}