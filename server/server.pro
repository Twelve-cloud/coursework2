TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lws2_32

SOURCES += \
        main.cpp

HEADERS += \
    streamtable.h \
    tcp_server_exception.h \
    winsock_tcp_server.h
