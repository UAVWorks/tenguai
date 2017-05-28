// ********************************************************************************************************************
// *                                                                                                                  *
// *                                           Control of right rudder's position.                                    *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                      Управление положением правого руля направления.                             *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 10 may 2017 at 16:03 *
// ********************************************************************************************************************

#pragma once

#include "XChFloatArray.h"

namespace tengu {
    
    class XChRightRudder : public XChFloatArray {
        
        public:
            XChRightRudder ();
            virtual ~XChRightRudder();            
    };
    
}; // namespace tengu


