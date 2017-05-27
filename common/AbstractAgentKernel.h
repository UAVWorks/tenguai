// ********************************************************************************************************************
// *                                                                                                                  *
// *    The "kernel" of abstract agent. Class has been introduced to automatically work of properties (sprouts).      *
// * ---------------------------------------------------------------------------------------------------------------- *
// *  "Ядро" абстрактного агента. Класс был введен для автоматической работы "свойств" (они же "отростки" - Sprout).  *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 27 may 2017 at 08:28 *
// ********************************************************************************************************************

#pragma once

#include <QObject>
#include <QString>
#include <QMutex>
#include <QTimer>
#include <QTime>
#include <QDateTime>

#include "LoRedis.h"

namespace tengu {

    class AbstractAgentKernel : public QObject {
    
        friend class Sprout;
        
        Q_OBJECT
        
        public:
            
            AbstractAgentKernel( QString name, QObject * parent = 0 );
            virtual ~AbstractAgentKernel();
            
            bool isActive();
            
            /**
             * @short Public connect procedure.
             * 
             * It should be called after full agent initialization.
             * 
             * Публичная процедура соединения с редисом. Ее нужно вызывать после полной 
             * инициализации агента.
             */
            
            void connect();
            
            // Is the publisher connected to redis?
            // Соединен ли с редисом публикатор?
            
            bool isPublisherConnected();
            
            // Is the subscriber connected to redis?
            // Соединен ли с редисом подписчик?
            
            bool isSubscriberConnected();
            
            // "Usable" mean "ready to work. This function should be overrided because abstract 
            // agent is not ready to work.
            // ------------------------------------------------------------------------------------
            // "Usable" обозначает "готовный к работе". Фунцию нужно перекрывать, т.к. абстрактный 
            // агент к работе не готов.
            
            virtual bool usable();
            
        protected:
            
            // Agent name (should be unique around the system)
            // Имя агента (хорошо бы уникальное в пределах системы).
            
            QString _name;   
            
            // Redis publicator and writer
            // Редис - который публикует и записывает.
            
            LoRedis * _pub_redis;
            
            // Redis subscriber
            // Редис - который подписчик.
            
            LoRedis * _sub_redis;
            
            // Procedure to birth the redises. Can be overrided from descendants to change LoRedis class.
            // Функция для порождения редисов, может быть перекрыта из потомков с целью изменить класс LoRedis.
            
            virtual void _createRedises();
            
            virtual void _setActivity( bool a );
            
            // There was got some variable from redis.
            // Было получено некоторое значение переменной из редиса.
            
            virtual void _got_value( QString sproutName );
            
        private:
            
            bool __pub_redis_connected;
            bool __sub_redis_connected;                        
            
            QTimer * __ping_timer;
            QTimer * __connect_timer;
            
            // List of reaction on redises messages
            // Список реакций на сообщения от редиса.
            
            // QList< reaction_t * > __reactions;
            
            // Mutex for change reactions properties.
            // Мутекс для изменения свойств реакций.
            
            // QMutex __reMutex;
            
            // Activity of this agent.
            // Активность данного агента.
            
            bool __activity;
            
        private slots:
            
            void __on_ping_timer();
            void __on_connect_timer();
            
            void __on_pub_redis_connected();
            void __on_sub_redis_connected();
            void __on_pub_redis_disconnected();
            void __on_sub_redis_disconnected();
            void __on_redis_error ( QString message );            
            
    };
    
};

