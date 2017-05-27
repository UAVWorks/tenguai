// ********************************************************************************************************************
// *                                                                                                                  *
// *      The abstract agent. Any logical completed piece which can do something. Usually it is a separate process    *
// *                                              inside operation system.                                            *
// * ---------------------------------------------------------------------------------------------------------------- *
// *    Абстрактный агент. Любой логически законченный кусок, который может что-то делать. Как правило - отдельный    *
// *                                             процесс операционной системы.                                        *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 26 may 2017 at 11:57 *
// ********************************************************************************************************************

#pragma once

#include <QObject>
#include <QString>
#include <QTimer>
#include <QTime>
#include <QDateTime>
#include <QMutex>
#include <QDebug>

#include "AbstractAgentKernel.h"
#include "Sprout.h"

namespace tengu {

    class AbstractAgent : public AbstractAgentKernel {
        
        Q_OBJECT
        
        public:

            // Callback procedure for message reaction. The parameters is a channel and a message in them.
            // Callback-процедура реакции на сообщение. Параметры - канал и сообщение в нем.
            
            // typedef void(* reaction_callback_t)( QString, QString );
            // 
            // struct reaction_t {
            //     
            //    // Channel is full redis channel, with prefix. As it must be in redis without any additions.
            //    // Канал - это полное имя канала в редисе, вместе с префиксом. Как оно должно быть в редисе, без каких-либо добавок.
            //    
            //    QString channel;
            //    bool subscribtion_applicated;
            //    bool subscribed;
            //    reaction_callback_t reaction;
            //    // void ( * reaction )( QString message );
            // };
            
            // The name must be unique around the system. The name is the same as "section" 
            // in config file.
            // ------------------------------------------------------------------------------------
            // Имя должно быть уникальным в пределах системы. Одновременно "имя" - это "секция" 
            // в файле конфигурации.
            
            AbstractAgent ( QString name, QObject * parent = 0 );
            virtual ~AbstractAgent();
            
            // Add "the sprout", reaction to redises messages.
            // Добавить "веточку" реакции на сообщения редиса.
            
            void addSprout( Sprout * sprout );
            
            // Add reaction for the specified channel, if it is not empty.
            // Добавить реакцию для указанного канала, если он не пустой.
            
            // void addReactionFor( QString channel, reaction_callback_t reaction );
            
            // Remove reaction from handled list
            // Удалить реакцию из списка обрабатываемых.
            
            // void removeReaction( reaction_t reaction );
            
            
        protected:
            
            
        private:
            
            QList <Sprout * > __sprouts;
            
            // The subscribtion function. Call at the moment subsciber redis connection.
            // Can be overrided from descendants.
            
            // Функция подписки. Вызывается на момент соединения подписчика редиса.
            // Может быть перекрыта из потомков.
            
            void __subscribe();
            
        private slots:
            
            void __on_subscriber_connected();
            void __on_got_message( QString channel, QString messaage );
            void __on_subscribed( QString channel );
            void __on_unsubscribed( QString channel );
            
    };
    
};

