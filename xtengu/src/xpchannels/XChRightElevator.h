// ********************************************************************************************************************
// *                                                                                                                  *
// *                                        Class for control right elevator's array.                                 *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                             Класс для управления отклонением правого руля высоты (массивом).                     *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 10 may 2017 at 12:17 *
// ********************************************************************************************************************

#pragma once

#include "XChFloatArrayTwice.h"

namespace tengu {

    class XChRightElevator: public XChFloatArrayTwice {
        
        public:
            
            XChRightElevator ();
            virtual ~XChRightElevator();
            
    };
};

