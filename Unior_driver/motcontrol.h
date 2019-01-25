#ifndef MOTCONTROL_H
#define MOTCONTROL_H

#include "sructList.h"
#include "libusbcontrol.h"
#include "ros_con.h"
#include <time.h>
#include <mutex>
#include <thread>
#include <unistd.h>    //Работа usleep()

#include <iostream>
#include <stdio.h>

using namespace std;

_motorData motorData;

void initMotor();
void motorSend();

void initMotor(){;
    std::thread threadMotor(motorSend);
    threadMotor.detach();
}


void motorSend(){
    int err = 0;
    while(true){
        usleep(16000);
        for(uint16_t k = 4; k < 127; k++){
            if(handle[k]){
                if(control>0){
                    motorData.msg.u[0]=(spAnglRC.speed+spAnglRC.angle);
                    motorData.msg.u[1]=(spAnglRC.speed-spAnglRC.angle);
                    RCMess.angular.z = spAnglRC.angle;
                    RCMess.linear.x = spAnglRC.speed;
                    cout<<"control1"<<endl<<"Left "<<motorData.msg.u[0]<<" Right "<<motorData.msg.u[0]<<endl;
                    err=usbWrite(k,motorData.buf,sizeof(motorData), 0x01);
                    if(err<0)deleteDevice(k);
                }
                else{
                    motorData.msg.u[0]=(spAnglRos.speed+spAnglRos.angle);
                    motorData.msg.u[1]=(spAnglRos.speed-spAnglRos.angle);
                    RCMess.angular.z = spAnglRC.angle;
                    RCMess.linear.x = spAnglRC.speed;
                    cout<<"control2"<<endl<<"Left "<<motorData.msg.u[0]<<" Right "<<motorData.msg.u[0]<<endl;
                    err=usbWrite(k,motorData.buf,sizeof(motorData), 0x01);
                    if(err<0)deleteDevice(k);;
                }
            }
        }
    }
}
#endif // MOTCONTROL_H
