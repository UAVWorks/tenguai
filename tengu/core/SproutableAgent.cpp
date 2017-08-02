// ********************************************************************************************************************
// *                                                                                                                  *
// *                                         The agent which can have a sprouts                                       *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                        Агент, который может иметь "отростки".                                    *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 26 may 2017 at 11:59 *
// ********************************************************************************************************************

#include "SproutableAgent.h"

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                    The constructor.                                              *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                      Конструктор.                                                *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::SproutableAgent::SproutableAgent () 
    : AbstractAgent () 
{    
    _oActivityChannel = QString("");
    _iFocusChannel = QString("");
    // _oFocusChannel = QString("");
        
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

void tengu::SproutableAgent::__on_subscriber_connected() {
    qDebug() << "SproutableAgent::on subscriber connected";
    __subscribe();
};

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                      We has been subscribed to someone channel.                                  *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                          Мы подписались на какой-то канал.                                       *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::SproutableAgent::__on_subscribed ( QString channel ) {
    
    qDebug() << "SproutableAgent::subscribe: " << channel;
    foreach ( tengu::Sprout * sprout, __sprouts ) {
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

void tengu::SproutableAgent::__on_unsubscribed ( QString channel ) {
    
    qDebug() << "SproutableAgent::on unsubscribed: " << channel;
    
    foreach ( tengu::Sprout * sprout, __sprouts ) {
        sprout->unsubscribed( channel );
    };
    qDebug() << "SproutableAgent::on unsubscribed done.";
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                      One sprout has been physically deleted.                                     *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                            Sprout был физически удален.                                          *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::SproutableAgent::__on__sprout_removed ( tengu::Sprout * sprout ) {
    
    removeSprout( sprout );
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                     We got some message from subscriber redis.                                   *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                    Мы получили сообщение от редиса по подписке.                                  *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::SproutableAgent::__on_got_message ( QString channel, QString message ) {

    bool handled = false;
        
    foreach ( tengu::Sprout * sprout, __sprouts ) {
        bool res = sprout->handleMessage( channel, message );
        if ( res ) {
            QString sproutName = sprout->getSystemName();
            // Call sprout's got value callback.
            // Вызов callback'а получения значения спраутом.
            _sprout_got_value( sproutName, sprout->getValue() );
            // emit signalSproutGotValue( sproutName, sprout->getValue() );
            handled = true;
        };
    }    
    
    if ( ! handled ) {
        qDebug() << "SproutableAgent::__on_got_message(" << channel << ") was not handled.";
    }
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                          Got some value from some sprout.                                        *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                 Получено некоторое значение от некоторого спраута.                               *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::SproutableAgent::_sprout_got_value ( QString sproutSysName, QVariant value ) {
    
    // By default, receiving value from sprout invoke one step of algorythm.
    // По умолчанию, получение значения от спраута вызывает один шаг алгоритма.
    
    _step();
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                       This function performs to make decision about activation of this channel                   *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                        Функция выполняется для принятия решения об активации данного канала.                     *
// *                                                                                                                  *
// ********************************************************************************************************************
/*
void tengu::SproutableAgent::__tryActivate() {
}
*/
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

void tengu::SproutableAgent::disconnect() {
    __unsubscribe();
    tengu::AbstractAgent::disconnect();
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                  Conversion from JSON form to values of this class.                              *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                  Преобразование из JSONа в значения данного класса.                              *
// *                                                                                                                  *
// ********************************************************************************************************************

bool tengu::SproutableAgent::fromJSON ( QJsonObject json ) {
    
    bool result = tengu::AbstractAgent::fromJSON ( json );
        
    /*
    if ( json.contains("sprouts") ) {
        
        __deleteSprouts();
        
        QJsonArray spa = json.value("sprouts").toArray();
                
        for( int i=0; i<spa.count(); i++ ) {
            QJsonObject os = spa.at(i).toObject();
            Sprout * sprout = new Sprout();
            sprout->fromJSON( os );
            addSprout( sprout );
        };
    };
    */
        
    return result;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                              Return a list of sprouts.                                           *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                              Вернуть список sprout'ов.                                           *
// *                                                                                                                  *
// ********************************************************************************************************************

QList< tengu::Sprout* > tengu::SproutableAgent::sprouts() {
    QList<Sprout * > result;
    foreach( Sprout * sprout, __sprouts ) {
        result.append( sprout );
    };
    return result;
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
/*
bool tengu::SproutableAgent::isActive() {
    return __activity;
}
*/
// ********************************************************************************************************************
// *                                                                                                                  *
// *                                               Is this agent focused?                                             *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                             В фокусе ли данный агент?                                            *
// *                                                                                                                  *
// ********************************************************************************************************************
/*
bool tengu::SproutableAgent::isFocused() {
    return __focus;
}
*/
// ********************************************************************************************************************
// *                                                                                                                  *
// *                                           Subscribe all existing sprouts.                                        *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                            Подписать всех имеющихся реакторов ("веточки" данного агента).                        *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::SproutableAgent::__subscribe() {
    
    foreach ( tengu::Sprout * sprout, __sprouts) {
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

void tengu::SproutableAgent::__unsubscribe() {
    foreach( tengu::Sprout * sprout, __sprouts ) {
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

void tengu::SproutableAgent::addSprout ( tengu::Sprout * sprout ) {
    
    sprout->__owner = this;
    __sprouts[ sprout->getUUID() ] = sprout ;    
    QObject::connect( sprout, SIGNAL(signalRemoveMe(Sprout*)), this, SLOT( __on__sprout_removed( Sprout * ) ) );
    
    // Sprout's auto naming 
    // Авто-именование sprout'а.
    
    if ( sprout->getSystemName().isEmpty() ) {
        QString sysName = tr("Sprout_") + QString::number( __sprouts.count() );
        sprout->setSystemName( sysName );
    };
    
    if ( sprout->getHumanName().isEmpty() ) {
        QString humName = tr("Sprout ") + QString::number( __sprouts.count() );
        sprout->setHumanName( humName );
    };    
    
    // if ( ( sprout->isExternal() ) && ( ( ! isSubscriberConnected() ) || ( ! isPublisherConnected() ) ) ) {
    //    connect();
    //};
    
    // Подписку нужно делать отдельно и принудительно.
    // __subscribe();
    
    _somethingChanged();
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                             Remove one sprout                                                    *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                          Удаление одного sprout'а                                                *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::SproutableAgent::removeSprout ( tengu::Sprout * sprout ) {
    
    if ( __sprouts.contains( sprout->getUUID() ) ) {
        __sprouts.remove( sprout->getUUID() );
        QObject::disconnect( sprout, SIGNAL(signalRemoveMe(Sprout*)), this, SLOT( __on__sprout_removed( Sprout * ) ) );
    };
    _somethingChanged();
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                        Delete all sprouts of this agent.                                         *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                     Удаление всех sprout'ов данного агента.                                      *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::SproutableAgent::__deleteSprouts() {
    
    if ( ! __sprouts.isEmpty() ) {
        
        foreach( Sprout * sprout, __sprouts ) {
            QObject::disconnect( sprout, SIGNAL(signalRemoveMe(Sprout*)), this, SLOT( __on__sprout_removed( Sprout * ) ) );
            delete( sprout );
        }
    
        __sprouts.clear();
    };
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                       Total sprout's count for this agent.                                       *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                      Общее число "ростков" данного агента.                                       *
// *                                                                                                                  *
// ********************************************************************************************************************

int tengu::SproutableAgent::sproutsCount() {
    return __sprouts.count();
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                         Get list of sprouts suitable (connectable) to specified sprout.                          *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                          Получить массив sprout'ов, соединябельных с представленным.                             *
// *                                                                                                                  *
// ********************************************************************************************************************

QList< tengu::Sprout* > tengu::SproutableAgent::sutiableSproutsFor ( tengu::Sprout * sp ) {
    
    QList < tengu::Sprout * > result;
    foreach ( tengu::Sprout * sprout, __sprouts ) {
        if ( (sp->getExecutionMode() == Sprout::EM_ALWAYS ) 
            || (sprout->getExecutionMode() == Sprout::EM_ALWAYS) 
            || ( sprout->getExecutionMode() == sp->getExecutionMode() ) ) 
        {
            
            bool usable = false;
            Sprout::sprout_type_t eType = sprout->getSproutType();
            switch ( sp->getSproutType() ) {
                case Sprout::SPT__EXTERNAL_INPUT: {
                    if ( eType == Sprout::SPT__EXTERNAL_OUTPUT ) usable = true;
                }; break;
                case Sprout::SPT__EXTERNAL_OUTPUT: {
                    if ( eType == Sprout::SPT__EXTERNAL_INPUT ) usable = true;
                }; break;
                case Sprout::SPT__IN_PROCESS_INPUT: {
                    if ( eType == Sprout::SPT__IN_PROCESS_OUTPUT ) usable = true;
                }; break;
                case Sprout::SPT__IN_PROCESS_OUTPUT: {
                    if ( eType == Sprout::SPT__IN_PROCESS_INPUT ) usable = true;
                }; break;
            };
            
            if ( usable ) result.append( sprout );
        };
    };
    return result;
    
}

// ********************************************************************************************************************
// * *
// * Conversion of this object to JSON form. *
// * ---------------------------------------------------------------------------------------------------------------- *
// * Преобразование данного объекта в JSON. *
// * *
// ********************************************************************************************************************

QJsonObject tengu::SproutableAgent::toJSON() {
    QJsonObject o = tengu::AbstractAgent::toJSON();
    if ( ! __sprouts.isEmpty() ) {
        QJsonArray spa;
        foreach ( Sprout * sprout, __sprouts ) {
            QJsonObject spo = sprout->toJSON();
            spa.append( spo );
        };
        o["sprouts"] = spa;
    };
    return o;
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

tengu::SproutableAgent::~SproutableAgent() {
    
}


