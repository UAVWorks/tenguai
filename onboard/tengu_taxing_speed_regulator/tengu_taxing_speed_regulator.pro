#######################################################################################################################
#                                    Ground speed regulator for the taxing and run-out stage.                         #
#                                                                                                                     #
# Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                          Created 24 may 2017 at 08:14 #
#######################################################################################################################

TEMPLATE=app
TARGET=build/tengu_ground_speed_regulator

QT += core
QT -= network widgets gui

CONFIG += qt warn_on release thread rtti c++11
#
#QMAKE_CXXFLAGS += -fPIC -pthread

MOC_DIR=build/moc
UI_DIR=build/uic
RCC_DIR=build/obj
OBJECTS_DIR=build/obj

LIBS += -L../../common/ -lLoRedis -L../../loredis -levent -lhiredis 

INCLUDEPATH += ../../common  ../  ../../loredis/src/

HEADERS += ../tengu_onboard_common/AbstractRegulator.h      \
        TaxingSpeedRegulator.h

SOURCES += main.cpp                                         \
        ../tengu_onboard_common/PID.cpp                     \
        ../tengu_onboard_common/AbstractRegulator.cpp       \
        TaxingSpeedRegulator.cpp


