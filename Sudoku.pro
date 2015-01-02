#-------------------------------------------------
#
# Project created by QtCreator 2014-12-25T15:10:36
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Sudoku
TEMPLATE = app


SOURCES += \
    main.cpp\
    draglabel.cpp \
    dragwidget.cpp \
    sudoku.cpp \
    sudokuwidget.cpp

HEADERS  += \
    draglabel.h \
    dragwidget.h \
    sudoku.h \
    sudokuwidget.h

FORMS    +=

DISTFILES += \
    style.qss

win32 {
    QMAKE_LFLAGS_WINDOWS = /SUBSYSTEM:WINDOWS,5.01
    QMAKE_LFLAGS_CONSOLE = /SUBSYSTEM:CONSOLE,5.01

    DEFINES += _ATL_XP_TARGETING
    QMAKE_CFLAGS += /D _USING_V110_SDK71_
    QMAKE_CXXFLAGS += /D _USING_V110_SDK71_
    LIBS *= -L”%ProgramFiles(x86)%/Microsoft SDKs/Windows/7.1A/Lib”
    INCLUDEPATH += “%ProgramFiles(x86)%/Microsoft SDKs/Windows/7.1A/Include”
}

