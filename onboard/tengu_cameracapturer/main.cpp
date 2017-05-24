// ********************************************************************************************************************
// *                  Запускашка программы снятия видео с камеры и публикации изображения в редисе.                   *
// *                                                                                                                  *
// * Eugene G. Sysoletin <unclesal@mail.ru>                                              created 20 apr 2017 at 22:15 *
// ********************************************************************************************************************

#include <QCoreApplication>
#include <QDebug>

#include "CameraCapturer.h"


using namespace tengu_onboard;
    
int main( int argc, char * argv[] ) {
    
    QCoreApplication app( argc, argv );
    CameraCapturer * capturer = new CameraCapturer();
    if ( capturer->isOpened() ) {
        capturer->start();
        return app.exec();
    } else {
        qDebug() << "Can't open camera, exit.";
        return -1;
    };
};
