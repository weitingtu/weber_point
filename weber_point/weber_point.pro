#-------------------------------------------------
#
# Project created by QtCreator 2017-02-28T19:24:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = weber_point
TEMPLATE = app

CONFIG += static

SOURCES += main.cpp\
        mainwindow.cpp \
    inputmanager.cpp \
    cdtmanager.cpp \
    panel.cpp \
    scene.cpp \
    fermatpoint.cpp \
    triangulation.cpp \
    triangle.cpp \
    poly.cpp \
    wavepropagation.cpp \
    decomposition.cpp \
    visibilitygraph.cpp

HEADERS  += mainwindow.h \
    inputmanager.h \
    cdtmanager.h \
    panel.h \
    scene.h \
    enum.h \
    fermatpoint.h \
    triangulation.h \
    triangle.h \
    poly.h \
    wavepropagation.h \
    decomposition.h \
    visibilitygraph.h

DEFINES += ANSI_DECLARATORS \
    TRILIBRARY
