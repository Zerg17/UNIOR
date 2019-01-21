#ifndef SONARCOONTROL_H
#define SONARCOONTROL_H

#include "libusbcontrol.h"
#include "sructList.h"
#include <time.h>
#include <mutex>
#include <thread>
#include <unistd.h>    //Работа usleep()

#include <ros/ros.h>
#include <sensor_msgs/Range.h>

using namespace std;
using namespace ros;

Publisher sonarPub1;
Publisher sonarPub2;

_sonarData srcDist;
_fLedData fledDist;

sensor_msgs::Range Distance1;
sensor_msgs::Range Distance2;

void initSonar();
void controlSonar();
int readSonar(float&, float&);

void initSonar(){
    std::thread threadSonar(controlSonar);
    threadSonar.detach();
}

void controlSonar(){
    int count =0;
    int err=1;
    Distance1.ULTRASOUND;
    Distance1.field_of_view = 20;
    Distance1.max_range = 3.f;
    Distance1.min_range = 0.189f;

    Distance2.ULTRASOUND;
    Distance2.field_of_view = 20;
    Distance2.max_range = 3.f;
    Distance2.min_range = 0.189f;
    fledDist.msg.head.id = 10;
    fledDist.msg.sonar[0] = 10000;
    fledDist.msg.sonar[1] = 600;
    fledDist.msg.sonar[2] = 1000;
    fledDist.msg.sonar[3] = 5000;

    while(1){
        if(allowSonarThread>0){
            count++;
            if(count>5){
                //cout<<fLedStatus<<endl;
                if(fLedStatus>3){
                    fledDist.msg.head.id = fLedStatus-3;;
                }
                else{
                    switch(fLedStatus){
                    case 0 :
                        fledDist.msg.head.id = 0;
                        fledDist.msg.sonar[0] = 1000;
                        fledDist.msg.sonar[1] = 1000;
                        fledDist.msg.sonar[2] = 1000;
                        fledDist.msg.sonar[3] = 1000;
                        break;
                    case 1 :
                        fledDist.msg.head.id = 0;
                        fledDist.msg.sonar[0] = 10000;
                        fledDist.msg.sonar[1] = 10000;
                        fledDist.msg.sonar[2] = 10000;
                        fledDist.msg.sonar[3] = 10000;
                        break;
                    case 2 :
                        fledDist.msg.head.id = 0;
                        fledDist.msg.sonar[0] = 0;
                        fledDist.msg.sonar[1] = 0;
                        fledDist.msg.sonar[2] = 0;
                        fledDist.msg.sonar[3] = 0;
                        break;
                    case 3 :
                        fledDist.msg.head.id = 0;
                        fledDist.msg.sonar[0] = 10000;
                        fledDist.msg.sonar[1] = 600;
                        fledDist.msg.sonar[2] = 1000;
                        fledDist.msg.sonar[3] = 5000;
                        break;
                    case 4 :
                        fledDist.msg.head.id = 0;
                        fledDist.msg.sonar[0] = 10000;
                        fledDist.msg.sonar[1] = 600;
                        fledDist.msg.sonar[2] = 1000;
                        fledDist.msg.sonar[3] = 5000;
                        break;
                    case 5 :
                        fledDist.msg.head.id = 0;
                        fledDist.msg.sonar[0] = 10000;
                        fledDist.msg.sonar[1] = 600;
                        fledDist.msg.sonar[2] = 1000;
                        fledDist.msg.sonar[3] = 5000;
                        break;
                    default :
                        fledDist.msg.head.id = 0;
                        fledDist.msg.sonar[0] = 10000;
                        fledDist.msg.sonar[1] = 600;
                        fledDist.msg.sonar[2] = 1000;
                        fledDist.msg.sonar[3] = 5000;
                        break;
                    }
                }
            count=0;
            }
            err = usbWrite(FORWARDHANDLE, fledDist.buf, sizeof (fledDist.msg), 0x01);
            if(fledDist.msg.head.id < 255) fledDist.msg.head.id = 255;
            startSonar = 0;
            usleep(150000);
            //cout<<err<<endl;
            err = readSonar(Distance1.range, Distance2.range);
            //cout<<err<<endl;;
            //err=-1;
            if(err>0){
                //cout<<"Sonar1: "<<Distance1.range<<"  Sonar2: "<<Distance2.range<<endl;
            }
            else{
                cout<<"Disconnect Sonar"<<endl;
                allowSonarThread = 0;
                deleteDevice(FORWARDHANDLE);
            }
        }
    }
}

int readSonar(float& sonar1, float& sonar2){
    int err = usbRead(FORWARDHANDLE, srcDist.buf, sizeof (srcDist.msg), 0x82);
    sonar1 = srcDist.msg.sonar[0]/1000.f;
    sonar2 = srcDist.msg.sonar[1]/1000.f;
    return err;
}
#endif // SONARCOONTROL_H
