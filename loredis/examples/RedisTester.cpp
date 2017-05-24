#include "RedisTester.h"

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                 Constructor.                                                     *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                 Конструктор.                                                     *
// *                                                                                                                  *
// ********************************************************************************************************************

RedisTester::RedisTester() 
    : QThread()
    , connected( false )
    , redis( new LoRedis() )
    , subscriber( new LoRedis() )
{
    QObject::connect( redis, SIGNAL( signalConnected() ), this, SLOT( onConnected()) );
    QObject::connect( redis, SIGNAL( signalDisconnected() ), this, SLOT( onDisconnected()) );
    QObject::connect( redis, SIGNAL( signalError(QString) ), this, SLOT( onError(QString)) );
    
    QObject::connect( subscriber, SIGNAL( signalSubscribed(QString) ), this, SLOT( onSubscribed(QString)) );
    QObject::connect( subscriber, SIGNAL( signalGotMessage(QString,QString)), this, SLOT( onGotMessage( QString, QString ) ) );
        
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                        the execution method of the thread.                                       *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                              Метод выполнения потока.                                            *
// *                                                                                                                  *
// ********************************************************************************************************************

void RedisTester::run() {
    
    qDebug() << "Connect to redis...";
    redis->connect();
    subscriber->connect();
    
    
    // --------------------------------------------------------------------------------------------
    //        You do not need to wait in a real program, of course. This is just for testing.
    //             In the real programm the processEvents() must be in another thread, 
    //                               the one for all LoRedis objects.
    // --------------------------------------------------------------------------------------------
    //         В реальной программе, конечно же, ждать - не нужно. Это только для тестирования. 
    //                 В реальной программе processEvents должен быть в другом потоке,
    //                               одном на все инстанции LoRedis'а.
    // --------------------------------------------------------------------------------------------
    
    do {
        
        LoRedis::processEvents(); 
        usleep(10); 
        
    } while ( ! connected );
    
    // --------------------------------------------------------------------------------------------
    //                          Keeping the value in the redis synchronously
    // --------------------------------------------------------------------------------------------
    //                            Сохраняем значение в редисе - синхронно. 
    // --------------------------------------------------------------------------------------------
    
    qDebug() << "Set value 20...";
    redis->set_sync("name", 20 );
    redis->set_sync("name2", "name2value");    
        
    // --------------------------------------------------------------------------------------------
    //                               Getting value from redis synchronously
    // --------------------------------------------------------------------------------------------
    //                         Получаем значение из редиса - синхронным способом. 
    // --------------------------------------------------------------------------------------------
    
    QVariant val = redis->get_sync("name");
    qDebug() << "Got value: " << val.toString();
    val = redis->get_sync("name2");
    qDebug() << "Got name2 value: " << val.toString();
            
    // Subscribe to the "test" channel.
    qDebug() << "Subscribe to the \"test\" channel...";
    subscriber->subscribe( "test" );    
    
    // Waiting one second to subscribe execution. This is more than enought.
    // Ждем одну секунду, чтобы подписка точно прошла. Этого - более чем достаточно.
    
    for ( int i = 0; i<100; i++ ) {
        LoRedis::processEvents();
        usleep(10);
    };
    
    qDebug() << "We should be subscribed, publish the message...";
    
    // --------------------------------------------------------------------------------------------
    //                                  Publish message to the test channel. 
    // --------------------------------------------------------------------------------------------
    //                                 Публикация сообщения в тестовый канал. 
    // --------------------------------------------------------------------------------------------
    
    redis->publish("test", "This is some pub/sub message" );
    
    // Waiting anain. The user should to see a message about gotted message.
    // Снова ждем. Пользователь должен увидеть надпись о полученном сообщени.
    
    for ( int i = 0; i<100; i++ ) {
        LoRedis::processEvents();
        usleep(10);
    };
    
    qDebug() << "Unsubscribe from test...";
    redis->unsubscribe("test");
    
    for ( int i = 0; i<100; i++ ) {
        LoRedis::processEvents();
        usleep(10);
    };
            
    qDebug() << "We hope you have received message. :-D RedisTester::run() done.";
    
    qDebug() << "Disconnect from Redis...";
    redis->disconnect();
    
    while ( connected ) { 
        usleep ( 10 ); 
        LoRedis::processEvents();
    };
    
    
    qApp->exit( 0 );
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                              slot on connect event.                                              *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                            Слот на событие соединения                                            *
// *                                                                                                                  *
// ********************************************************************************************************************

void RedisTester::onConnected() {
    qDebug() << "--> We are connected to Redis server.";
    connected = true;

}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                             Slot on disconnect event.                                            *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                            Слот на событие рассоединения                                         *
// *                                                                                                                  *
// ********************************************************************************************************************

void RedisTester::onDisconnected() {
    qDebug() << "--> We are disconnected from redis server.";
    connected = false;

}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                          Slot if we have had an errors                                           *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                          Слот, если у нас были ошибки.                                           *
// *                                                                                                                  *
// ********************************************************************************************************************

void RedisTester::onError ( QString message ) {
    qDebug() << "--> We have an error: " << message ;
    
    // -----------------------------------------------------
    //          the quit, if any error occured
    // -----------------------------------------------------
    //             Если были ошибки - выходим.
    // -----------------------------------------------------
    
    qApp->exit(1);
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                               The confirmation that we are subscribed to channel                                 *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                Подтверждение того, что мы подписались на канал.                                  *
// *                                                                                                                  *
// ********************************************************************************************************************

void RedisTester::onSubscribed ( QString channel ) {
    qDebug() << "--> we are subscribed to " << channel << " channel";
}   

// ********************************************************************************************************************
// *                                                                                                                  *
// *                       We received the message from the channel which we have subsribed to                        *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                           Мы получили сообщение с канала, на который мы подписаны.                               *
// *                                                                                                                  *
// ********************************************************************************************************************

void RedisTester::onGotMessage ( QString channel, QString message ) {
    qDebug() << "--> We have received message from subscribed channel " << channel << ". Message is: " << message;
}


RedisTester::~RedisTester() {

}

