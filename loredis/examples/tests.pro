TEMPLATE = app
TARGET = test

MOC_DIR=build/moc
UI_DIR=build/uic
RCC_DIR=build/obj
OBJECTS_DIR=build/obj

CONFIG += c++11 qt warn_on release thread

QT += core
QT -= network widgets

INCLUDEPATH += ../src

LIBS += -lLoRedis -levent -lhiredis -L../src/

HEADERS += RedisTester.h
SOURCES += main.cpp RedisTester.cpp
