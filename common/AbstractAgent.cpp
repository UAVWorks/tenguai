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
    __activity = false;
    
    __pub_redis_connected = false;
    __sub_redis_connected = false;
    
    // Creating redis'es objects.
    // Создание редисовых объектов.
    
    _createRedises();
    
    // Binding redis object
    // Связка публикатора редиса.
    
    QObject::connect( _pub_redis, SIGNAL( signalConnected() ), this, SLOT( __on_pub_redis_connected() ) );
    QObject::connect( _pub_redis, SIGNAL( signalDisconnected() ), this, SLOT( __on_pub_redis_disconnected() ) );
    QObject::connect( _pub_redis, SIGNAL( signalError(QString) ), this, SLOT( __on_redis_error( QString ) ) );
    
    // Binding the subscriber redis object
    // Связка объекта подписчика на редиса.
    
    QObject::connect( _sub_redis, SIGNAL( signalConnected() ), this, SLOT( __on_sub_redis_connected() ) );
    QObject::connect( _sub_redis, SIGNAL( signalDisconnected() ), this, SLOT( __on_sub_redis_disconnected() ) );
    QObject::connect( _sub_redis, SIGNAL( signalError(QString) ), this, SLOT( __on_redis_error( QString) ) );
    QObject::connect( _sub_redis, SIGNAL( signalSubscribed(QString) ), this, SLOT( __on_subscribed( QString ) ) );
    QObject::connect( _sub_redis, SIGNAL( signalUnsubscribed(QString) ), this, SLOT( __on_unsubscribed( QString ) ) );
    QObject::connect( _sub_redis, SIGNAL( signalGotMessage(QString, QString) ), this, SLOT( __on_got_message( QString, QString ) ) );
    
    // The timers for agent.
    // Таймеры для этого агента.
    
    __ping_timer = new QTimer( this );
    QObject::connect( __ping_timer, SIGNAL( timeout() ), this, SLOT( __on_ping_timer() ) );
    __ping_timer->start( 100 );
    
    __connect_timer = new QTimer();
    QObject::connect( __connect_timer, SIGNAL( timeout() ), this, SLOT( __on_connect_timer() ) );
    __connect_timer->start( 1000 );
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                     Creating of redises objects. Override this procedure to change LoRedis class.                *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                   Создание объектов редиса. Перекройте эту функцию для изменения класса LoRedis.                 *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractAgent::_createRedises() {
    _pub_redis = new LoRedis();
    _sub_redis = new LoRedis();
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                      Public connect procedure. It should be called after full initialization.                    *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                   Публичная процедура соединения. Ее надо вызывать после полной инициализации.                   *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractAgent::connect() {
    
    // Connect redises if they exists.
    // Соединение редисеров, если они есть.
    
    if ( _pub_redis ) _pub_redis->connect();
    if ( _sub_redis ) _sub_redis->connect();
    
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
    _subscribe();
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
// *                                        Is the publisher connected to redis?                                      *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                          Соединен ли публикатор с редисом?                                       *
// *                                                                                                                  *
// ********************************************************************************************************************

bool tengu::AbstractAgent::isPublisherConnected() {
    return __pub_redis_connected;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                       Is the subscriber connected to redis?                                      *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                          Соединен ли подписчик с редисом?                                        *
// *                                                                                                                  *
// ********************************************************************************************************************

bool tengu::AbstractAgent::isSubscriberConnected() {
    return __sub_redis_connected;
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
// *                                      We has been subscribed to someone channel.                                  *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                          Мы подписались на какой-то канал.                                       *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractAgent::__on_subscribed ( QString channel ) {
    
    for ( int i=0; i<__reactions.length(); i++ ) {
        
        reaction_t * r = __reactions.at(i);
        
        if ( r->channel == channel ) {
            
            bool locked = __reMutex.tryLock( 300 );
            
            if ( locked ) {
                
                r->subscribed = true;
                __reMutex.unlock();                
                
            } else {
                qDebug() << "AbstractAgent::__on_subscribed(" << channel << "), could not lock reaction's mutex.";
            };
            
        };
    };
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                   We has been unsubscribed from someone channel.                                 *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                         Нас отписали от какого-то канала.                                        *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractAgent::__on_unsubscribed ( QString channel ) {
    
    for ( int i=0; i<__reactions.length(); i++ ) {
        reaction_t * r = __reactions.at(i);
        if ( r->channel == channel ) {
            bool locked = __reMutex.tryLock( 300 );
            if ( locked ) {
                r->subscribed = false;
                r->subscribtion_applicated = false;
                __reMutex.unlock();
            } else {
                qDebug() << "AbstractAgent::__on_UNsubscribed(" << channel << "), could not lock reaction's mutex.";
            }
        };
    };
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
// *                               Add the reaction which will be handled (processed) .                               *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                         Добавить обрабатываемую реакцию.                                         *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractAgent::addReaction ( tengu::AbstractAgent::reaction_t reaction ) {
    
    reaction_t * nr = new reaction_t();
    nr->channel = reaction.channel;
    nr->reaction = reaction.reaction;
    nr->subscribed = false;
    nr->subscribtion_applicated = false;
    bool locked = __reMutex.tryLock( 300 );
    if ( locked ) {
        __reactions.append( nr );
        __reMutex.unlock();
    } else {
        qDebug() << "AbstractAgent::addReaction(), could not lock the reaction's mutex.";
    };    
    
    _subscribe();
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                             Add reaction for specified channel if it is not empty                                *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                         Добавление реакции для определенного канала, если он не пустой.                          *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractAgent::addReactionFor( QString channel, reaction_callback_t reaction) {
    
    if ( channel.length() > 0 ) {
        reaction_t r;
        r.channel = channel;
        r.reaction = reaction;
        r.subscribed = false;
        r.subscribtion_applicated = false;
        addReaction( r );
    };
    
}


// ********************************************************************************************************************
// *                                                                                                                  *
// *                                      Remove the reaction from handled list.                                      *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                    Удалить реакцию из списка обрабатываемых.                                     *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractAgent::removeReaction ( tengu::AbstractAgent::reaction_t reaction ) {
    
    try {
        
        for ( int i=0; i<__reactions.length(); i++ ) {
            reaction_t * r = __reactions.at(i);
            if ( ( r->channel == reaction.channel ) && ( r->reaction == reaction.reaction ) ) {
                __reactions.removeAt( i );
                delete( r );
                return;
            }
        };        
        
    } catch ( ... ) {
        qDebug() << "AbstractAgent::removeReaction(), delete reaction_t struct error.";
    }    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                               Subscribe function.                                                *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                Функция подписки.                                                 *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractAgent::_subscribe() {
    
    if ( __sub_redis_connected ) {
        
        bool locked = __reMutex.tryLock( 300 );
        
        if ( locked ) {
            
            for ( int i=0; i<__reactions.length(); i++ ) {
                reaction_t * reaction = __reactions.at(i);
                
                // We here only look at submitting an application for a subscription, but not for the fact 
                // of subscription itself.
                
                // Здесь смотрим только на подачу заявки на подписку, но не на сам факт подписки.
                
                if ( ! reaction->subscribtion_applicated ) {
                    _sub_redis->subscribe( reaction->channel );
                    reaction->subscribtion_applicated = true;
                };
            };
        
            __reMutex.unlock();
            
        } else {
            // Error handler for AbstractAgent.
            // Обработка ошибок для абстрактного агента.
            qDebug() << "AbstractAgent::_subscribe(): could not lock the mutex.";
        };
    };
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                             Set activity for this agent.                                         *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                       Установка активности для данного агента.                                   *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractAgent::_setActivity(bool a) {
    __activity = a;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                             Is this agent active?                                                *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                        Является ли данный агент активным?                                        *
// *                                                                                                                  *
// ********************************************************************************************************************

bool tengu::AbstractAgent::isActive() {
    return __activity;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                      Abstract agent cannot be used directly.                                     *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                              Абстрактный агент не может быть использован напрямую.                               *
// *                                                                                                                  *
// ********************************************************************************************************************

bool tengu::AbstractAgent::usable() {
    return false;
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
        QString channel = QString("agents.") + _name + ".ping";
        _pub_redis->publish( channel, repr );
        _pub_redis->set( channel, repr );
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
    
    if ( ( ! __pub_redis_connected ) && ( _pub_redis ) ) _pub_redis->connect();
    if ( ( ! __sub_redis_connected ) && ( _sub_redis ) ) _sub_redis->connect();
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                    The destructor.                                               *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                       Деструктор.                                                *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::AbstractAgent::~AbstractAgent() {
    
    // The reactions has been add dynamically. The destructor must free memory.
    // Реакции были добавлены динамически. Деструктор должен освободить память.
    
    try {
        
        while ( __reactions.length() > 0 ) {
            reaction_t * r = __reactions.at(0);
            __reactions.removeAt( 0 );
            delete( r );
        };
        
    } catch ( ... ) {
        qDebug() << "AbstractAgent::AbstractAgent(), clean reaction error.";
    };
}


