#-------------------------------------------------
#
# Project created by QtCreator 2016-05-14T17:27:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ftdi_ui
TEMPLATE = app

LIBS += 'pkg-config copencv --libs'

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
