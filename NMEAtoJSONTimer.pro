QT += core
QT -= gui

CONFIG += c++11

TARGET = NMEAtoJSONTimer
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    nmeatojsontimer.cpp

HEADERS += \
    nmeatojsontimer.h
