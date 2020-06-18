/* This class is used to get location from GPS
 * It reads NMEA messages using UART
 */

#ifndef _GPS_LIBRARY_H_
#define _GPS_LIBRARY_H_

#include "mbed.h"

class GPS{
    public:
        GPS(PinName TXPin, PinName RXPin);
        void readData();
        double getLongitude();
        double getLatitude();
        float getAltitude();
        char getNSInd();
        char getEWInd();
        int getSatellites();
        bool isFixed();

        char UTCTIM[11];

    private:
        UnbufferedSerial gps;

        char MSGID[6];
        char Latitude[10];
        char NSInd[2];
        char Longitude[11];
        char EWInd[2];
        char PosFix[2];
        char Satellites[3];
        char Altitude[6];

        int posfix;

        void readBytesUntil(char container[], char req, int size);
        double minutesConvert(char container[], int start);
        int degreesConvert(char container[], int end);
};

#endif