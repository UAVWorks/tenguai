// ********************************************************************************************************************
// *                                                                                                                  *
// *         The agent which will waiting the availability of at least one of his sprouts (inputs) after activity.    *
// * ---------------------------------------------------------------------------------------------------------------- *
// *              Агент, который будет ждать хотя бы одного из своих входов ("веток") после активизации.              *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 09 jun 2017 at 09:44 *
// ********************************************************************************************************************

#pragma once

#include "AbstractAgentKernel.h"

namespace tengu {
    
    class ORer : public AbstractAgentKernel {
        
        Q_OBJECT
        
        public:
            
            ORer ();
            virtual ~ORer();
            
    };
};
