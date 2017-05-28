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
    : AbstractAgentKernel ( name, parent ) 
{
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
    
    for ( int i=0; i<__sprouts.length(); i++ ) {
        __sprouts.at(i)->subscribed( channel );
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
    
    for ( int i=0; i<__sprouts.length(); i++ ) {
        __sprouts.at(i)->unsubscribed( channel );
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
    for ( int i=0; i<__sprouts.length(); i++ ) {
    
        bool res = __sprouts.at(i)->handleMessage( channel, message );
        if ( res ) handled = true;
    };
    
    if ( ! handled ) {
        qDebug() << "AbstractAgent::__on_got_message(" << channel << ") was not handled.";
    }
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                           Subscribe all existing sprouts.                                        *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                            Подписать всех имеющихся реакторов ("веточки" данного агента).                        *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractAgent::__subscribe() {
    for ( int i=0; i<__sprouts.length(); i++ ) {
        __sprouts.at(i)->subscribe();
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
    __sprouts.append( sprout );
    __subscribe();
}


// ********************************************************************************************************************
// *                                                                                                                  *
// *                             Add reaction for specified channel if it is not empty                                *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                         Добавление реакции для определенного канала, если он не пустой.                          *
// *                                                                                                                  *
// ********************************************************************************************************************

/*
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
*/

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                      Remove the reaction from handled list.                                      *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                    Удалить реакцию из списка обрабатываемых.                                     *
// *                                                                                                                  *
// ********************************************************************************************************************
/*
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
*/

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
    /*
    try {
        
        while ( __reactions.length() > 0 ) {
            reaction_t * r = __reactions.at(0);
            __reactions.removeAt( 0 );
            delete( r );
        };
        
    } catch ( ... ) {
        qDebug() << "AbstractAgent::AbstractAgent(), clean reaction error.";
    };
    */
}


