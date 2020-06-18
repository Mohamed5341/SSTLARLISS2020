#include "mbed.h"
#include "GPSlib.h"

GPS gps(p9, p10);

int main()
{

    while (true) {
        gps.readData();

        if(gps.isFixed()){
            printf("\n>>>>>>UTC:%s\nLat:%ld\t%c\nLong:%ld\t%c\nPosFix:%d\nAlt:%d\n",gps.UTCTIM,(long)(gps.getLatitude()*100000),gps.getNSInd(),(long)(gps.getLongitude()*100000),gps.getEWInd(),1,(int)(gps.getAltitude()*10));
        }else {
            printf("Position is not fix\n");

            wait_us(3000*1000);
        }
    }
}

