#-------------------------------------------------
#
# Project created by QtCreator
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ForensicAnalyzer
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


SOURCES += src/main.cpp \
    src/mainwindow.cpp \
    src/logfiledialog.cpp \
    src/configfiledialog.cpp \
    src/cmodel.cpp \
    src/qtfilecontroller.cpp \
    src/crulegenerator.cpp \
    src/caliasesmanager.cpp \
    src/cfsfunctions.cpp

HEADERS  += \
    src/mainwindow.h \
    src/logfiledialog.h \
    src/configfiledialog.h \
    src/cmodel.h \
    src/qtfilecontroller.h \
    src/crulegenerator.h \
    src/caliasesmanager.h \
    src/cfsfunctions.h

FORMS    += \
    src/mainwindow.ui \
    src/logfiledialog.ui \
    src/configfiledialog.ui

RESOURCES += \
    res.qrc
