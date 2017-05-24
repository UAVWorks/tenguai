#######################################################################################################################
#      The common project for the tengu's agents, regardless of their location, are they in the sky or on earth       #
# ------------------------------------------------------------------------------------------------------------------- #
#        Общий проект для агентов Tengu, независимо от того, где они находятся физически (на небе или на земле)       #
#                                                                                                                     #
#                                                                                                                     #
#                                                                             Eugene G. Sysoletin <unclesal@mail.ru>, #
#                                                          Ural Federal University, the Institute of radioelectronics #
#                                                                           and information tehnology, IT department. #
#                                                                                                                     #
#                                                                                        created 24 apr 2017 at 16:45 #
#######################################################################################################################

TEMPLATE = lib
TARGET = tengu_common

# Only for file arranging.

MOC_DIR=build/moc
UI_DIR=build/uic
RCC_DIR=build/obj
OBJECTS_DIR=build/obj

# We want statically linked library. But we need thread support due endless libevent dispatch loop.

CONFIG += staticlib thread release rtti qt

# for nullptr

QMAKE_CXXFLAGS += -std=gnu++11


# The Qt section contains nothing here except core. LoRedis itself does not directly do network 
# functions and does not have any GUIs.

QT += core
QT -= network

LIBS += -lLoRedis -L../loredis

INCLUDEPATH += ../loredis/src/

HEADERS += TenguRedis.h
SOURCES += TenguRedis.cpp 
