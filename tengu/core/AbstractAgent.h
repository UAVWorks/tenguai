// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                 The Abstract Agent.                                              *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                  Абстрактный агент.                                              *
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

    class AbstractAgent : public QObject, public AbstractEntity {
    
        friend class Sprout;
                                
        Q_OBJECT
        
        Q_PROPERTY( QString system_name READ getSystemName WRITE setSystemName );
        Q_PROPERTY( QString human_name READ getHumanName WRITE setHumanName );
        Q_PROPERTY( QString comment READ getComment WRITE setComment );
        Q_PROPERTY( QString uuid READ getUUID );
        Q_PROPERTY( AbstractEntity::execution_mode_t execution_mode READ getExecutionMode WRITE setExecutionMode );
        
        signals:
            
            void signalSomethingChanged();
        
        public:
                        
            AbstractAgent();
            virtual ~AbstractAgent();
            
            /**
             * @short Public connect procedure.
             * 
             * It should be called after full agent initialization.
             * 
             * Публичная процедура соединения с редисом. Ее нужно вызывать после полной 
             * инициализации агента.
             */
            
            virtual void connect();
            
            virtual void disconnect();
            
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
            
            virtual void addChild( AbstractAgent * child );
            bool hasChildren();
            virtual void removeChild( AbstractAgent * child );
            QList <AbstractAgent * > children();
            AbstractAgent * parent();
            void deleteChildren();
            
            virtual QJsonObject toJSON();
            virtual void unmodify();
            
            /**
             * @short Adding or replacing previous (in the sence of focus flow) agent.
             */
            
            void addPreviousByFocus( AbstractAgent * previous );            
            void addNextByFocus( AbstractAgent * next );                       
            void removeNeighborByFocus( AbstractAgent * agent );
            // void removeNeighborByFocus( QString uuid );
                                    
            // The agent can be provided either as object in memory
            // or separate process in the operation system for 
            // resource savings.
            
            // Агент может быть обеспечен как порождением его класса,
            // так и отдельным процессом в операционной системе для
            // экономии ресурсов.
            
            // virtual QString subProcessPath();        
            
            /**
             * @short Get all entities of specified type, whose name or comment contains the mask.
             * Получить все "сущности" указанного типа, имеющие маску в своем имени или комментарии.
             */
            
            template <class T> QList< T > getEntities ( QString mask ) {

                QList < T > result;
    
                foreach( AbstractAgent * oneChild, _children ) {
        
                    T entity = __satisfiedTheMask< T >( oneChild, mask );
                    if ( entity ) result.append( entity );
        
                    // Re-enterable call procedure for children of this agent.
                    // Реентерабельный вызов процедуры для детей этого агента.
        
                    QList < AbstractAgent *>  reChildrenList = oneChild->children();
                    if ( reChildrenList.count() > 0 ) {
                        for ( int cIndex=0; cIndex < reChildrenList.count(); cIndex ++ ) {
                            AbstractAgent * reChild = reChildrenList.at( cIndex );
                            entity = __satisfiedTheMask< T >( reChild, mask );
                            if ( entity ) result.append( entity );
                        };
                    };
                
                };
        
                return( result );
            };                        
            
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
            
            AbstractAgent * _parent;
            QMap< QString, AbstractAgent * > _children;
            
            // Line-like (flat, 2d, by time flow) structure of agents. Key for map is an UUID.            
            // Линейная (плоская, 2d, по времени) структура агентов. Ключ для словарика - это UUID.
            
            QMap< QString, AbstractAgent * > _previousByFocus;
            QMap< QString, AbstractAgent * > _nextByFocus;
                        
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
            
            bool __must_be_connected;
            
            /**
             * @short Check type of this entity and it contains the mask in it's names.
             * Проверить тип сущности, а так же что она содержит маску в именах.
             */
                        
            template <class T > T __satisfiedTheMask ( tengu::AbstractAgent * child, QString mask ) {

                QString sName = child->getSystemName();
                QString hName = child->getHumanName();
                QString comment = child->getComment();
                
                if ( mask.isEmpty() || ( sName.contains( mask ) ) || ( hName.contains( mask ) ) || ( comment.contains(mask) ) ) {
        
                    T och = dynamic_cast< T >( child );
                    if ( och ) return och;
        
                };  
    
                return nullptr;
            };
            
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

