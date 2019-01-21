#ifndef MOTCONTROL_H
#define MOTCONTROL_H

#include <sructList.h>
#include <libusbcontrol.h>
#include <time.h>
#include <mutex>
#include <thread>
#include <unistd.h>    //Работа usleep()

#include <ros/ros.h>
#include <geometry_msgs/Twist.h>

#include <iostream>
#include <stdio.h>

using namespace std;
using namespace ros;

_motorData motorData;

Publisher motorPub;
Subscriber motorSub;

void controlMotor(const geometry_msgs::Twist&);
void initMotor();
void motorSend();

void initMotor(){
    motorPub = node->advertise<geometry_msgs::Twist>("unior2/RC", 1000);
    motorSub = node->subscribe("/unior2/twist",1000, controlMotor);
    std::thread threadMotor(motorSend);
    threadMotor.detach();
}

void controlMotor(const geometry_msgs::Twist& msg){
    RC_mutex.lock();
    motorData.msg.u[0]=uint8_t(msg.angular.z);
    motorData.msg.u[1]=uint8_t(msg.linear.x);
    RC_mutex.unlock();
}

void motorSend(){
    int err = 0;
    while(true){
        usleep(16000);
        for(uint16_t k = 4; k < 127; k++){
            if(handle[k]){
                if(control>0){
                    //cout<<"msg1"<<endl;

                    motorData.msg.u[0]=(spAngl.speed+spAngl.angle);
                    motorData.msg.u[1]=(spAngl.speed-spAngl.angle);
                    err=usbWrite(k,motorData.buf,sizeof(motorData));
                    //cout<<int(err)<<endl;
                    if(err<0)deleteDevice(k);
                    //cout<<"Send msg1"<<endl;
                }
                else{
                    //cout<<"msg2"<<endl;
                    err=usbWrite(k,motorData.buf,sizeof(motorData));
                    //cout<<int(err)<<endl;
                    if(err<0)deleteDevice(k);
                    //cout<<"Send msg2"<<endl;
                }
            }
        }
    }
}
#endif // MOTCONTROL_H
