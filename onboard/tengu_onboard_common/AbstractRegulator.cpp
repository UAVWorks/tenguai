// ********************************************************************************************************************
// *                                                                                                                  *
// *                                             Abstract reguator class.                                             *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                          Класс абстрактного регулятора.                                          *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 24 may 2017 at 10:59 *
// ********************************************************************************************************************

#include "AbstractRegulator.h"

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                 The constructor.                                                 *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                   Конструктор.                                                   *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::AbstractRegulator::AbstractRegulator( QString section, float min_value, float max_value ) 
    : QObject() 
{
    _section = section;
    
    CREATE_SETTINGS_ONBOARD;
    settings_onboard.beginGroup( _section );
    _output_channel = settings_onboard.value( "output_channel", "" ).toString();
    settings_onboard.endGroup();
    
    __pub_redis_connected = false;
    __sub_redis_connected = false;
    
    // The PID-regulator and it's values.
    // ПИД-регулятор и его значения.
    
    _pid = new PID( section, 0.0, min_value, max_value );
    _input_value = 0.0;
    _desired_value = 0.0;
    __active = false;
    
    // Create publisher redis object
    // Создание публикатора редиса.
    
    __pub_redis = new LoRedis();
    QObject::connect( __pub_redis, SIGNAL( signalConnected() ), this, SLOT( __on_pub_redis_connected() ) );
    QObject::connect( __pub_redis, SIGNAL( signalDisconnected() ), this, SLOT( __on_pub_redis_disconnected() ) );
    QObject::connect( __pub_redis, SIGNAL( signalError(QString) ), this, SLOT( __on_error( QString ) ) );
    __pub_redis->connect();
    
    // Create subscriber redis object
    // Создаем объект подписчика на редиса.
    
    __sub_redis = new LoRedis();
    QObject::connect( __sub_redis, SIGNAL( signalConnected() ), this, SLOT( __on_sub_redis_connected() ) );
    QObject::connect( __sub_redis, SIGNAL( signalDisconnected() ), this, SLOT( __on_sub_redis_disconnected() ) );
    QObject::connect( __sub_redis, SIGNAL( signalError(QString) ), this, SLOT( __on_error( QString) ) );
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

void tengu::AbstractRegulator::__on_pub_redis_connected() {
    __pub_redis_connected = true;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                      The subscriber was connected to redis.                                      *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                         Подписчик соединился с редисом.                                          *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractRegulator::__on_sub_redis_connected() {
    __sub_redis_connected = true;
    __subscribe();
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                    The publisher was disconnected from redis.                                    *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                        Публикатор рассоединился с редисом.                                       *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractRegulator::__on_pub_redis_disconnected() {
    __pub_redis_connected = false;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                    The subscriber was disconnected from redis.                                   *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                         Подписчик рассоединился с редисом.                                       *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractRegulator::__on_sub_redis_disconnected() {
    __sub_redis_connected = false;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                         Someone from redises have an error.                                      *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                        У кого-то из редисов возникла ошибка.                                     *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractRegulator::__on_error ( QString message ) {
    qDebug() << "AbstractRegulator::__on_error: " << message;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                          On the ping timer fire event.                                           *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                               Тычок таймера пинга.                                               *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractRegulator::__on_ping_timer() {
    
    if ( __pub_redis_connected ) {
        
        // Publish the last live time of this regulator.
        // Публикация последнего времени жизни этого регулятора.
        
        QDateTime dt = QDateTime::currentDateTime();
        QTime time = QTime::currentTime();
        QString repr = QString::number( dt.toTime_t() ) + "." + QString::number( time.msec() );
        __pub_redis->publish( AbstractRegulator::prefix(_section) + "ping", repr );
    };
    
    LoRedis::processEvents();
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                              Connection timer fired.                                             *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                         Срабатывание таймера соединения.                                         *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractRegulator::__on_connect_timer() {
    if ( ! __pub_redis_connected ) __pub_redis->connect();
    if ( ! __sub_redis_connected ) __sub_redis->connect();
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                    Get redis prefix for this regulator's messages.                               *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                  Получить префикс сообщений этого регулятора в редисе.                           *
// *                                                                                                                  *
// ********************************************************************************************************************

QString tengu::AbstractRegulator::prefix( QString section ) {
    return ( QString("tengu.regulators.") + section.toLower() + "." );    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                   Subscribe on the common regulator's channels.                                  *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                      Подписка на общие каналы регуляторов.                                       *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractRegulator::__subscribe() {
    
    if ( ( __sub_redis ) && ( __sub_redis_connected ) ) {
        
        // The activity of this regulator.
        // Активность этого регулятора.
        
        __sub_redis->subscribe( AbstractRegulator::prefix( _section ) + "activity" );
        
        CREATE_SETTINGS_ONBOARD;
        settings_onboard.beginGroup( _section );
        _input_channel = settings_onboard.value( "input_channel", "").toString();
        _desired_channel = settings_onboard.value( "desired_channel", "" ).toString();
        qDebug() << "Desired channel=" << _desired_channel ;
        settings_onboard.endGroup();
        
        // Input value for this regulator
        // Входное значение для этого регулятора.
        
        if ( _input_channel.length() > 0 )    __sub_redis->subscribe( _input_channel );
        
        // Desired value for this regulator.
        // Желаемое значение для данного регулятора.
        
        if ( _desired_channel.length() > 0 ) __sub_redis->subscribe( _desired_channel );
        
        // Channels for P-I-D on-the-fly changes
        // Каналы для установки значений P-I-D "на лету".
        
        __sub_redis->subscribe( AbstractRegulator::prefix( _section ) + "P" );
        __sub_redis->subscribe( AbstractRegulator::prefix( _section ) + "I" );
        __sub_redis->subscribe( AbstractRegulator::prefix( _section ) + "D" );
        
    };
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                          Do one step for this regulator.                                         *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                       Сделать один шаг данного регулятора.                                       *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractRegulator::__do_step() {
        
    if ( ( __active ) && ( _output_channel.length() > 0 ) && ( __pub_redis ) && ( __pub_redis_connected ) ) {
        
        float oval = _pid->step( _input_value, _desired_value );
        qDebug() << "Publish value " << oval;
        __pub_redis->publish( _output_channel, QString::number(oval) );
        
    };
    
}


// ********************************************************************************************************************
// *                                                                                                                  *
// *                                     We got some message from subscriber redis.                                   *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                    Мы получили сообщение от редиса по подписке.                                  *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractRegulator::__on_got_message ( QString channel, QString message ) {
        
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
}


// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                  The destructor.                                                 *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                     Деструктор.                                                  *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::AbstractRegulator::~AbstractRegulator() {

}


