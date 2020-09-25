#include "mbed.h"
#include "MyMPU6050.h"
#include "MotorDriver.h"


float current_angle,required_angle,angle_error;
float Kp=0.08,Ki=0.0,Kd=0.0,s;
float prev_error,int_error;
float base_speed=0.33;
bool receive_status = false;
char temp[10];
int counter = 0;


//MyMPU6050(SDA,SCL.Inter)
MyMPU6050 mpu(p28,p27,p11); 
//Motor(In1,In2,Speed,min,max)
Motor motorB(p17,p16,p23,0.2,0.7);
Motor motorA(p20,p19,p24,0.2,0.7);
//RawSerial(tx,rx)
RawSerial RF(p13,p14);

void RFISR();

int main()
{
    motorA.stop();
    motorB.stop();
    
    //ThisThread::sleep_for(5000);
    
    RF.format(8,SerialBase::None,1);
    RF.baud(9600);
    
    
    RF.attach(&RFISR,Serial::RxIrq);

    
    mpu.setup();
    required_angle = 0.0;
    prev_error = 0.0;
    int_error = 0.0;


    motorA.setSpeed(base_speed);
    motorB.setSpeed(base_speed);

    while (true) {
        mpu.loop();
        current_angle = mpu.get_angle();
        
        angle_error = atan2(sin(required_angle - current_angle),cos(required_angle - current_angle));
        int_error = int_error + angle_error;
        s = Kp * angle_error + Ki * int_error + Kd * (angle_error - prev_error);
        prev_error = angle_error;
        
        
        mpu.loop();
        
        
        motorA.setSpeed(base_speed + s);
        motorB.setSpeed(base_speed - s);
        
        
        RF.printf("e=%0.2f\t", angle_error*180/M_PI);
        RF.printf("R=%0.2f\t", motorA.getSpeed());
        RF.printf("L=%0.2f\t\n", motorB.getSpeed());
        
        if(receive_status){
            temp[counter] = '\0';
            required_angle = atof(temp)*M_PI/180;
            
            
            RF.printf("r=%f\n",required_angle*180/M_PI);
            receive_status = false;
            counter = 0;
        }
    }
}


void RFISR(){
    char c;
    c = RF.getc();    
    if(c == 'a'){
        receive_status = true;
    }else{
        temp[counter] = c;
    }

    counter++;
}