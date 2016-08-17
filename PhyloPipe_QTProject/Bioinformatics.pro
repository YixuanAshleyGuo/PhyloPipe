#-------------------------------------------------
#
# Project created by QtCreator 2014-07-05T20:50:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Bioinformatics
TEMPLATE = app


SOURCES += main.cpp\
        biomainwindow.cpp \
    hamstrdialog.cpp \
    hamcmbdialog.cpp \
    settingsdialog.cpp \
    mafftdialog.cpp \
    muscledialog.cpp \
    aliscoredialog.cpp \
    concatenatedialog.cpp

HEADERS  += biomainwindow.h \
    hamstrdialog.h \
    hamcmbdialog.h \
    settingsdialog.h \
    mafftdialog.h \
    muscledialog.h \
    aliscoredialog.h \
    concatenatedialog.h

FORMS    += biomainwindow.ui \
    hamstrdialog.ui \
    hamcmbdialog.ui \
    settingsdialog.ui \
    mafftdialog.ui \
    muscledialog.ui \
    aliscoredialog.ui \
    concatenatedialog.ui
