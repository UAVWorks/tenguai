#######################################################################################################################
#                                                                                                                     #
#   X-Tengu is part of the Tengu project, dedicated to artificial intelligence in general and its applying for the    #
#  development of autonomous unmanned vehicles in particular. Assigning this plugin is modeling. Instead of flying a  #
#  real plane, you can first try to fly on the computer screen. And the X-Plane is suitable for this purpose as well  #
#  as possible it is (i think so :-D ).                                                                               #
#                                                                                                                     #
# ------------------------------------------------------------------------------------------------------------------- #
#                                                                                                                     #
#     X-Tengu - это часть проекта Tengu, посвященного искусственному интеллекту вообще и его применению в области     #
#    автономных летательных аппаратов в частности. Его назначение - это моделирование. Вместо того, чтобы летать на   #
#    реальном самолете, сначала можно полетать на экране компьютера. И X-Plane подходит для этих целей - настолько    #
#                               хорошо, насколько это вообще возможно (я так думаю :-D )                              #
#                                                                                                                     #
# ------------------------------------------------------------------------------------------------------------------- #
#                                                                                                                     #
#                                                                      Eugene G. Sysoletin <e.g.sysoletin@gmail.com>, #
#                                                          Ural Federal University, the Institute of radioelectronics #
#                                                                           and information tehnology, IT department. #
#                                                                                                                     #
#                                                                                        Created 29 apr 2017 at 13:33 #
#######################################################################################################################

TEMPLATE=lib
TARGET=build/xtengu

MOC_DIR=build/moc
UI_DIR=build/uic
RCC_DIR=build/obj
OBJECTS_DIR=build/obj

QT += core 
QT -= widgets network gui

CONFIG += release thread shared c++11

DEFINES += APL=0 IBM=0 LIN=1 XPLM200 XPLM210

QMAKE_CXXFLAGS += -fPIC -pthread

INCLUDEPATH += ../3d_party/XPSDK-213/CHeaders/XPLM/ src/xpchannels ../loredis/src ../common

HEADERS += src/XTengu.h

LIBS += -lLoRedis -levent -lhiredis -L../loredis/

SOURCES += src/xplane_plugin.cpp                \
        src/xpchannels/XPlaneChannel.cpp        \
        src/xpchannels/XChRelocate.cpp          \
        src/xpchannels/XPlaneChannel_0_100.cpp  \
        src/xpchannels/XChFloatArray.cpp        \
        src/xpchannels/XChFloatArrayTwice.cpp   \
        src/xpchannels/XChLeftAileron.cpp       \
        src/xpchannels/XChLeftFlaperon.cpp      \
        src/xpchannels/XChLeftRudder.cpp        \
        src/xpchannels/XChLeftElevator.cpp      \
        src/xpchannels/XChRightFlaperon.cpp     \
        src/xpchannels/XChRightAileron.cpp      \        
        src/xpchannels/XChRightRudder.cpp       \        
        src/xpchannels/XChRightElevator.cpp     \
        src/xpchannels/XChThrottle.cpp          \
        src/xpchannels/XChSteeringWheel.cpp     \
        src/XPlaneOverrided.cpp                 \
        src/XTengu.cpp                          \
        src/AbstractAircraft.cpp                \
        src/EnvironmentAircraft.cpp             \
        src/UserAircraft.cpp

