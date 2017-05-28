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

TARGET = tengu_modeller
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp

HEADERS  += MainWindow.h

FORMS    += MainWindow.ui
