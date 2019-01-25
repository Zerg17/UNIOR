#ifndef RCCONTROL_H
#define RCCONTROL_H

#include "sructList.h"
#include "libusbcontrol.h"
#include <time.h>
#include <thread>
#include <unistd.h>    //Работа usleep()h>

#include <iostream>
#include <stdio.h>


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
    std::thread rcThread(usbRC);//Создание потока для работы RC пульта
    rcThread.detach();//Открепление потока(Работа без ожидания завершения потока)
}

void readRC(int16_t& speed, int16_t& angle, int16_t& control, uint8_t Nchanal){
    uint8_t buf[1] = {0};
            usleep(1000);
            byte = usbWrite(0,buf,1,0x01);
            if(byte<0){ cout<<"1"<<endl; return;}
    switch ( Nchanal ){
        case 3:
        byte = usbRead(0, rc3.buf, sizeof(rc3.msg), 0x82);
        //if(rc3.msg.head.id!=0) readRC(speed, angle, control, Nchanal);
        angle=int16_t(map1(rc3.msg.u[0],1000,2000,-10000,10000));
        speed=int16_t(map1(rc3.msg.u[1],1000,2000,-10000,10000));
        control=int16_t(map1(rc3.msg.u[2],1000,2000,0,2));
        if(angle>0)angle=max(0,angle-500);
        if(angle<0)angle=min(0,angle+500);
        if(speed>0)speed=max(0,speed-500);
        if(speed<0)speed=min(0,speed+500);

            break;
        case 4:
        byte = usbRead(0, rc4.buf, sizeof(rc4.msg), 0x82);
        //if(rc4.msg.head.id!=0) readRC(speed, angle, control, Nchanal);
        angle=int16_t(map1(rc4.msg.u[0],1000,2000,-10000,10000));
        speed=int16_t(map1(rc4.msg.u[1],1000,2000,-10000,10000));
        control=int16_t(map1(rc4.msg.u[2],1000,2000,0,2));
            break;
        case 5:
        byte = usbRead(0, rc5.buf, sizeof(rc5.msg), 0x82);
        //if(rc5.msg.head.id!=0) readRC(speed, angle, control, Nchanal);
        angle=int16_t(map1(rc5.msg.u[0],1000,2000,-10000,10000));
        speed=int16_t(map1(rc5.msg.u[1],1000,2000,-10000,10000));
        control=int16_t(map1(rc5.msg.u[2],1000,2000,0,2));
            break;
    }
}

//if(angle>0)angle=max(0,angle-15);
//if(angle<0)angle=min(0,angle+15);
//if(speed>0)speed=max(0,speed-15);
//if(speed<0)aRC=min(0,speed+15);


void usbRC(){
    while(1){
        if(allowRcThread>0){
            usleep(16000);
            readRC(spAnglRC.speed,spAnglRC.angle,control,3);
            RC_mutex.lock();
            if(byte>0){
                if(control<1){
                    spAnglRC.angle = 0;
                    spAnglRC.speed = 0;
                }
                if(spAnglRC.speed>20000 | spAnglRC.angle>20000 | spAnglRC.speed>20000 | spAnglRC.angle>20000){
                    spAnglRC.angle = 0;
                    spAnglRC.speed = 0;
                }
            }
            else{
                NumErr++;
                allowRcThread=0;
                deleteDevice(RCHANDLE);
                cout<<"delete RC"<<endl;
                spAnglRC.angle=0;
                spAnglRC.speed=0;
            }
            RC_mutex.unlock();
        }
        else usleep(50000);
    }
}


#endif // RCCONTROL_H
