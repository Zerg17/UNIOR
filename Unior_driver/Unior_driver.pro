TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp
INCLUDEPATH += /opt/ros/kinetic/include

LIBS += -L/opt/ros/kinetic/lib
LIBS += -lroscpp -lrosconsole -lroscpp_serialization -lrostime -lxmlrpcpp -lcpp_common -lrosconsole_backend_interface -lrosconsole_log4cxx

LIBS += -lusb-1.0 -pthread

HEADERS += \
    libusbcontrol.h \
    sructList.h \
    rccontrol.h \
    motcontrol.h
