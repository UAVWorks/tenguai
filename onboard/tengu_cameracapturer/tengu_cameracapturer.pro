#######################################################################################################################
#                                                                                                                     #
#             Сборка проекта съема видео-изображения с камеры и публикации его через локальный редис.                 #
#                                                                                                                     #
# Eugene G. Sysoletin <unclesal@mail.ru>                                                    created 20 apr 2017 22:09 #
#######################################################################################################################

TEMPLATE = app
TARGET = build/tengu_cameracapturer

QT -= widgets gui
QT += core network

QMAKE_CXXFLAGS += -std=gnu++11

MOC_DIR=build/moc
UI_DIR=build/uic
RCC_DIR=build/obj
OBJECTS_DIR=build/obj

LIBS += -lopencv_core -lopencv_video -lopencv_highgui -lopencv_imgproc -ltengu_common -L../../common/ -lLoRedis -L../../loredis -levent -lhiredis

INCLUDEPATH += ../../common  ../  ../../loredis/src/


HEADERS += CameraCapturer.h

SOURCES += main.cpp                 \
    CameraCapturer.cpp              
