// ********************************************************************************************************************
// *                                                                                                                  *
// *                                        Steering wheel, usualy front (forward).                                   *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                             Рулевое колесо, обычно переднее.                                     *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 10 may 2017 at 16:27 *
// ********************************************************************************************************************

#pragma once

#include "XPlaneChannel.h"

namespace tengu {
    
    class XChSteeringWheel : public XPlaneChannel {
        
        public:
            
            XChSteeringWheel();
            virtual ~XChSteeringWheel();
        
            virtual void set( int value );
            virtual void set( float value );
        
    };
};

