#ifndef MATRIXCONTROL_H
#define MATRIXCONTROL_H

#include "sructList.h"
#include "libusbcontrol.h"
#include <time.h>
#include <mutex>
#include <thread>
#include <unistd.h>    //Работа usleep()
#include <iostream>
#include <stdio.h>

uint8_t buf1[64][32][3];
uint8_t buf2[64*16*4];

void readBmp(char*);
int sendImage();
void initMatrix();
void matrixThr();

void readBmp(char* name){
    FILEHEADER fh;
    INFOHEADER ih;
    IMAGE BMP;
    FILE *img;
    char FN[255]={NULL};
    char filename[31] ="/home/unior/emote/";
    strcat(FN,filename);
    strcat(FN,name);
    strcat(FN,".BMP");
    img = fopen(FN, "rb");
    fread(&fh, sizeof(unsigned char), sizeof(FILEHEADER), img);
    fread(&ih, sizeof(unsigned char), sizeof(INFOHEADER), img);
    fseek(img,fh.imageDataOffset,0);
    for(int y = (32-1); y >= 0; y--)
    {
        for(int x = (64-1); x >= 0; x--)
        {
            fread(&BMP, sizeof(unsigned char), sizeof(IMAGE), img);
            buf1[63-x][y][0] = BMP.r;
            buf1[63-x][y][1] = BMP.g;
            buf1[63-x][y][2] = BMP.b;
        }
    }
    fclose(img);
}

int sendImage(){
    int err = 0;
    uint16_t count = 0;
    for(int k =7; k>3; k--){
        for(int y = 15; y >= 0; y--){
            for(int x = 63; x >= 0; x--){
                    buf2[count]=((((int(buf1[x][y+16][0])>>k) & 1)<<0) | (((int(buf1[x][y+16][1])>>k) & 1)<<1) | (((int(buf1[x][y+16][2])>>k) & 1)<<2)|
                            (((int(buf1[x][y][0])>>k) & 1)<<3) | (((int(buf1[x][y][1])>>k) & 1)<<4) | (((int(buf1[x][y][2])>>k) & 1)<<5));
                    count++;
            }
        }
    }
        for(int i = (4096/32)-1; i >= 0; i--){
            uint8_t mes[34];
            for(int k = 0; k <32; k++){
                mes[k]=buf2[32*(i) + k];
            }
            mes[32]=i>>8;
            mes[33]=i;
                err = usbWrite(MATRIXHANDLE, mes, sizeof(mes), 0x01);
                if(err<0)return err;
        }
        return 0;
}

void initMatrix(){
    std::thread matrixThread(matrixThr);//Создание потока для работы RC пульта
    matrixThread.detach();//Открепление потока(Работа без ожидания завершения потока)
}

void matrixThr(){
    while(1){
        if(AllowMatrixThread>0){
            usleep(15990);
            uint8_t mask = 0;
                if(spAnglRC.speed == 0) mask|=(1<<0);
                if(spAnglRC.speed < 0) mask|=(1<<1);
                if(spAnglRC.speed > 0) mask|=(1<<2);
                if(spAnglRC.angle == 0) mask|=(1<<3);
                if(spAnglRC.angle < 0) mask|=(1<<4);
                if(spAnglRC.angle > 0) mask|=(1<<5);
                switch(mask){
                    case 0b001001 : readBmp("Stop"); cout<<1<<endl;fLedStatus =0; break;
                case 0b001010 : readBmp("Back"); cout<<2<<endl;fLedStatus =0; break;
                case 0b001100 : readBmp("Forward"); cout<<3<<endl;fLedStatus =1; break;
                case 0b010001 : readBmp("Spin"); cout<<4<<endl;fLedStatus =10; break;
                case 0b010010 : readBmp("Left"); cout<<5<<endl;fLedStatus =5; break;
                case 0b010100 : readBmp("Right"); cout<<6<<endl;fLedStatus =6; break;
                case 0b100001 : readBmp("Spin"); cout<<7<<endl;fLedStatus =11; break;
                case 0b100010 : readBmp("Right"); cout<<8<<endl;fLedStatus =6; break;
                case 0b100100 : readBmp("Left"); cout<<9<<endl;fLedStatus =5; break;
                default : readBmp("Stop"); cout<<0<<endl;fLedStatus =0; break;
                }
            //readBmp("Stop");
            //cout<<"Send image"<<endl;
            if(sendImage()<0){AllowMatrixThread = 0; deleteDevice(MATRIXHANDLE);}
        }
    }
}
#endif // MATRIXCONTROL_H

/*    FILE* flImg = fopen("qwer.h","wb");
    fprintf(flImg,"const uint8_t buf[] = {");
    for(int k =7; k>3; k--)
        for(int y = 15; y >= 0; y--)
            for(int x = 63; x >= 0; x--){
                int xP, yP;
                xP=x;
                yP=y;
                fprintf(flImg,"0x%.2X, ", (((int(buf1[xP][yP+16][0])>>k) & 1)<<0) | (((int(buf1[xP][yP+16][1])>>k) & 1)<<1) | (((int(buf1[xP][yP+16][2])>>k) & 1)<<2)|
                                          (((int(buf1[xP][yP][0])>>k) & 1)<<3) | (((int(buf1[xP][yP][1])>>k) & 1)<<4) | (((int(buf1[xP][yP][2])>>k) & 1)<<5));
                    }
            fprintf(flImg,"};\n\n\n");
            fclose(flImg);
*/
