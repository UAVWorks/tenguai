// ********************************************************************************************************************
// *                                                                                                                  *
// *                                            The user ( the base ) aircraft.                                       *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                          Пользовательский (основной) самолет.                                    *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 03 may 2017 at 10:45 *
// ********************************************************************************************************************

#pragma once 

#include "AbstractAircraft.h"
#include "XPlaneChannel.h"
#include "XPlaneChannel_0_100.h"

#include "XChThrottle.h"
#include "XChSteeringWheel.h"

#include "XChLeftAileron.h"
#include "XChLeftFlaperon.h"
#include "XChLeftElevator.h"
#include "XChLeftRudder.h"

#include "XChRightAileron.h"
#include "XChRightFlaperon.h"
#include "XChRightElevator.h"
#include "XChRightRudder.h"

namespace tengu {
    
    class UserAircraft : public AbstractAircraft {
        
        public:
            UserAircraft();
            virtual ~UserAircraft();
            
            // Overrided function for publish movement information
            // Перекрытая функция для публикации информации о движении.
            
            void publishMovement( LoRedis * redis );
            
    };
    
}; // namespace tengu

