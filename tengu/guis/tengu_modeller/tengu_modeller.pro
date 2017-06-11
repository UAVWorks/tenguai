#######################################################################################################################
#                                                                                                                     #
#                                                    Modeller.                                                        #
#                             Qt GUI application for creation internal structure of agents.                           #
# ------------------------------------------------------------------------------------------------------------------- #
#                                                    Моделлер.                                                        #
#                       Графическое Qt приложение для создания внутренней структуры агентов.                          #
#                                                                                                                     #
# Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                          Created 27 may 2017 at 08:24 #
#######################################################################################################################

CONFIG += qt release thread rtti
QT += core gui widgets 
#opengl

QMAKE_CXXFLAGS += -std=gnu++11 

TARGET = build/tengu_modeller
TEMPLATE = app

MOC_DIR=build/moc
UI_DIR=build/uic
RCC_DIR=build/obj
OBJECTS_DIR=build/obj

INCLUDEPATH += src src/schema ../../core ../../loredis/src

LIBS += -lLoRedis -L../../loredis/build -ltengu_core -L../../core/build -lhiredis -levent

HEADERS  += src/schema/AbstractEntityItem.h \
        src/schema/TaskItem.h               \
        src/schema/SproutItem.h             \
        src/schema/VehicleItem.h            \
        src/MainWindow.h                    \
        src/MainWindowLeft.h                \
        src/AgentPropertyModel.h            \
        src/AgentPropertyView.h             \
        src/WorkSpace.h                     \
        src/MainWindowRight.h               \
        src/schema/SchemaScene.h            \
        src/schema/SchemaView.h      

SOURCES += src/main.cpp                     \
        src/schema/AbstractEntityItem.cpp   \
        src/schema/TaskItem.cpp             \
        src/schema/SproutItem.cpp           \
        src/schema/VehicleItem.cpp          \
        src/MainWindow.cpp                  \
        src/MainWindowLeft.cpp              \
        src/AgentPropertyModel.cpp          \
        src/AgentPropertyView.cpp           \
        src/WorkSpace.cpp                   \
        src/MainWindowRight.cpp             \
        src/schema/SchemaScene.cpp          \
        src/schema/SchemaView.cpp

RESOURCES += tengu_modeller.qrc



