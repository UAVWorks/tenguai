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

#include <QObject>
#include <QVariant>
#include "AbstractAgentKernel.h"
#include "AbstractEntity.h"
#include "Constants.h"

namespace tengu {
    
    class Sprout : public QObject, public AbstractEntity {
        
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
                    
        public:
            
            enum sprout_type_t {
                // Signal only, without redis.
                // Только через сигналы, без редиса.
                IN_PROCESS_INPUT,
                // External mean throught redis
                // Внешний - означает через редис.
                EXTERNAL_INPUT,
                IN_PROCESS_OUTPUT,
                EXTERNAL_OUTPUT
            };
            
            // The name should be unique in the parent (owner) namespace.
            // Имя лучше бы было уникальным в пределах родителя (владельца).
            
            Sprout( AbstractAgentKernel * owner = Q_NULLPTR );
            virtual ~Sprout();
            
            // void setInputChannel( QString channel );
            // void setOutputChannel( QString channel );
            
            QString getSignalName();
            void setSignalName( QString signal );
            
            void setSproutType( sprout_type_t type );
            sprout_type_t getSproutType();
            bool isInput();
            bool isOutput();
            
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
            
        protected:
            
            bool _to_me( QString channel );
            
        private:
            
            
            QVariant __value;
            
            bool __subscribed;
            bool __subscribtion_requested;
            
            AbstractAgentKernel * __owner;
            
            // QString __inputChannel;
            // QString __outputChannel;
            
            QString __signalName;
                        
            sprout_type_t __sprout_type;                            
            
            float __minimal_value;
            float __maximal_value;
            
    };
    
};

