#-------------------------------------------------
#
# Project created by QtCreator 2016-10-05T20:24:15
#
#-------------------------------------------------

QT       += core gui
CONFIG += console

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Puzzle
TEMPLATE = app


SOURCES += main.cpp\
        interfacedojogo.cpp \
    jogo.cpp

HEADERS  += interfacedojogo.h \
    jogo.h

FORMS    += interfacedojogo.ui
