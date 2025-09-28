QT += core gui xml network sql

ROOT = ../..
include( $$ROOT/common.pri )
include( $$ROOT/app.pri )

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
TARGET = telegrambot
#CONFIG += console

TEMPLATE = app

SOURCES += \
    dbmanager.cpp \
    mainwindow.cpp \
    telegrambotgui.cpp \
    main.cpp \
    botcontroller.cpp \
    messagebroker.cpp

HEADERS += \
    dbmanager.h \
    mainwindow.h \
    telegrambotgui.h \
    botcontroller.h \
    messagebroker.h \
    types.h

FORMS += \
    mainwindow.ui

LIBS += \
    -ltelegrambotlib-qt-fork$${LIB_SUFFIX}

INCLUDEPATH += $${INC_PATH}/

DISTFILES += \
    resources\appicon.rc

RC_FILE = resources\appicon.rc

RESOURCES += \
    resources.qrc

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


