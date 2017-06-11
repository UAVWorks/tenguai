#include <QCoreApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QStringList>
#include <QString>
#include <QObject>
#include <QDebug>

#include "Processor.h"
#include "Vehicle.h"

int main( int argc, char * argv[] ) {
    
    QCoreApplication app( argc, argv );
    
    
    QCommandLineParser parser;     
    tengu::Processor::prepareCommandLineParser( parser );
    parser.process( app );
    bool as_daemon = ! parser.isSet( tengu::Processor::noDaemonOption() );
    
    // Create "vehicle"
    // Порождение "самоходки".
    
    tengu::Vehicle vehicle;
    
    
    // Check "vehicle's" configuration parameters.
    // Проверка конфигурационных параметров "самоходки".
    
    if ( ! vehicle.configLoaded() ) {
        qDebug() << QObject::tr("Config file processor.conf | vehicle.conf not found.");
        return 1;
    };
    
    if ( !vehicle.configCorrect( true ) ) {
        qDebug() << QObject::tr("Configuration data is incorrect.");
        return 1;
    };
    
    // Check the "vehicle" does not running.
    // Проверка того факта, что "самоходка" еще не выполняется.
    
    return app.exec();
    
    /*
    if ( ! as_daemon ) 
    else {
        // Daemon execution mode.
        // Выполняемся - как демон.
                
    };
    */
};
