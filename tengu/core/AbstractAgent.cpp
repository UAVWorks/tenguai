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

tengu::AbstractAgent::AbstractAgent () 
    : AbstractAgentKernel () 
{    
    _oActivityChannel = QString("");
    _iFocusChannel = QString("");
    // _oFocusChannel = QString("");
    
    __focus = false;
    __activity = false;
    
    // Additional reaction for subscriber
    if ( _sub_redis ) {
        
        QObject::connect( _sub_redis, SIGNAL( signalConnected() ), this, SLOT( __on_subscriber_connected() ) );
        QObject::connect( _sub_redis, SIGNAL( signalSubscribed( QString ) ), this, SLOT( __on_subscribed( QString ) ) );
        QObject::connect( _sub_redis, SIGNAL( signalUnsubscribed( QString ) ), this, SLOT( __on_unsubscribed( QString ) ) );
        QObject::connect( _sub_redis, SIGNAL( signalGotMessage( QString, QString ) ), this, SLOT( __on_got_message( QString, QString ) ) );
    };    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                           Subscriber redis has been connected.                                   *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                              Редис-подписчик - соединился.                                       *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractAgent::__on_subscriber_connected() {
    qDebug() << "AbstractAgent::on subscriber connected";
    __subscribe();
};

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                      We has been subscribed to someone channel.                                  *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                          Мы подписались на какой-то канал.                                       *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractAgent::__on_subscribed ( QString channel ) {
    
    foreach ( Sprout * sprout, __sprouts ) {
        sprout->subscribed( channel );
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
    
    foreach ( Sprout * sprout, __sprouts ) {
        sprout->unsubscribed( channel );
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

    bool handled = false;
    
    foreach ( Sprout * sprout, __sprouts ) {
        bool res = sprout->handleMessage( channel, message );
        if ( res ) {
            QString sproutName = sprout->getSystemName();
            emit signalSproutGotValue( sproutName, sprout->getValue() );
            handled = true;
        };
    }    
    
    if ( ! handled ) {
        qDebug() << "AbstractAgent::__on_got_message(" << channel << ") was not handled.";
    }
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                       This function performs to make decision about activation of this channel                   *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                        Функция выполняется для принятия решения об активации данного канала.                     *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractAgent::__tryActivate() {
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                           Got message from activity channel.                                     *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                        Получено сообщение из канала активности.                                  *
// *                                                                                                                  *
// ********************************************************************************************************************
/*
void tengu::AbstractAgent::__on_activity_channel_message ( QVariant value ) {
    
    bool ok = false;
    QString msg = value.toString();
    int i = msg.toInt( & ok );
    if ( ( ok ) && ( i != 0 ) ) _setActivity( true );
    else if ( msg.toUpper() == "TRUE" ) _setActivity( true );
    else _setActivity( value.toBool() );
    
}
*/

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                         Disconnect this agent from RedisIO                                       *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                               Рассоединиться с редисом.                                          *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractAgent::disconnect() {
    __unsubscribe();
    tengu::AbstractAgentKernel::disconnect();
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                             Set activity for this agent.                                         *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                       Установка активности для данного агента.                                   *
// *                                                                                                                  *
// ********************************************************************************************************************

// void tengu::AbstractAgent::_setActivity(bool a) {
//    __activity = a;
// }

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
// *                                               Is this agent focused?                                             *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                             В фокусе ли данный агент?                                            *
// *                                                                                                                  *
// ********************************************************************************************************************

bool tengu::AbstractAgent::isFocused() {
    return __focus;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                           Subscribe all existing sprouts.                                        *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                            Подписать всех имеющихся реакторов ("веточки" данного агента).                        *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractAgent::__subscribe() {
    
    foreach ( Sprout * sprout, __sprouts) {
        sprout->subscribe();
    };
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                          Unsubscribe from redis messages.                                        *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                            отписаться от событий редиса.                                         *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractAgent::__unsubscribe() {
    foreach( Sprout * sprout, __sprouts ) {
        sprout->unsubscribe();
    };
}


// ********************************************************************************************************************
// *                                                                                                                  *
// *                               Add the reaction which will be handled (processed) .                               *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                         Добавить обрабатываемую реакцию.                                         *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractAgent::addSprout ( tengu::Sprout * sprout ) {
    __sprouts[ sprout->getUUID() ] = sprout ;
    __subscribe();
    _changed = true;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                       Total sprout's count for this agent.                                       *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                      Общее число "ростков" данного агента.                                       *
// *                                                                                                                  *
// ********************************************************************************************************************

int tengu::AbstractAgent::sproutsCount() {
    return __sprouts.count();
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                Set activity channel                                              *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                             Установка канала активности.                                         *
// *                                                                                                                  *
// ********************************************************************************************************************
/*
void tengu::AbstractAgent::setActivityChannel ( QString activityChannel ) {
    
    if ( ! activityChannel.isEmpty() ) {
        _activityChannel = activityChannel;
        Sprout * aSprout = new Sprout( this, "Activity" );
        aSprout->setInputChannel( activityChannel );
        QObject::connect( aSprout, SIGNAL( signalGotValue( QVariant ) ), this, SLOT( __on_activity_channel_message( QVariant ) ) );
        addSprout( aSprout );
    };
    
}
*/

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                    The destructor.                                               *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                       Деструктор.                                                *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::AbstractAgent::~AbstractAgent() {
    
}


