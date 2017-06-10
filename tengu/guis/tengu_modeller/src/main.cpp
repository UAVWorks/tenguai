// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                   Modeller.                                                      *
// *                           Qt GUI application for creation internal structure of agents.                          *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                   Моделлер.                                                      *
// *                      Графическое Qt приложение для создания внутренней структуры агентов.                        *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 27 may 2017 at 08:24 *
// ********************************************************************************************************************

#include "MainWindow.h"
#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[]) {

    // QApplication::setStyle( QStyleFactory::create("Fusion") );
    QApplication a(argc, argv);
    
    tengu::MainWindow w;
    w.show();

    return a.exec();
}
