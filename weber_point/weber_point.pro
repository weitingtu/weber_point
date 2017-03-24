#-------------------------------------------------
#
# Project created by QtCreator 2017-02-28T19:24:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = weber_point
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    triangle.cpp \
    inputmanager.cpp \
    cdtmanager.cpp \
    panel.cpp

HEADERS  += mainwindow.h \
    triangle.h \
    inputmanager.h \
    cdtmanager.h \
    panel.h

DEFINES += ANSI_DECLARATORS \
    TRILIBRARY
