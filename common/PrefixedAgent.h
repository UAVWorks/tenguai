// ********************************************************************************************************************
// *                                                                                                                  *
// *    "Prefixed agent" is "agent with the prefix". Prefix plus agent name = path in redis subscribed channels to    *
// *                                       most common messages to / from this agent.                                 *
// * ---------------------------------------------------------------------------------------------------------------- *
// * PrefixedAgent - это "агент с префиксом". Префикс плюс имя агента дают путь в редисе для подписываемых каналов,   *
// *                                  в которых ходят общие сообщения к / от этого агента.                            *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 26 may 2017 at 15:53 *
// ********************************************************************************************************************

#pragma once

#include <QObject>

#include "AbstractAgent.h"

namespace tengu {
    
    class PrefixedAgent : public AbstractAgent {
        
        Q_OBJECT
        
        public:
            
            PrefixedAgent( QString prefix, QString name, QObject * parent = 0 );
            virtual ~PrefixedAgent();
            
        protected:
            
            QString _prefix;
            
        private:
            
            void __on_activity_received( QString channel, QString message );
    };
    
};
