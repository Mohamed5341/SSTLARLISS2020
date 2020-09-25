#include "mbed.h"
#include "MyMPU6050.h"
#include "MotorDriver.h"

//MyMPU6050(SDA,SCL.Inter)
MyMPU6050 mpu(p28,p27,p11); 
//Motor(In1,In2,Speed,min,max)
Motor motorB(p17,p16,p23,0.2,0.7);
Motor motorA(p20,p19,p24,0.2,0.7);
//Serial(TX,RX)
Serial RF(p13,p14);

float current_angle,required_angle,angle_error;
float Kp=0.0001,Ki=0.0,Kd=0.0,s;
float prev_error,int_error;
float base_speed;

int main()
{   
    motorA.stop();
    motorB.stop();
    
    RF.format(8,SerialBase::None,1);
    RF.baud(9600);
    
    
    RF.printf("base Speed\n");
    while(true){
        if(RF.readable()){
            RF.scanf("%f",&base_speed);
            RF.printf("speed=%.2f\n",base_speed);
            break;
        }
    }
    
    RF.printf("Kp\n");
    while(true){
        if(RF.readable()){
            RF.scanf("%f",&Kp);
            RF.printf("Kp=%.5f\n",Kp);
            break;
        }
    }
    
    RF.printf("Ki\n");
    while(true){
        if(RF.readable()){
            RF.scanf("%f",&Ki);
            RF.printf("Ki=%.5f\n",Ki);
            break;
        }
    }
    
    RF.printf("Kd\n");
    while(true){
        if(RF.readable()){
            RF.scanf("%f",&Kd);
            RF.printf("Kd=%.5f\n",Kd);
            break;
        }
    }
    
    mpu.setup();
    required_angle = 0.0;
    prev_error = 0.0;
    int_error = 0.0;
    
    
    motorA.setSpeed(base_speed);
    motorB.setSpeed(base_speed);
    
    while (true) {
        mpu.loop();
        current_angle = mpu.get_angle()*180.0/M_PI;
        
        
        angle_error = required_angle - current_angle;
        int_error = int_error + angle_error;
        s = Kp * angle_error + Ki * int_error + Kd * (angle_error - prev_error);
        prev_error = angle_error;
        
        
        mpu.loop();
        
        
        motorA.setSpeed(base_speed + s);
        motorB.setSpeed(base_speed - s);
        
        
        RF.printf("e=%0.2f\t", angle_error);
        RF.printf("R=%0.2f\t", motorA.getSpeed());
        RF.printf("L=%0.2f\t\n", motorB.getSpeed());
        //mpu.loop();
    }
}

