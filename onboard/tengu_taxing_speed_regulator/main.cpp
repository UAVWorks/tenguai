// ********************************************************************************************************************
// *                                                                                                                  *
// *              Main file for the taxing ground speed regulator, the part of Tengu - onboard program set.           *
// * ---------------------------------------------------------------------------------------------------------------- *
// *              Запускашка для регулятора скорости самолета относительно земли в фазе рулежки и разбега.            *
// *          Это - часть набора программ проекта Тенгу, предназначенных для выполнения непосредственно на борту.     *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 24 may 2017 at 08:23 *
// ********************************************************************************************************************

#include <QCoreApplication>
#include "TaxingSpeedRegulator.h"

int main ( int argc, char * argv[] ) {
        
    QCoreApplication app( argc, argv );
    tengu::TaxingSpeedRegulator * regulator = new tengu::TaxingSpeedRegulator();    
    return app.exec();    
    
};
