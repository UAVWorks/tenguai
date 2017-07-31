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
QT += core gui widgets qml quick
#opengl

QMAKE_CXXFLAGS += -std=gnu++11 -fpermissive

TARGET = build/tengu_modeller
TEMPLATE = app

MOC_DIR=build/moc
UI_DIR=build/uic
RCC_DIR=build/obj
OBJECTS_DIR=build/obj

INCLUDEPATH += src src/dialogs src/schema src/overboard ../../core ../../loredis/src /usr/include/libbson-1.0 /usr/include/libmongoc-1.0/ ../../../simulators/xplane/xtengu/src/

LIBS += -lLoRedis -L../../loredis/build -ltengu_core -L../../core/build -lhiredis -levent -lmongoc-1.0 -L/usr/lib/x86_64-linux-gnu

HEADERS  += src/overboard/XPlaneAircraft.h                      \ 
        src/overboard/XPlaneProcess.h                           \
        src/overboard/XPlaneSimulator.h                         \
        src/overboard/SproutProxy.h                             \
        src/overboard/TopLevelEntity.h                          \
        src/overboard/WorkSpace.h                               \
        src/schema/AbstractEntityItem.h                         \
        src/schema/TopLevelItem.h                               \
        src/schema/ItemWithLinks.h                              \
        src/schema/ResizeableItemWithLinks.h                    \
        src/schema/XPlaneAgentItem.h                            \
        src/schema/XPlaneSimulatorItem.h                        \
        src/schema/ProcessStartItem.h                           \
        src/schema/ProcessStopItem.h                            \
        src/schema/ANDorItem.h                                  \
        src/schema/ORerItem.h                                   \
        src/schema/TaskItem.h                                   \
        src/schema/SproutItem.h                                 \
        src/schema/LinkItem.h                                   \
        src/schema/ProcessItem.h                                \
        src/schema/VehicleItem.h                                \
        src/schema/WorkSpaceItem.h                              \
        src/schema/NotSavedOperation.h                          \
        src/LibraryTab_Button.h                                 \
        src/LibraryTab_Processes.h                              \
        src/LibraryTab.h                                        \
        src/MainWindow.h                                        \
        src/TreeStructure.h                                     \
        src/MainWindowLeft.h                                    \
        src/AgentPropertyElement.h                              \
        src/AgentPropertyDelegate.h                             \
        src/AgentPropertyModel.h                                \
        src/AgentPropertyView.h                                 \ 
        src/MainWindowRight.h                                   \
        src/schema/SchemaScene.h                                \
        src/schema/SchemaView.h                                 \
        src/dialogs/QMLHighlighter.h                            \
        src/dialogs/DialogWithButtons.h                         \
        src/dialogs/DialogProperties.h                          \
        src/dialogs/DialogPropertiesSprout.h                    \
        src/dialogs/DialogPropertiesTask_TabStartCondition.h    \
        src/dialogs/DialogPropertiesTask_TabStopCondition.h     \
        src/dialogs/DialogPropertiesTask_TabAlgorythm.h         \
        src/dialogs/DialogPropertiesTask_TabQML.h               \
        src/dialogs/DialogPropertiesTask.h                      \
        src/dialogs/DialogOpenSaveModel.h

SOURCES += src/main.cpp                                         \
        src/overboard/XPlaneAircraft.cpp                        \
        src/overboard/XPlaneProcess.cpp                         \
        src/overboard/XPlaneSimulator.cpp                       \
        src/overboard/SproutProxy.cpp                           \
        src/overboard/TopLevelEntity.cpp                        \
        src/overboard/WorkSpace.cpp                             \
        src/schema/AbstractEntityItem.cpp                       \
        src/schema/TopLevelItem.cpp                             \
        src/schema/ItemWithLinks.cpp                            \
        src/schema/ResizeableItemWithLinks.cpp                  \
        src/schema/XPlaneAgentItem.cpp                          \
        src/schema/XPlaneSimulatorItem.cpp                      \
        src/schema/ProcessStartItem.cpp                         \
        src/schema/ProcessStopItem.cpp                          \
        src/schema/ANDorItem.cpp                                \
        src/schema/ORerItem.cpp                                 \
        src/schema/TaskItem.cpp                                 \
        src/schema/SproutItem.cpp                               \
        src/schema/LinkItem.cpp                                 \
        src/schema/ProcessItem.cpp                              \
        src/schema/VehicleItem.cpp                              \
        src/schema/WorkSpaceItem.cpp                            \
        src/schema/AgentItemFactory.cpp                         \
        src/schema/NotSavedOperation.cpp                        \
        src/LibraryTab_Button.cpp                               \
        src/LibraryTab_Processes.cpp                            \
        src/LibraryTab.cpp                                      \
        src/MainWindow.cpp                                      \
        src/TreeStructure.cpp                                   \
        src/MainWindowLeft.cpp                                  \
        src/AgentPropertyElement.cpp                            \
        src/AgentPropertyDelegate.cpp                           \
        src/AgentPropertyModel.cpp                              \
        src/AgentPropertyView.cpp                               \      
        src/MainWindowRight.cpp                                 \
        src/schema/SchemaScene.cpp                              \
        src/schema/SchemaView.cpp                               \
        src/dialogs/QMLHighlighter.cpp                          \
        src/dialogs/DialogWithButtons.cpp                       \
        src/dialogs/DialogProperties.cpp                        \
        src/dialogs/DialogPropertiesSprout.cpp                  \
        src/dialogs/DialogPropertiesTask_TabStartCondition.cpp  \
        src/dialogs/DialogPropertiesTask_TabStopCondition.cpp   \
        src/dialogs/DialogPropertiesTask_TabAlgorythm.cpp       \
        src/dialogs/DialogPropertiesTask_TabQML.cpp             \
        src/dialogs/DialogPropertiesTask.cpp                    \
        src/dialogs/DialogOpenSaveModel.cpp

RESOURCES += tengu_modeller.qrc
