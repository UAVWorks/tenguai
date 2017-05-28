// ********************************************************************************************************************
// *                                                                                                                  *
// *                                       An environment (non-user, external) aircraft.                              *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                           Внешний (не-пользовательский, из окружающего пространства) самолет.                    *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 03 may 2017 at 10:37 *
// ********************************************************************************************************************

#pragma once 

#include <QString>

#include "AbstractAircraft.h"

namespace tengu {
    
    class EnvironmentAircraft : public AbstractAircraft {
        
        public:
            EnvironmentAircraft( int index );
            virtual ~EnvironmentAircraft();
            
    };
    
}; // namespace tengu
