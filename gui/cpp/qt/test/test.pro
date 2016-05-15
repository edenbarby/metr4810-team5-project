QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test
TEMPLATE = app

#LIBS += 'pkg-config copencv --libs'

SOURCES += main.cpp \
    display.cpp

HEADERS  += \
    display.h

FORMS    +=
