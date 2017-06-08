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
QT += core gui widgets opengl

QMAKE_CXXFLAGS += -std=gnu++11 

TARGET = build/tengu_modeller
TEMPLATE = app

MOC_DIR=build/moc
UI_DIR=build/uic
RCC_DIR=build/obj
OBJECTS_DIR=build/obj

INCLUDEPATH += items ../../core ../../loredis/src

LIBS += -lLoRedis -L../../loredis/build -ltengu_core -L../../core/build -lhiredis -levent

HEADERS  += items/AbstractAgentItem.h       \ 
        items/VehicleItem.h                 \
        MainWindow.h                        \
        MainWindowLeft.h                    \
        AgentPropertyModel.h                \
        AgentPropertyView.h                 \
        GUIModel.h                          \
        MainWindowRight.h                   \
        MainWindowSchema.h      

SOURCES += main.cpp                         \
        items/AbstractAgentItem.cpp         \
        items/VehicleItem.cpp               \
        MainWindow.cpp                      \
        MainWindowLeft.cpp                  \
        AgentPropertyModel.cpp              \
        AgentPropertyView.cpp               \
        GUIModel.cpp                        \
        MainWindowRight.cpp                 \
        MainWindowSchema.cpp    

RESOURCES += tengu_modeller.qrc



