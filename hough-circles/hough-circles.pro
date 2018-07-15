#-------------------------------------------------
#
# Project created by QtCreator 2017-12-07T23:45:58
#
#-------------------------------------------------
 
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = hough-circles
TEMPLATE = app

CONFIG += c++11
SOURCES += main.cpp\
        mainwindow.cpp \
    convertions.cpp \
    worker-thread.cpp

HEADERS  += mainwindow.h \
    convertions.h \
    worker-thread.h

FORMS    += mainwindow.ui

LIBS += -lopencv_core -lopencv_highgui -lopencv_imgproc