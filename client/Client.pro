QT       += core gui network charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

win32: RC_ICONS += appicon\\Icon.ico

DESTDIR += "C:\\Users\\twelv\\Dropbox\\coursework3\\client"

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    authentification.cpp \
    broker_mainwindow.cpp \
    chat_window.cpp \
    client_entity.cpp \
    consultant_mainwindow.cpp \
    get_fields.cpp \
    histogram.cpp \
    main.cpp \
    mainwindow.cpp \
    registration.cpp

HEADERS += \
    authentification.h \
    broker_mainwindow.h \
    chat_window.h \
    client_entity.h \
    consultant_mainwindow.h \
    histogram.h \
    mainwindow.h \
    registration.h

FORMS += \
    authentification.ui \
    broker_mainwindow.ui \
    chat_window.ui \
    consultant_mainwindow.ui \
    histogram.ui \
    mainwindow.ui \
    registration.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    style.css
