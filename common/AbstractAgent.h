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

#include "LoRedis.h"

namespace tengu {

    class AbstractAgent : public QObject {
        
        Q_OBJECT
        
        public:
            
            // The name must be unique around the system. The name is the same as "section" in config file.
            //
            // Имя должно быть уникальным в пределах системы. Одновременно "имя" - это "секция" в файле
            // конфигурации.
            
            AbstractAgent ( QString name, QObject * parent = 0 );
            virtual ~AbstractAgent();
            
        protected:
            
            // Agent name (should be unique around the system)
            // Имя агента (хорошо бы уникальное в пределах системы).
            
            QString _name;                                    
            
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
            
            
        private slots:
            
            void __on_pub_redis_connected();
            void __on_sub_redis_connected();
            void __on_pub_redis_disconnected();
            void __on_sub_redis_disconnected();
            void __on_redis_error ( QString message );            
            void __on_got_message( QString channel, QString messaage );
            
            void __on_ping_timer();
            void __on_connect_timer();
    };
    
};

