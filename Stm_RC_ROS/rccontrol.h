#ifndef RCCONTROL_H
#define RCCONTROL_H

#include <sructList.h>
#include <libusbcontrol.h>
#include <time.h>
#include <thread>
#include <unistd.h>    //Работа usleep()

#include <ros/ros.h>
#include <geometry_msgs/Twist.h>

#include <iostream>
#include <stdio.h>

using namespace std;
using namespace ros;




Publisher RCPub;
geometry_msgs::Twist RCMess;
_rc3 rc3;
_rc4 rc4;
_rc5 rc5;

int byte;
int inRC;
uint32_t NumErr = 0;


void readRC(int16_t&, int16_t&, int16_t&, uint8_t);
void usbRC();
void initRC();


void initRC(){
    RCPub = node->advertise<geometry_msgs::Twist>("unior2/RC", 1000);
    //usbWrite(0, , 1);
    std::thread rcThread(usbRC);//Создание потока для работы RC пульта
    rcThread.detach();//Открепление потока(Работа без ожидания завершения потока)
}

void readRC(int16_t& speed, int16_t& angle, int16_t& control, uint8_t Nchanal){
    switch ( Nchanal ){
        case 3:
        byte = usbRead(0, rc3.buf, sizeof(rc3.msg));
        //if(rc3.msg.head.id!=0) readRC(speed, angle, control, Nchanal);
        angle=int16_t(map1(rc3.msg.u[0],1000,2000,-10000,10000));
        speed=int16_t(map1(rc3.msg.u[1],1000,2000,-10000,10000));
        control=int16_t(map1(rc3.msg.u[2],1000,2000,0,2));
            break;
        case 4:
        byte = usbRead(0, rc4.buf, sizeof(rc4.msg));
        //if(rc4.msg.head.id!=0) readRC(speed, angle, control, Nchanal);
        angle=int16_t(map1(rc4.msg.u[0],1000,2000,-10000,10000));
        speed=int16_t(map1(rc4.msg.u[1],1000,2000,-10000,10000));
        control=int16_t(map1(rc4.msg.u[2],1000,2000,0,2));
            break;
        case 5:
        byte = usbRead(0, rc5.buf, sizeof(rc5.msg));
        //if(rc5.msg.head.id!=0) readRC(speed, angle, control, Nchanal);
        angle=int16_t(map1(rc5.msg.u[0],1000,2000,-10000,10000));
        speed=int16_t(map1(rc5.msg.u[1],1000,2000,-10000,10000));
        control=int16_t(map1(rc5.msg.u[2],1000,2000,0,2));
            break;
    }
}

void usbRC(){
    while(1){
        if(allowRcThread>0){
            usleep(16000);
            readRC(spAngl.speed,spAngl.angle,control,3);
            cout<<dec<<"Кол-во шибок USB: "<<NumErr<<" Time: "<<rc3.msg.head.time<<" Bytes: "<<byte<<" Angle: "<<spAngl.angle<<" Speed: "<<spAngl.speed<<" Chanel 3: "<<control<<" Error: "<<(int)rc3.msg.head.err<<endl<<endl;
            //printf("speed: %d, angle: %d",spAngl.speed,spAngl.angle);
            //ROS_INFO("SpeedRC: %d AngularRC: %d", spAngl.speed,spAngl.angle);
            RC_mutex.lock();
            if(byte>0){
                RCMess.angular.z=spAngl.angle/100;
                RCMess.linear.x=spAngl.speed/100;
                if(control<1){
                    RCMess.angular.z=0;
                    RCMess.linear.x=0;
                }
                RCPub.publish(RCMess);
            }
            else{
                NumErr++;
                allowRcThread=0;
                deleteDevice(RCHANDLE);
                spAngl.angle=0;
                spAngl.speed=0;
            }
            RC_mutex.unlock();
        }
        else usleep(50000);
    }
}


#endif // RCCONTROL_H
