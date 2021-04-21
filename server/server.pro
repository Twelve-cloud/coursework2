TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lws2_32

SOURCES += \
        source/main.cpp \
        source/menu.cpp

HEADERS += \
    header/account.h \
    header/constants.h \
    header/mysql_api.h \
    header/mysql_api_exception.h \
    header/streamtable.h \
    header/tcp_server_exception.h \
    header/winsock_tcp_server.h


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/'../../../../../Program Files/MySQL/MySQL Server 8.0/lib/' -llibmysql
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/'../../../../../Program Files/MySQL/MySQL Server 8.0/lib/' -llibmysql

INCLUDEPATH += $$PWD/'../../../../../Program Files/MySQL/MySQL Server 8.0/include'
DEPENDPATH += $$PWD/'../../../../../Program Files/MySQL/MySQL Server 8.0/include'
