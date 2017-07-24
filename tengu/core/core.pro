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
TARGET = build/tengu_core

# Only for file arranging.

MOC_DIR=build/moc
UI_DIR=build/uic
RCC_DIR=build/obj
OBJECTS_DIR=build/obj

# We want statically linked library. But we need thread support due endless libevent dispatch loop.

CONFIG += shared thread release rtti qt

# for nullptr

QMAKE_CXXFLAGS += -std=gnu++11 

#-fpermissive


# The Qt section contains nothing here except core. LoRedis itself does not directly do network 
# functions and does not have any GUIs.

QT += core qml
QT -= network

LIBS += -lLoRedis -L../loredis/build 

#-lopencv_core -lopencv_video -lopencv_highgui -lopencv_imgproc

INCLUDEPATH += ../loredis/src /usr/include/libbson-1.0 /usr/include/libmongoc-1.0/

HEADERS += MongoStorage.h               \
        AbstractEntity.h                \
        AbstractAgent.h                 \
        Sprout.h                        \
        SproutableAgent.h               \
        ORer.h                          \
        ANDor.h                         \
        Task.h                          \
        SimpleRegulator.h               \
        Summator.h                      \
        Process.h                       \
        ProcessStart.h                  \
        ProcessStop.h                   \
        Processor.h                     \
        State.h                         \
        Vehicle.h
            
SOURCES += MongoIndex.cpp               \ 
        MongoStorage.cpp                \
        AbstractEntity.cpp              \
        AbstractAgent.cpp               \
        Sprout.cpp                      \
        SproutableAgent.cpp             \
        ORer.cpp                        \
        ANDor.cpp                       \
        Task.cpp                        \
        SimpleRegulator.cpp             \
        Summator.cpp                    \
        Process.cpp                     \
        ProcessStart.cpp                \
        ProcessStop.cpp                 \
        Processor.cpp                   \
        State.cpp                       \        
        Vehicle.cpp                     \
        AgentFactory.cpp            
        
