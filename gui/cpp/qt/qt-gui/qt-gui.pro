QT      += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET   = qt-gui
TEMPLATE = app

SOURCES += \
    mainwindow.cpp \
    main.cpp

HEADERS += \
    mainwindow.hpp \
    ../../lib/ftd2xx.h \
    ../../lib/WinTypes.h

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../lib/release/ -lftd2xx
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../lib/debug/ -lftd2xx
else:unix: LIBS += -L$$PWD/../../lib/ -lftd2xx

INCLUDEPATH += $$PWD/../../
DEPENDPATH += $$PWD/../../

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../lib/release/libftd2xx.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../lib/debug/libftd2xx.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../lib/release/ftd2xx.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../lib/debug/ftd2xx.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../lib/libftd2xx.a

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += opencv
