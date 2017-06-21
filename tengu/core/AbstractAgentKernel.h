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
#include <QMap>

#include "LoRedis.h"
#include "AbstractEntity.h"

namespace tengu {

    class AbstractAgentKernel : public QObject, public AbstractEntity {
    
        friend class Sprout;
                                
        Q_OBJECT
        
        signals:
            
            void signalSomethingChanged();
        
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
            bool hasChildren();
            
            /**
             * @short Adding or replacing previous (in the sence of focus flow) agent.
             */
            
            void addPreviousByFocus( AbstractAgentKernel * previous );            
            void addNextByFocus( AbstractAgentKernel * next );                       
            void removeNeighborByFocus( AbstractAgentKernel * agent );
                        
            // The agent can be provided either as object in memory
            // or separate process in the operation system for 
            // resource savings.
            
            // Агент может быть обеспечен как порождением его класса,
            // так и отдельным процессом в операционной системе для
            // экономии ресурсов.
            
            // virtual QString subProcessPath();                        
            
        protected:
                        
            // QString _subProcessPath;
                        
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
            QMap< QString, AbstractAgentKernel * > _children;
            
            // Line-like (flat, 2d, by time flow) structure of agents. Key for map is an UUID.            
            // Линейная (плоская, 2d, по времени) структура агентов. Ключ для словарика - это UUID.
            
            QMap< QString, AbstractAgentKernel * > _previousByFocus;
            QMap< QString, AbstractAgentKernel * > _nextByFocus;
                        
            // Tree-like structure of agents. For loading entire "tree branch" completely.
            // Древовидная структура агентов. Для загрузки всей "ветки" дерева целиком.
            
            QString _parent_uuid;               
            
            // Overrided change's method for emit signal
            // Перекрытый метод изменения, чтобы вызвать сигнал
            
            void _somethingChanged();
            
        private:
            
            bool __pub_redis_connected;
            bool __sub_redis_connected;                        
            
            QTimer * __connect_timer;
            QTimer * __ping_timer;
            
        private slots:
            
            void __on_connect_timer();
            void __on_ping_timer();
            
            void __on_pub_redis_connected();
            void __on_sub_redis_connected();
            void __on_pub_redis_disconnected();
            void __on_sub_redis_disconnected();
            void __on_redis_error ( QString message );              
            
    };
    
};

