QT += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = minesweep
TEMPLATE = app

SOURCES += main.cpp \
           mainwindow.cpp \
           board.cpp \
           cell.cpp

HEADERS += mainwindow.h \
           board.h \
           cell.h

RESOURCES += resources.qrc
