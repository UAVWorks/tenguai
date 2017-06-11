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
#include <QUuid>

#include "LoRedis.h"

namespace tengu {

    class AbstractAgentKernel : public QObject {
    
        friend class Sprout;
        
        Q_OBJECT
        
        Q_PROPERTY( QString name READ name WRITE setName )
        Q_PROPERTY( QString uuid READ uuid WRITE setUUID )
        
        public:
            
            AbstractAgentKernel();
            virtual ~AbstractAgentKernel();
            
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
            
            void addChild( AbstractAgentKernel * child );
            
            // true, really unique identifier for this object
            // Действительно уникальный идентификатор для данного объекта.
            
            QString uuid();
            void setUUID( QString uuid );            
            
            // The name of this agent.
            // Имя агента.
            
            QString name();
            void setName( QString name );
            
            
            QString comment();
            void setComment( QString comment );
            
            // The agent can be provided either as object in memory
            // or separate process in the operation system for 
            // resource savings.
            
            // Агент может быть обеспечен как порождением его класса,
            // так и отдельным процессом в операционной системе для
            // экономии ресурсов.
            
            virtual QString subProcessPath();
            
            
        protected:
            
            // Agent name
            // Имя агента
            
            QString _name;
            
            // Any human readable comment for this agent.
            // Любой человеко-ориентированный комментарий для агента.
            
            QString _comment;
            
            QString _subProcessPath;
                        
            // Redis publicator and writer
            // Редис - который публикует и записывает.
            
            LoRedis * _pub_redis;
            
            // Redis subscriber
            // Редис - который подписчик.
            
            LoRedis * _sub_redis;
            
            // Procedure to birth the redises. Can be overrided from descendants to change LoRedis class.
            // Функция для порождения редисов, может быть перекрыта из потомков с целью изменить класс LoRedis.
            
            virtual void _createRedises();
            
            // There was got some variable from redis.
            // Было получено некоторое значение переменной из редиса.
            
            // virtual void _got_value( QString sproutName );
            
            // Tree-like ("the space" or "the volume", 3d) structure of agents.
            // Деревообразная ("объемная", 3d) структура агентов.
            
            AbstractAgentKernel * _parent;
            QList<AbstractAgentKernel * > _children;
            
            // Line-like (flat, 2d, by time flow) structure of agents.
            // Линейная (плоская, 2d, по времени) структура агентов.
            
            QList<AbstractAgentKernel * > _prefiousAgents;
            QList<AbstractAgentKernel * > _nextAgents;
            
            // Unique (in entire system range) identifier of this agent.
            // Уникальный (в пределах вообще всей системы) идентификатор данного агента.
            
            QString _uuid;
            
            // Tree-like structure of agents. For loading entire "tree branch" completely.
            // Древовидная структура агентов. Для загрузки всей "ветки" дерева целиком.
            
            QString _parent_uuid;
            
        private:
            
            bool __pub_redis_connected;
            bool __sub_redis_connected;                        
            
            QTimer * __ping_timer;
            QTimer * __connect_timer;
            
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

