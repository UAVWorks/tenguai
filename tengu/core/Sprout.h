// ********************************************************************************************************************
// *                                                                                                                  *
// *           The sprout from / to agent, the communication branch between agents using redis messages.              *
// *                                  In other hand this is a property of the agent.                                  *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                 Отросток из / в агента. Веточка для связи между агентами через сообщения редиса.                 *
// *                                  С другой стороны, он же и "свойство" агента.                                    *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 27 may 2017 at 08:02 *
// ********************************************************************************************************************

#pragma once

// #ifndef TENGU_CORE_SPROUT_H
// #define TENGU_CORE_SPROUT_H

#include <QObject>
#include <QVariant>

#include "AbstractAgent.h"
#include "Constants.h"

namespace tengu {
    
    class Sprout : public QObject, public AbstractEntity {
        
        friend class SproutableAgent;
        
        Q_OBJECT

        Q_PROPERTY( QString system_name READ getSystemName WRITE setSystemName );
        Q_PROPERTY( QString human_name READ getHumanName WRITE setHumanName );
        Q_PROPERTY( QString comment READ getComment WRITE setComment );
        Q_PROPERTY( QString uuid READ getUUID );
        Q_PROPERTY( execution_mode_t execution_mode READ getExecutionMode WRITE setExecutionMode );
        Q_PROPERTY( sprout_type_t sprout_type READ getSproutType WRITE setSproutType );
        Q_PROPERTY( QString signal_name READ getSignalName WRITE setSignalName );
        Q_PROPERTY( float minimal_value READ getMinimalValue WRITE setMinimalValue );
        Q_PROPERTY( float maximal_value READ getMaximalValue WRITE setMaximalValue );
        
        Q_PROPERTY( QVariant value READ getValue WRITE setValue );
        
        signals:
            
            void signalGotValue( QVariant value );
            void signalRemoveMe( Sprout * sprout );
                    
        public:
            
            enum sprout_type_t {
                // Signal only, without redis.
                // Только через сигналы, без редиса.
                SPT__IN_PROCESS_INPUT,
                // External mean throught redis
                // Внешний - означает через редис.
                SPT__EXTERNAL_INPUT,
                SPT__IN_PROCESS_OUTPUT,
                SPT__EXTERNAL_OUTPUT
            };
            
            // The name should be unique in the parent (owner) namespace.
            // Имя лучше бы было уникальным в пределах родителя (владельца).
            
            Sprout( AbstractAgent * owner = nullptr );
            virtual ~Sprout();
            
            // void setInputChannel( QString channel );
            // void setOutputChannel( QString channel );
            
            QString getSignalName();
            void setSignalName( QString signal );
            
            void setSproutType( sprout_type_t type );
            sprout_type_t getSproutType();
            bool isInput();
            bool isOutput();
            bool isExternal();
            bool isInternal();
            
            virtual void subscribe();
            virtual void unsubscribe();
            virtual void subscribed( QString channel );
            virtual void unsubscribed( QString channel );
            virtual bool handleMessage( QString channel, QString message );     
            
            QVariant getValue();
            void setValue( QVariant value );
            
            
            float getMinimalValue();
            void setMinimalValue( float min );
            float getMaximalValue();
            void setMaximalValue( float max );                        
            
            // Get sprout's owner ( the agent, usual the task )
            // Получить владельца sprout'а ( агента, обычно - задачу ).
            
            AbstractAgent * owner();
            
            virtual bool fromJSON( QJsonObject o ) override;
            
            // Convert the object to JSON form.
            // Преобразование объекта в JSON-форму.
            
            virtual QJsonObject toJSON() override;
            
            
        protected:
            
            bool _to_me( QString channel );
            
        private:
            
            
            AbstractAgent * __owner;
            
            QVariant __value;
            
            volatile bool __subscribed;
            bool __subscribtion_requested;
            
            
            // QString __inputChannel;
            // QString __outputChannel;
            
            QString __signalName;
                        
            sprout_type_t __sprout_type;                            
            
            float __minimal_value;
            float __maximal_value;
            
    };
    
};

// #endif
