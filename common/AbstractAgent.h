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

#include "LoRedis.h"

namespace tengu {

    class AbstractAgent : public QObject {
        
        Q_OBJECT
        
        public:
            
            struct reaction_t {
                
                // Channel is full redis channel, with prefix. As it must be in redis without any additions.
                // Канал - это полное имя канала в редисе, вместе с префиксом. Как оно должно быть в редисе, без каких-либо добавок.
                
                QString channel;
                bool subscribtion_applicated;
                bool subscribed;
                void ( * reaction )( QString message );
            };
            
            // The name must be unique around the system. The name is the same as "section" in config file.
            //
            // Имя должно быть уникальным в пределах системы. Одновременно "имя" - это "секция" в файле
            // конфигурации.
            
            AbstractAgent ( QString name, QObject * parent = 0 );
            virtual ~AbstractAgent();
            
            void addReaction( reaction_t reaction );
            void removeReaction( reaction_t reaction );
            
        protected:
            
            // Agent name (should be unique around the system)
            // Имя агента (хорошо бы уникальное в пределах системы).
            
            QString _name;   
            
            // The subscribtion function. Call at the moment subsciber redis connection.
            // Can be overrided from descendants.
            
            // Функция подписки. Вызывается на момент соединения подписчика редиса.
            // Может быть перекрыта из потомков.
            
            virtual void _subscribe();
            
        private:
            
            // Redis publicator and writer
            // Редис - который публикует и записывает.
            
            LoRedis * __pub_redis;
            
            // Redis subscriber
            // Редис - который подписчик.
            
            LoRedis * __sub_redis;
            
            bool __pub_redis_connected;
            bool __sub_redis_connected;                        
            
            QTimer * __ping_timer;
            QTimer * __connect_timer;
                        
            // List of reaction on redises messages
            // Список реакций на сообщения от редиса.
            
            QList< reaction_t * > __reactions;
            
            // Mutex for change reactions properties.
            // Мутекс для изменения свойств реакций.
            
            QMutex __reMutex;
            
            
        private slots:
            
            void __on_pub_redis_connected();
            void __on_sub_redis_connected();
            void __on_pub_redis_disconnected();
            void __on_sub_redis_disconnected();
            void __on_redis_error ( QString message );            
            void __on_got_message( QString channel, QString messaage );
            void __on_subscribed( QString channel );
            void __on_unsubscribed( QString channel );
            
            void __on_ping_timer();
            void __on_connect_timer();
    };
    
};

