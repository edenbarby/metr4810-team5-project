QT      += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET   = qt-gui
TEMPLATE = app

LIBS += `pkg-config opencv --libs`

SOURCES += \
    mainwindow.cpp \
    main.cpp

HEADERS += \
    mainwindow.hpp
