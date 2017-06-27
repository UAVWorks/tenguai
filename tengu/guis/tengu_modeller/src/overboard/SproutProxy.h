// ********************************************************************************************************************
// *                                                                                                                  *
// *                      Sprout proxy (the sprout without real connection, only for signal names)                    *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                   Заместитель Sprout'а (Sprout без реального соединения, только для имен сигналов).              *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 27 jun 2017 at 10:10 *
// ********************************************************************************************************************

#pragma once

#include "Sprout.h"

namespace tengu {
    
    class SproutProxy : public Sprout {
        
        Q_OBJECT
        
        public:
            
            SproutProxy ( AbstractAgentKernel * owner = nullptr );
            virtual ~SproutProxy();
            
            virtual void subscribe();
            virtual void unsubscribe();
            virtual void subscribed( QString channel );
            virtual void unsubscribed( QString channel );
            virtual bool handleMessage( QString channel, QString message );     
            
    };
    
};
