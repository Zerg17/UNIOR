#ifndef STRUCTLIST_H
#define STRUCTLIST_H

#define RCHANDLE 0x0
#define MATRIXHANDLE 0x1
#define FORWARDHANDLE 0x2
#define BACKWARDHANDLE 0x3

//#define min(a,b) ((a) < (b) ? (a) : (b))
//#define max(a,b) ((a) > (b) ? (a) : (b))
//#define constrain(amt,low,high) ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))


#include <stdint.h>
#include <ros/ros.h>
#include <mutex>

std::mutex RC_mutex;

struct _head{
    uint32_t time;
    uint16_t id;
    uint8_t status;
    uint8_t err;
};

union _rc3 {
    struct __attribute__((__packed__)) {
        struct _head head;
        int16_t u[3];
    } msg;
    uint8_t buf[14];
};

union _rc4 {
    struct __attribute__((__packed__)) {
        struct _head head;
        int16_t u[3];
    } msg;
    uint8_t buf[16];
};

union _rc5 {
    struct __attribute__((__packed__)) {
        struct _head head;
        int16_t u[3];
    } msg;
    uint8_t buf[18];
};

union _motorData {
    struct __attribute__((__packed__)) {
        struct _head head;
        int16_t u[2];
    } msg;
    uint8_t buf[12];
};

struct _spAngl{
    int16_t speed;
    int16_t angle;
}spAngl;

ros::NodeHandle* node;
uint8_t RCtoROS = 0;
int16_t control = 0;


long map1(long x, long in_min, long in_max, long out_min, long out_max)
{
return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

#endif // STRUCTLIST_H
