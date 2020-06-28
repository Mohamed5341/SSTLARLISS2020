/* This class is used to define MPU6050
 * which is used for main procedure of code
 */


#ifndef _MY_MPU6050_H_
#define _MY_MPU6050_H_

#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
# define M_PI 3.14159265358979323846


class MyMPU6050{
    private:
        MPU6050 mpu;

        // MPU control/status vars
        bool dmpReady = false;  // set true if DMP init was successful
        uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
        uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
        uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
        uint16_t fifoCount;     // count of all bytes currently in FIFO
        uint8_t fifoBuffer[64]; // FIFO storage buffer

        // orientation/motion vars
        Quaternion q;           // [w, x, y, z]         quaternion container
        VectorInt16 aa;         // [x, y, z]            accel sensor measurements
        VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
        VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
        VectorFloat gravity;    // [x, y, z]            gravity vector
        float euler[3];         // [psi, theta, phi]    Euler angle container
        float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

    public:

        MyMPU6050(PinName i2cSda, PinName i2cScl, PinName interrupt) : mpu(i2cSda, i2cScl){
            
        }

        void setup(){

            //initialize mpu
            mpu.initialize();
            
            //coneection test
            mpu.testConnection();

            //initialize DMP
            devStatus = mpu.dmpInitialize();

            // supply your own gyro offsets here, scaled for min sensitivity
            mpu.setXGyroOffset(220);
            mpu.setYGyroOffset(76);
            mpu.setZGyroOffset(-85);
            mpu.setZAccelOffset(1788); // 1688 factory default for my test chip
            

            // make sure it worked (returns 0 if so)
            if (devStatus == 0) {
                // Calibration Time: generate offsets and calibrate our MPU6050
                mpu.CalibrateAccel(6);
                mpu.CalibrateGyro(6);
                mpu.PrintActiveOffsets();
                // turn on the DMP, now that it's ready
                mpu.setDMPEnabled(true);
                
                // enable Arduino interrupt detection
                mpuIntStatus = mpu.getIntStatus();

                // set our DMP Ready flag so the main loop() function knows it's okay to use it
                dmpReady = true;

                // get expected DMP packet size for later comparison
                packetSize = mpu.dmpGetFIFOPacketSize();
            } else {
                // ERROR!
                // 1 = initial memory load failed
                // 2 = DMP configuration updates failed
                // (if it's going to break, usually the code will be 1)

                printf("ERROR:\t%d\n",devStatus);
            }
        }


        void loop(){
            // if programming failed, don't try to do anything
            if (!dmpReady) return;
            // read a packet from FIFO
            if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) { // Get the Latest packet 

                // display Euler angles in degrees
                mpu.dmpGetQuaternion(&q, fifoBuffer);
                mpu.dmpGetGravity(&gravity, &q);
                mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

            }
        }

        float get_angle(){
            return ypr[0];
        }

};

#endif