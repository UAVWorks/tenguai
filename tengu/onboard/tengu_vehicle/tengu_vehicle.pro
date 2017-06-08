#######################################################################################################################
#                                                                                                                     #
#                                                      Vehicle application.                                           #
#                                 Any of vehicles (mobile robots: car, truck, aircraft e.t.c. )                       #
# ------------------------------------------------------------------------------------------------------------------- #
#                                                  Приложение для "самоходки".                                        #
#                                 Любой тип мобильного робота (машинка, грузовик, самолет и др.)                      #
#######################################################################################################################

TEMPLATE = app
TARGET = build/tengu_vehicle

MOC_DIR=build/moc
UI_DIR=build/uic
RCC_DIR=build/obj
OBJECTS_DIR=build/obj

CONFIG += qt rtti release thread
QMAKE_CXXFLAGS += -std=gnu++11

QT += core
QT -= network core

INCLUDEPATH += ../../core ../../loredis/src
LIBS += -L../../core/build -ltengu_core -L../../loredis/build -lLoRedis -levent -lpthread -lhiredis

SOURCES = main.cpp