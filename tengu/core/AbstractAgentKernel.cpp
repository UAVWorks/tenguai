// ********************************************************************************************************************
// *                                                                                                                  *
// *    The "kernel" of abstract agent. Class has been introduced to automatically work of properties (sprouts).      *
// * ---------------------------------------------------------------------------------------------------------------- *
// *  "Ядро" абстрактного агента. Класс был введен для автоматической работы "свойств" (они же "отростки" - Sprout).  *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 27 may 2017 at 08:31 *
// ********************************************************************************************************************

#include "AbstractAgentKernel.h"

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                 The constructor.                                                 *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                    Конструктор.                                                  *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::AbstractAgentKernel::AbstractAgentKernel()
    : QObject()
    , AbstractEntity()
{
        
    _parent = nullptr;
        
    _parent_uuid = QString("");
    // _subProcessPath = QString("");
    
    _pub_redis = nullptr;
    _sub_redis = nullptr;
    
    __pub_redis_connected = false;
    __sub_redis_connected = false;
    
    _previousByFocus = QMap < QString, AbstractAgentKernel * > ();
    _nextByFocus = QMap < QString, AbstractAgentKernel * > ();
    
    _children = QMap< QString, AbstractAgentKernel * > ();
    
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
  
    // The timers for agent.
    // Таймеры для этого агента.
    
    __ping_timer = new QTimer( this );
    QObject::connect( __ping_timer, SIGNAL( timeout() ), this, SLOT( __on_ping_timer() ) );
    __ping_timer->start( 100 );
    
    __connect_timer = new QTimer();
    QObject::connect( __connect_timer, SIGNAL( timeout() ), this, SLOT( __on_connect_timer() ) );
    __connect_timer->start( 1000 );        
    
    __must_be_connected = false;
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                     Creating of redises objects. Override this procedure to change LoRedis class.                *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                   Создание объектов редиса. Перекройте эту функцию для изменения класса LoRedis.                 *
// *                                                                                                                  *
// ********************************************************************************************************************


void tengu::AbstractAgentKernel::_createRedises() {
    
    _pub_redis = new LoRedis();
    _sub_redis = new LoRedis();
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                    Callback procedure after got some value from redis.                           *
// *                         It is empty in this class and should be overriden in the descendants.                    *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                           Callback-процедура после получения некоторого значения из редиса.                      *
// *                           Она в данном классе пустая, ее надо бы перекрывать в потомках.                         *
// *                                                                                                                  *
// ********************************************************************************************************************

/*
void tengu::AbstractAgentKernel::_got_value( QString sproutName ) {
}
*/

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                              Connection timer fired.                                             *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                         Срабатывание таймера соединения.                                         *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractAgentKernel::__on_connect_timer() {

    if ( ! __must_be_connected ) return;
    
    if ( ( ! __pub_redis_connected ) && ( _pub_redis ) ) {
        _pub_redis->connect();
    };
    
    if ( ( ! __sub_redis_connected ) && ( _sub_redis ) ) {
        _sub_redis->connect();
    };
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                          On the ping timer fire event.                                           *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                               Тычок таймера пинга.                                               *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractAgentKernel::__on_ping_timer() {
        
    if ( ( __pub_redis_connected ) && ( ! getUUID().isEmpty() ) ) {
        
        // Publish the last live time of this agent.
        // This time allows you to conclude whether the agent is running or not
        
        // Публикация последнего времени жизни этого агента.
        // Это время позволяет делать вывод, выполняется этот агент или нет.
        
        QDateTime dt = QDateTime::currentDateTime();
        QTime time = QTime::currentTime();
        
        // Time representation with ms.
        // Представление времени с милисекундами.
        
        QString repr = QString::number( dt.toTime_t() ) + "." + QString::number( time.msec() );
        
        // If we have an UUID - we will identify using it. 
        // Если есть UUID - идентификация идет по нему. 
                       
        QString channel = QString("agents.") + getUUID() + ".ping";
        
        // Was commented. I use yet only setting value into redis, but not pub/sub.
        // Было закомментировано. Использую только значение в редисе, но не pub/sub.
        // _pub_redis->publish( channel, repr );
        
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
// *                                      The publisher was connected to redis.                                       *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                         Публикатор соединился с редисом.                                         *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractAgentKernel::__on_pub_redis_connected() {
    __pub_redis_connected = true;
    qDebug() << "on pub redis connected";
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                      The subscriber was connected to redis.                                      *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                         Подписчик соединился с редисом.                                          *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractAgentKernel::__on_sub_redis_connected() {
    __sub_redis_connected = true;    
    qDebug() << "on sub redis connected";
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                    The publisher was disconnected from redis.                                    *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                        Публикатор рассоединился с редисом.                                       *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractAgentKernel::__on_pub_redis_disconnected() {
    __pub_redis_connected = false;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                    The subscriber was disconnected from redis.                                   *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                         Подписчик рассоединился с редисом.                                       *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractAgentKernel::__on_sub_redis_disconnected() {
    __sub_redis_connected = false;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                         Someone from redises have an error.                                      *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                        У кого-то из редисов возникла ошибка.                                     *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractAgentKernel::__on_redis_error ( QString message ) {
    qDebug() << "AbstractAgentKernel::__on_error: " << message;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                     Something has been changed in this agent.                                    *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                         Что-то в данном агенте изменилось.                                       *
// *                                                                                                                  *
// ********************************************************************************************************************


void tengu::AbstractAgentKernel::_somethingChanged() {
    
    AbstractEntity::_somethingChanged();
    emit signalSomethingChanged();
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                        Is the publisher connected to redis?                                      *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                          Соединен ли публикатор с редисом?                                       *
// *                                                                                                                  *
// ********************************************************************************************************************

bool tengu::AbstractAgentKernel::isPublisherConnected() {
    return __pub_redis_connected;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                       Is the subscriber connected to redis?                                      *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                          Соединен ли подписчик с редисом?                                        *
// *                                                                                                                  *
// ********************************************************************************************************************

bool tengu::AbstractAgentKernel::isSubscriberConnected() {
    return __sub_redis_connected;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                      Abstract agent cannot be used directly.                                     *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                              Абстрактный агент не может быть использован напрямую.                               *
// *                                                                                                                  *
// ********************************************************************************************************************

bool tengu::AbstractAgentKernel::usable() {
    return false;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                      Public connect procedure. It should be called after full initialization.                    *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                   Публичная процедура соединения. Ее надо вызывать после полной инициализации.                   *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractAgentKernel::connect() {
    
    qDebug() << "AbstractAgentKernel::connect()";
    
    __must_be_connected = true;
    // Connect redises if they exists.
    // Соединение редисеров, если они есть.
    
    if ( _pub_redis ) _pub_redis->connect();
    if ( _sub_redis ) _sub_redis->connect();
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                               Disconnect from RedisIO.                                           *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                Рассоединение с редисом.                                          *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractAgentKernel::disconnect() {
    qDebug() << "Abstract agent kernel :: disconnect()";
    __must_be_connected = false;
    if ( _pub_redis ) _pub_redis->disconnect();
    if ( _sub_redis ) _sub_redis->disconnect();
}


// ********************************************************************************************************************
// *                                                                                                                  *
// *                                           Add child to tree-like structure.                                      *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                       Добавить ребенка в древовидную структуру.                                  *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractAgentKernel::addChild ( tengu::AbstractAgentKernel * child ) {
    _children[ child->getUUID() ] = child;
    _changed = true;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                Path for process to provide this agent as a separate process in the operation system.             *
// * ---------------------------------------------------------------------------------------------------------------- *
// *     Путь для процесса, который предоставляет данного агента как отдельный процесс в операционной системе.        *
// *                                                                                                                  *
// ********************************************************************************************************************

/*
QString tengu::AbstractAgentKernel::subProcessPath() {
    return _subProcessPath;
}
*/

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                          Has this agent any of children?                                         *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                           Есть ли дети у данного агента?                                         *
// *                                                                                                                  *
// ********************************************************************************************************************

bool tengu::AbstractAgentKernel::hasChildren() {
    return _children.count() > 0;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                      Adding previous (in the sence of focus transition flow) agent.                              *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                  Добавление предыдущего (в смысле течения потока передачи фокуса) агента.                        *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractAgentKernel::addPreviousByFocus ( tengu::AbstractAgentKernel* previous ) {
    _previousByFocus[ previous->getUUID() ] = previous;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                  Remove previous (in sence of focus) agent.                                      *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                           Удаление соседа по фокусу.                                             *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractAgentKernel::removeNeighborByFocus ( tengu::AbstractAgentKernel * agent ) {
    if ( _previousByFocus.contains( agent->getUUID() ) ) _previousByFocus.remove( agent->getUUID() );
    if ( _nextByFocus.contains( agent->getUUID() ) ) _nextByFocus.remove( agent->getUUID() );
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                              Adding next (in the sence of focus flow) agent.                                     *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                       Добавление следующего (в смысле потока передачи фокуса) агента.                            *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractAgentKernel::addNextByFocus ( tengu::AbstractAgentKernel* next ) {
    _nextByFocus[ next->getUUID() ] = next;
}


// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                  The destructor.                                                 *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                     Деструктор.                                                  *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::AbstractAgentKernel::~AbstractAgentKernel() {
        
    // The children are stored right here. We must clean them.
    // Дети хранятся прямо здесь и их нужно чистить.
    
    if ( _children.size() > 0 ) {
        
        foreach ( AbstractAgentKernel * child, _children ) {
            delete( child );
        };
        
    };
    
}

