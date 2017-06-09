// ********************************************************************************************************************
// *                                                                                                                  *
// *                The agent which will be waiting the availability of all his inputs after activity.                *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                    Агент, который будет ждать наличия всех входов после получения активности.                    *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 09 jun 2017 at 10:02 *
// ********************************************************************************************************************

#pragma once

#include "AbstractAgent.h"

namespace tengu {

    class ANDor : public AbstractAgent {
        
        Q_OBJECT
        
        public:
            
            ANDor ( QString systemName );
            virtual ~ANDor();
            
    };
    
};


