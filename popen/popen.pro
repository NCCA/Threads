#-------------------------------------------------
#
# Project created by QtCreator 2016-02-04T14:29:11
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = popen
TEMPLATE = app

CONFIG-=app_bundle
CONFIG+=c++11
SOURCES += main.cpp\
        MainWindow.cpp

HEADERS  += MainWindow.h

FORMS    += MainWindow.ui
