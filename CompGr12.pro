#-------------------------------------------------
#
# Project created by QtCreator 2017-02-28T23:23:22
#
#-------------------------------------------------

QT       += core gui opengl

CONFIG += c++11 c++14

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CompGr12
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    glview.cpp \
    mesh.cpp \
    vertex.cpp \
    ifractal.cpp \
    splinevertex.cpp \
    application.cpp \
    abstractscene.cpp \
    graphicsobject.cpp \
    utils.cpp

HEADERS  += mainwindow.h \
    glview.h \
    mesh.h \
    vertex.h \
    ifractal.h \
    splinevertex.h \
    application.h \
    abstractscene.h \
    graphicsobject.h \
    utils.h

FORMS    += \
    mainwindow.ui

RESOURCES += \
    resources.qrc

INCLUDEPATH += .

DISTFILES += \
    shaders/simple.vert \
    shaders/simple.frag \
    ../build-CompGr12-Desktop_Qt_5_8_0_GCC_64bit-Debug/shaders/simple.frag \
    ../build-CompGr12-Desktop_Qt_5_8_0_GCC_64bit-Debug/shaders/simple.vert \
    ../build-CompGr12-Desktop_Qt_5_8_0_MinGW_32bit-Debug/shaders/simple.frag \
    ../build-CompGr12-Desktop_Qt_5_8_0_MinGW_32bit-Debug/shaders/simple.vert
