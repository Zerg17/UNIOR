#include <libusbcontrol.h>
#include <sructList.h>
#include <rccontrol.h>
#include <motcontrol.h>
#include <unistd.h>    //Работа usleep()

#include <iostream>

using namespace std;
int main(int argc, char* argv[])
{
    startUsb(argc, argv);
    usleep(100);
    initRC();
    usleep(100);
    initMotor();
    while(1){
        usleep(1000000);
    }
}



