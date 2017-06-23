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

namespace tengu {
    
    class Sprout : public QObject, public AbstractEntity {
        
        Q_OBJECT        

        Q_PROPERTY( QString system_name READ getSystemName WRITE setSystemName );
        Q_PROPERTY( QString human_name READ getHumanName WRITE setHumanName );
        Q_PROPERTY( QString comment READ getComment WRITE setComment );
        Q_PROPERTY( QString uuid READ getUUID );
        Q_PROPERTY( execution_mode_t execution_mode READ getExecutionMode WRITE setExecutionMode );
        Q_PROPERTY( sprout_type_t sprout_type READ getSproutType WRITE setSproutType );
        
        Q_PROPERTY( QVariant value READ __getValue WRITE __setValue );
                    
        public:
            
            enum sprout_type_t {
                SP_INPUT,
                SP_OUTPUT
            };
            
            // The name should be unique in the parent (owner) namespace.
            // Имя лучше бы было уникальным в пределах родителя (владельца).
            
            Sprout( AbstractAgentKernel * owner = Q_NULLPTR );
            virtual ~Sprout();
            
            void setInputChannel( QString channel );
            void setOutputChannel( QString channel );
            
            void setSproutType( sprout_type_t type );
            sprout_type_t getSproutType();
            
            void subscribe();
            void subscribed( QString channel );
            void unsubscribed( QString channel );
            virtual bool handleMessage( QString channel, QString message );                                                
            
        protected:
            
            bool _to_me( QString channel );
            
        private:
            
            
            QVariant __value;
            
            bool __subscribed;
            bool __subscribtion_requested;
            
            AbstractAgentKernel * __owner;
            
            QString __inputChannel;
            QString __outputChannel;
            
            QVariant __getValue();
            void __setValue( QVariant value );
            
            sprout_type_t __sprout_type;
            
        signals:
            
            void signalGotValue( QVariant value );
        
            
    };
    
};

