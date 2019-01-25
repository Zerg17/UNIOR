#include "libusbcontrol.h"
#include "matrixcontrol.h"
#include "sructList.h"
#include "rccontrol.h"
#include "motcontrol.h"
#include "sonarcoontrol.h"
#include "ros_con.h"
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
    usleep(100);
    initMatrix();
    usleep(100);
    initSonar();
    initRos();
    while(1){
        usleep(1000000);
    }
}
