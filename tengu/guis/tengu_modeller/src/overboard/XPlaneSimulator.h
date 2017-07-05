// ********************************************************************************************************************
// *                                                                                                                  *
// *                             An X-Plane simulator as a top level entity in the WorkSpace.                         *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                   Симулятор X-Plane как "сущность верхнего уровня" внутри рабочего пространства.                 *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 05 jun 2017 at 15:25 *
// ********************************************************************************************************************

#pragma once

#include "TopLevelEntity.h"
#include "XPlaneProcess.h"

namespace tengu {
    
    class XPlaneSimulator : public TopLevelEntity {
        
        Q_OBJECT
        public:
            
            XPlaneSimulator();
            virtual ~XPlaneSimulator();
            
    };
    
};

