#ifndef LIBUSBCONTROL_H
#define LIBUSBCONTROL_H

#include <libusb-1.0/libusb.h>
#include <mutex>
#include <thread>
#include <unistd.h>    //Работа usleep()

#include "sructList.h"

#include <iostream>

using namespace std;

#define Vid 0x0bda
#define Pid 0x2838
#define RCBCD 0xFF
#define MATRIXBCD 0xF0
#define FORWARDBCD 0x10
#define BACKWARDBCD 0x20

libusb_context *ctx = NULL;
libusb_device_handle *handle[127];
std::mutex handle_mutex;
uint16_t bcdList[128];
uint8_t allowRcThread = 0;
uint8_t AllowMatrixThread = 0;
uint8_t allowSonarThread = 0;
uint8_t startSonar = 0;

void scanDevice();
void startUsb(int, char*[]);
int usbRead(uint8_t, uint8_t*, uint8_t, uint8_t);
int usbWrite(uint8_t, uint8_t*, uint8_t, uint8_t);
int linSearch(uint16_t*, int);
void deleteDevice(uint8_t);

void scanDevice(){
    while(1){
        //cout<<"Сканирование устройств"<<endl;
        libusb_device **devList; //Указатель на указатель на устройства
        ssize_t cnt; //Количество найденых устройств
        cnt = libusb_get_device_list(ctx, &devList); //Получение списка утройств
        if(cnt > 0) {
            for(ssize_t i = 0; i < cnt; i++){
                libusb_device_descriptor desc;
                libusb_get_device_descriptor(devList[i], &desc);
                if(desc.idProduct==Pid && desc.idVendor==Vid){
                    handle_mutex.lock();
                    switch ( desc.bcdDevice ){
                    case RCBCD:
                        if(!handle[RCHANDLE]){
                            if(linSearch(bcdList, desc.bcdDevice)<1){
                                bcdList[RCHANDLE]=desc.bcdDevice;
                                cout<<"Открытие RC"<<endl;
                                libusb_open(devList[i], &handle[RCHANDLE]);
                                uint8_t buf[1] = {0};
                                usleep(1000);
                                usbWrite(0,buf,1,0x01);
                                usleep(1000);
                                allowRcThread = 1;
                            }
                        }
                        break;
                    case MATRIXBCD:
                        if(!handle[MATRIXHANDLE]){
                            if(linSearch(bcdList, desc.bcdDevice)<1){
                                bcdList[MATRIXHANDLE]=desc.bcdDevice;
                                cout<<"Открытие MATRIX"<<endl;
                                libusb_open(devList[i], &handle[MATRIXHANDLE]);;
                                AllowMatrixThread = 1;
                            }
                        }
                        break;
                    case FORWARDBCD:
                        if(!handle[FORWARDHANDLE]){
                            if(linSearch(bcdList, desc.bcdDevice)<1){
                                bcdList[FORWARDHANDLE]=desc.bcdDevice;
                                cout<<"Открытие FORWARD"<<endl;
                                libusb_open(devList[i], &handle[FORWARDHANDLE]);
                                startSonar = 1;
                                allowSonarThread = 1;
                                cout<<"Sonar connect"<<endl;
                            }
                        }
                        break;
                    case BACKWARDBCD:
                        if(!handle[BACKWARDHANDLE]){
                            if(linSearch(bcdList, desc.bcdDevice)<1){
                                bcdList[BACKWARDHANDLE]=desc.bcdDevice;
                                cout<<"Открытие BACKWARD"<<endl;
                                libusb_open(devList[i], &handle[BACKWARDHANDLE]);
                            }
                        }
                        break;;
                    default:
                        for(uint16_t k = 4; k < 127; k++){
                            if(!handle[k]){
                                if(linSearch(bcdList, desc.bcdDevice)<1){
                                    bcdList[k]=desc.bcdDevice;
                                    cout<<"Открытие мотора"<<(k-3)<<endl;
                                    libusb_open(devList[i], &handle[k]);
                                    break;
                                }
                            }
                        }
                    }
                }
                handle_mutex.unlock();
            }
        }
        libusb_free_device_list(devList, 1); //Очистка списка устройств
        usleep(500000);
    }
}

void startUsb(int argc, char* argv[]){
    libusb_init(&ctx); //Запуск сессии USB
    libusb_set_debug(ctx, 3); //Установка режима дебага в INFO
    std::thread Th1(scanDevice);//Создание потока для сканирования USB устройств
    Th1.detach();//Открепление потока(Работа без ожидания завершения потока)
}

int usbRead(uint8_t handleNum ,uint8_t* buf, uint8_t size, uint8_t bulkadr){
    int bytes = 0;
    libusb_device_handle *hand = handle[handleNum];
    int err = libusb_bulk_transfer(hand, bulkadr, (unsigned char*)(buf), size, &bytes, 100);
    if(err<0) return err;
    return bytes;
}

int usbWrite(uint8_t handleNum ,uint8_t* buf, uint8_t size, uint8_t bulkadr){
    int bytes = 0;
    libusb_device_handle *hand = handle[handleNum];
    int err = libusb_bulk_transfer(hand, bulkadr, (unsigned char*)(buf), size, &bytes, 5100);
    if(err<0) return err;
    return bytes;
}

int linSearch(uint16_t* buf, int element){
 for (int i = 0; i < 127; i++)
 {
    if (buf[i] == element)
        return i;
    }
    return -1;
}

void deleteDevice(uint8_t numHandle){
    //int c =linSearch(bcdList, numHandle);
    libusb_close(handle[numHandle]);
    bcdList[numHandle] = 0;
    handle[numHandle]= NULL;
}


#endif // LIBUSBCONTROL_H
