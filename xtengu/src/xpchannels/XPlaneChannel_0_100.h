// ********************************************************************************************************************
// *                                                                                                                  *
// *                     Channel for transform 0..100 integer values to 0..1 float values.                            *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                  Канал для преобразования целых значений 0..100 в вещественные значения 0..1                     *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 20 may 2017 at 12:08 *
// ********************************************************************************************************************

#pragma once
#include "XPlaneChannel.h"

namespace tengu {

        class XPlaneChannel_0_100 : public XPlaneChannel {
            
            public:
                
                XPlaneChannel_0_100 ( QString aName, QString path );
                virtual ~XPlaneChannel_0_100();
                void set( int value );
                void set( float value );
                
        };
};

