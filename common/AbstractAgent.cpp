// ********************************************************************************************************************
// *                                                                                                                  *
// *      The abstract agent. Any logical completed piece which can do something. Usually it is a separate process    *
// *                                              inside operation system.                                            *
// * ---------------------------------------------------------------------------------------------------------------- *
// *    Абстрактный агент. Любой логически законченный кусок, который может что-то делать. Как правило - отдельный    *
// *                                             процесс операционной системы.                                        *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 26 may 2017 at 11:59 *
// ********************************************************************************************************************

#include "AbstractAgent.h"

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                    The constructor.                                              *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                      Конструктор.                                                *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::AbstractAgent::AbstractAgent ( QString name, QObject * parent ) 
    : QObject ( parent ) 
{
    _name = name;
    
    __pub_redis_connected = false;
    __sub_redis_connected = false;
    
    // Create publisher redis object
    // Создание публикатора редиса.
    
    __pub_redis = new LoRedis();
    QObject::connect( __pub_redis, SIGNAL( signalConnected() ), this, SLOT( __on_pub_redis_connected() ) );
    QObject::connect( __pub_redis, SIGNAL( signalDisconnected() ), this, SLOT( __on_pub_redis_disconnected() ) );
    QObject::connect( __pub_redis, SIGNAL( signalError(QString) ), this, SLOT( __on_redis_error( QString ) ) );
    __pub_redis->connect();
    
    // Create subscriber redis object
    // Создаем объект подписчика на редиса.
    
    __sub_redis = new LoRedis();
    QObject::connect( __sub_redis, SIGNAL( signalConnected() ), this, SLOT( __on_sub_redis_connected() ) );
    QObject::connect( __sub_redis, SIGNAL( signalDisconnected() ), this, SLOT( __on_sub_redis_disconnected() ) );
    QObject::connect( __sub_redis, SIGNAL( signalError(QString) ), this, SLOT( __on_redis_error( QString) ) );
    QObject::connect( __sub_redis, SIGNAL( signalGotMessage(QString, QString) ), this, SLOT( __on_got_message( QString, QString ) ) );
    __sub_redis->connect();
    
    __ping_timer = new QTimer( this );
    QObject::connect( __ping_timer, SIGNAL( timeout() ), this, SLOT( __on_ping_timer() ) );
    __ping_timer->start( 100 );
    
    __connect_timer = new QTimer();
    QObject::connect( __connect_timer, SIGNAL( timeout() ), this, SLOT( __on_connect_timer() ) );
    __connect_timer->start( 1000 );

}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                      The publisher was connected to redis.                                       *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                         Публикатор соединился с редисом.                                         *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractAgent::__on_pub_redis_connected() {
    __pub_redis_connected = true;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                      The subscriber was connected to redis.                                      *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                         Подписчик соединился с редисом.                                          *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractAgent::__on_sub_redis_connected() {
    __sub_redis_connected = true;
    // __subscribe();
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                    The publisher was disconnected from redis.                                    *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                        Публикатор рассоединился с редисом.                                       *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractAgent::__on_pub_redis_disconnected() {
    __pub_redis_connected = false;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                    The subscriber was disconnected from redis.                                   *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                         Подписчик рассоединился с редисом.                                       *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractAgent::__on_sub_redis_disconnected() {
    __sub_redis_connected = false;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                         Someone from redises have an error.                                      *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                        У кого-то из редисов возникла ошибка.                                     *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractAgent::__on_redis_error ( QString message ) {
    qDebug() << "AbstractRegulator::__on_error: " << message;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                     We got some message from subscriber redis.                                   *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                    Мы получили сообщение от редиса по подписке.                                  *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractAgent::__on_got_message ( QString channel, QString message ) {

    /*
    QString prefix = AbstractRegulator::prefix( _section );
    bool ok = false;
    if ( channel == prefix + "activity" ) {
        
        // We got an regulator's activity command
        // Мы получили команду активности регулятора
        
        int ia = message.toInt( & ok );
        __active = false;
        if ( ( message.toUpper() == "TRUE" ) || ( ( ok ) && ( ia != 0 ) ) )
            __active = true;
        
        return;
        
    } else if ( channel == _input_channel ) {
        
        // We got new input value
        // Мы получили новое входное значение.
        
        _input_value = message.toFloat();
        __do_step();
        return;
        
    } else if ( channel == _desired_channel ) {
        qDebug() << "Got desired value" << message; 
        
        // We got new desired value
        // Мы получили новое желаемое значение
        
        _desired_value = message.toFloat();
        __do_step();
        return;
        
    } else if ( channel == prefix + "steady_state" ) {
        
        float val = message.toFloat( & ok );
        if ( ok ) _pid->__set_steady_state( val );
        return;
        
    } else if ( channel == prefix + "P" ) {
        
        float p = message.toFloat( & ok ) ;
        if ( ok ) _pid->_params.P = p;            
        return;
        
    } else if ( channel == prefix + "I" ) {
        
        float i = message.toFloat( & ok );
        if ( ok ) _pid->_params.I = i;        
        return;
        
    } else if ( channel == prefix + "D" ) {
        
        float d = message.toFloat( & ok );
        if ( ok ) _pid->_params.D = d;
        return;
        
    } else {
        qDebug() << "Abstract regulator::__on_got_message(), channel " << channel << " unhandled.";
    };
    */
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                          On the ping timer fire event.                                           *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                               Тычок таймера пинга.                                               *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractAgent::__on_ping_timer() {
    
    if ( __pub_redis_connected ) {
        
        // Publish the last live time of this agent.
        // This time allows you to conclude whether the agent is running or not
        
        // Публикация последнего времени жизни этого агента.
        // Это время позволяет делать вывод, выполняется этот агент или нет.
        
        QDateTime dt = QDateTime::currentDateTime();
        QTime time = QTime::currentTime();
        QString repr = QString::number( dt.toTime_t() ) + "." + QString::number( time.msec() );
        // __pub_redis->publish( AbstractRegulator::prefix(_section) + "ping", repr );
    };
    
    // In any case, it does not matter whether we are connected or not.
    // This is enought to call only one static class method independed of 
    // quantity of real objects.
    
    // В любом случае, не имеет значения, соединены мы или нет.
    // Достаточно просто одного вызова статической функции класса, не надо по 
    // каждому из объектов.
    
    LoRedis::processEvents();
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                              Connection timer fired.                                             *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                         Срабатывание таймера соединения.                                         *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractAgent::__on_connect_timer() {
    if ( ! __pub_redis_connected ) __pub_redis->connect();
    if ( ! __sub_redis_connected ) __sub_redis->connect();
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                    The destructor.                                               *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                       Деструктор.                                                *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::AbstractAgent::~AbstractAgent() {

}


