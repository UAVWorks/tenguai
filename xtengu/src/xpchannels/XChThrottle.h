// ********************************************************************************************************************
// *                                                                                                                  *
// *                                              Throttle control channel.                                           *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                            Канал управления ручкой газа.                                         *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 06 may 2017 at 15:32 *
// ********************************************************************************************************************

#pragma once

#include "XPlaneChannel.h"

extern void xplugin__log( QString message );

namespace tengu {
    
    // A throttle channel establishes a position that controls the position of engine's handle for all possible engines 
    // at once. There are max 8 of them in the X-Plane. The position of the handle goes in percent, from 0 to 100. 
    // It can be real or integer, both functions does the same thing.
    
    // Канал газа устанавливает положение ручки управления двигателем для всех возможных двигателей сразу. 
    // В X-Plane их может быть до 8 максимально. Положение ручки идет в процентах, от 0 до 100. Устанавливаемое 
    // значение может быть хоть вещественным, хоть целым, обе функции делают одно и то же.
    
    class XChThrottle : public XPlaneChannel {
    
        public:
            
            XChThrottle();
            ~XChThrottle();
            
            void set( int value );
            void set( float value );
    };
    
}; // namespace tengu
