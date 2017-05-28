#######################################################################################################################
# Eng:                                                                                                                #
#              qmake-based project file for building LoRedis, the yet another C++ hiredis wrapper.                    #
#      LoRedis is the part of Tengu, the Artifical Intelligence research project. But you can use it as you wish,     #
#            under the GPLv3 terms. The building process is standard, just do qmake and then make command.            #
#                                                                                                                     #
# Ru:                                                                                                                 #
#          LoRedis - это основанный на Qt еще один C++ клиент для hiredis, официального API базы данных Redis.        #
#         Вообще говоря, LoRedis является частью большого проекта Tengu, посвященного искусственному интеллекту.      #
#    Но Вы можете использовать его отдельно, самостоятельно и так, как оно Вам заблагорассудится - в рамках терминов  #
#                          свободной лицензии GPLv3. Сборка - стандартная, qmake и потом make.                        #
#                                                                                                                     #
#                                                                                                                     #
#                                                                      Eugene G. Sysoletin <e.g.sysoletin@gmail.com>, #
#                                                          Ural Federal University, the Institute of radioelectronics #
#                                                                           and information tehnology, IT department. #
#                                                                                                                     #
#                                                                                        created 24 apr 2017 at 16:45 #
#######################################################################################################################


TEMPLATE = lib
TARGET = LoRedis

# Only for file arranging.

MOC_DIR=build/moc
UI_DIR=build/uic
RCC_DIR=build/obj
OBJECTS_DIR=build/obj


# You can install builded static lib to /usr/local

isEmpty(PREFIX) {
 PREFIX = /usr/local
}

libtarget.path = $$PREFIX/lib/
libtarget.files = libLoRedis.a

libheaders.path = $$PREFIX/include/
libheaders.files = src/LoRedis.h src/LoRedisReactor.h

INSTALLS += libtarget libheaders

# We want statically linked library. But we need thread support due endless libevent dispatch loop.

CONFIG += staticlib thread release rtti qt

# for nullptr

QMAKE_CXXFLAGS += -std=gnu++11


# The Qt section contains nothing here except core. LoRedis itself does not directly do network 
# functions and does not have any GUIs.

QT += core
QT -= network core

# To build this static library, you need libhiredis-dev and libevent-dev installed
# on your host.

LIBS += -lhiredis -levent

HEADERS = src/LoRedis.h 
SOURCES = src/LoRedis.cpp 

