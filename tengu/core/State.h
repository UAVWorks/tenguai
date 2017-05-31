// ********************************************************************************************************************
// *                                                                                                                  *
// *                                              The State for some agent.                                           *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                            Состояние некоторого агента.                                          *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 29 may 2017 at 07:18 *
// ********************************************************************************************************************

#pragma once

#include <QList>
#include "AbstractAgent.h"

namespace tengu {

    class State : public AbstractAgent {
        
        Q_OBJECT
        
        public:
            
            State( AbstractAgentKernel * parent, QString systemName );
            virtual ~State();
            
        private:
            
    };
    
};
