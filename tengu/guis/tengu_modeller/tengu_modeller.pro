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

INCLUDEPATH += src src/schema src/overboard ../../core ../../loredis/src /usr/include/libbson-1.0 /usr/include/libmongoc-1.0/

LIBS += -lLoRedis -L../../loredis/build -ltengu_core -L../../core/build -lhiredis -levent -lmongoc-1.0 -L/usr/lib/x86_64-linux-gnu

HEADERS  += src/overboard/XPlaneAgent.h     \ 
        src/schema/AbstractEntityItem.h     \        
        src/schema/XPlaneAgentItem.h        \
        src/schema/ProcessStartItem.h       \
        src/schema/ProcessStopItem.h        \
        src/schema/ANDorItem.h              \
        src/schema/ORerItem.h               \
        src/schema/TaskItem.h               \
        src/schema/SproutItem.h             \
        src/schema/ProcessItem.h            \
        src/schema/VehicleItem.h            \
        src/LibraryTab_Button.h             \
        src/LibraryTab_Processes.h          \
        src/LibraryTab.h                    \
        src/MainWindow.h                    \
        src/MainWindowLeft.h                \
        src/AgentPropertyModel.h            \
        src/AgentPropertyView.h             \
        src/WorkSpace.h                     \
        src/MainWindowRight.h               \
        src/schema/SchemaScene.h            \
        src/schema/SchemaView.h      

SOURCES += src/main.cpp                     \
        src/overboard/XPlaneAgent.cpp       \
        src/schema/AbstractEntityItem.cpp   \
        src/schema/XPlaneAgentItem.cpp      \
        src/schema/ProcessStartItem.cpp     \
        src/schema/ProcessStopItem.cpp      \
        src/schema/ANDorItem.cpp            \
        src/schema/ORerItem.cpp             \
        src/schema/TaskItem.cpp             \
        src/schema/SproutItem.cpp           \
        src/schema/ProcessItem.cpp          \
        src/schema/VehicleItem.cpp          \
        src/schema/AgentItemFactory.cpp     \
        src/LibraryTab_Button.cpp           \
        src/LibraryTab_Processes.cpp        \
        src/LibraryTab.cpp                  \
        src/MainWindow.cpp                  \
        src/MainWindowLeft.cpp              \
        src/AgentPropertyModel.cpp          \
        src/AgentPropertyView.cpp           \
        src/WorkSpace.cpp                   \
        src/MainWindowRight.cpp             \
        src/schema/SchemaScene.cpp          \
        src/schema/SchemaView.cpp

RESOURCES += tengu_modeller.qrc



