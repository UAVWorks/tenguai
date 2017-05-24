#include <QCoreApplication>

#include "RedisTester.h"

int main( int argc, char * argv[] ) {
    RedisTester * test = new RedisTester();
    QCoreApplication app( argc, argv );
    test->start();
    return app.exec();
};
