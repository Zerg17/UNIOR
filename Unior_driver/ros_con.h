#ifndef ROS_CON_H
#define ROS_CON_H

#include <ros/ros.h>
#include "sructList.h"
#include <geometry_msgs/Twist.h>

#include <iostream>
#include <stdio.h>

using namespace std;
using namespace ros;

Publisher motorPub;
Subscriber motorSub;
geometry_msgs::Twist RCMess;

void initRos();
void rosMotor(const geometry_msgs::Twist&);


void initRos(){
    int inv = 0;
    init(inv, 0, "Unior2_0");
    NodeHandle node;
    motorPub = node.advertise<geometry_msgs::Twist>("unior2/RC", 1000);
    motorSub = node.subscribe("/unior2/twist",1000, rosMotor);
    cout<<"ROS start"<<endl;
    spin();
    cout<<"ROS fail"<<endl;
}

void rosMotor(const geometry_msgs::Twist& msg){
    spAnglRos.speed=msg.linear.x*500;
    spAnglRos.angle=msg.angular.z*500;
    cout<<"ROS Принял"<<endl;
}

#endif // ROS_CON_H
