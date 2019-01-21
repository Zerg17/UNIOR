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

union _sonarData {
    struct __attribute__((__packed__)) {
        struct _head head;
        uint16_t sonar[2];
    } msg;
    uint8_t buf[64];
};

union _fLedData {
    struct __attribute__((__packed__)) {
        struct _head head;
        uint16_t sonar[4];
    } msg;
    uint8_t buf[64];
};

struct _spAngl{
    int16_t speed;
    int16_t angle;
};

typedef struct __attribute__((__packed__)) {
    unsigned char fileMarker1;
    unsigned char fileMarker2;
    unsigned int   bfSize;
    uint16_t unused1;
    uint16_t unused2;
    unsigned int   imageDataOffset;
} FILEHEADER;

typedef struct __attribute__((__packed__)) {
    unsigned int   biSize;
    int            width;
    int            height;
    uint16_t planes;
    uint16_t bitPix;
    unsigned int   biCompression;
    unsigned int   biSizeImage;
    int            biXPelsPerMeter;
    int            biYPelsPerMeter;
    unsigned int   biClrUsed;
    unsigned int   biClrImportant;
} INFOHEADER;

typedef struct __attribute__((__packed__)) {
    unsigned char  b;
    unsigned char  g;
    unsigned char  r;
} IMAGE;

_spAngl spAnglRC;
_spAngl spAnglRos;

uint8_t RCtoROS = 0;
int16_t control = 0;

int fLedStatus = 0;

long map1(long x, long in_min, long in_max, long out_min, long out_max)
{
return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

#endif // STRUCTLIST_H
