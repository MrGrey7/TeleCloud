
QT += core gui xml

ROOT = ../..
include( $$ROOT/common.pri )
include( $$ROOT/app.pri )

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = telegrambot-deprecated
TEMPLATE = app

SOURCES += main.cpp

#RESOURCES += $$ROOT/resources.qrc
