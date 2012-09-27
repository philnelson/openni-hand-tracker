TEMPLATE = app
CONFIG += console
CONFIG -= qt

SOURCES += main.cpp \
    PointDrawer.cpp \
    signal_catch.cpp

HEADERS += \
    PointDrawer.h \
    signal_catch.h

unix:!macx:!symbian: LIBS += -L$$PWD/../../../../usr/lib/ -lOpenNI

INCLUDEPATH += $$PWD/../../../../usr/include/openni
DEPENDPATH += $$PWD/../../../../usr/include/openni

unix:!macx:!symbian: LIBS += -L$$PWD/../../../../usr/lib/ -lXnVNite_1_5_2

INCLUDEPATH += $$PWD/../../../../usr/include/nite
DEPENDPATH += $$PWD/../../../../usr/include/nite

unix:!macx:!symbian: LIBS += -L$$PWD/../../../../usr/lib/ -lglut

INCLUDEPATH += $$PWD/../../../../usr/include
DEPENDPATH += $$PWD/../../../../usr/include

unix:!macx:!symbian: LIBS += -L$$PWD/../../../../usr/local/lib/ -lopencv_highgui

INCLUDEPATH += $$PWD/../../../../usr/local/include
DEPENDPATH += $$PWD/../../../../usr/local/include

unix:!macx:!symbian: LIBS += -L$$PWD/../../../../usr/local/lib/ -lopencv_core

INCLUDEPATH += $$PWD/../../../../usr/local/include
DEPENDPATH += $$PWD/../../../../usr/local/include

unix:!macx:!symbian: LIBS += -L$$PWD/../../../../usr/local/lib/ -lopencv_imgproc

INCLUDEPATH += $$PWD/../../../../usr/local/include
DEPENDPATH += $$PWD/../../../../usr/local/include

unix:!macx:!symbian: LIBS += -L$$PWD/../../../../usr/local/lib/ -lavutil

INCLUDEPATH += $$PWD/../../../../usr/local/include
DEPENDPATH += $$PWD/../../../../usr/local/include
