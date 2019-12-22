#-------------------------------------------------
#
# Project created by QtCreator 2019-01-12T12:27:11
#
#-------------------------------------------------

QT       += core gui

CONFIG += c++14

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = fig_OpenGL
TEMPLATE = app


SOURCES += main.cpp\
        bresenham.cpp \
        grille.cpp \
        pixel.cpp \
        princ.cpp \
        glarea.cpp \
        quaternion.cpp

HEADERS  += princ.h \
        bresenham.h \
        glarea.h \
        grille.h \
        pixel.h \
        quaternion.h

FORMS    += princ.ui

RESOURCES += \
    fig.qrc
