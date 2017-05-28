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

QT += core gui widgets

TARGET = build/tengu_modeller
TEMPLATE = app

MOC_DIR=build/moc
UI_DIR=build/uic
RCC_DIR=build/obj
OBJECTS_DIR=build/obj


HEADERS  += MainWindow.h        \
        MainWindowLeft.h        \
        MainWindowSchema.h      

SOURCES += main.cpp             \
        MainWindow.cpp          \
        MainWindowLeft.cpp      \
        MainWindowSchema.cpp    


