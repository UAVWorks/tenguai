// ********************************************************************************************************************
// *                                                                                                                  *
// *                                         Class for control left elevator's array.                                 *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                              Класс для управления отклонением левого руля высоты (массивом).                     *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 10 may 2017 at 12:08 *
// ********************************************************************************************************************

#pragma once

#include "XChFloatArrayTwice.h"

namespace tengu {

    class XChLeftElevator: public XChFloatArrayTwice {
        
        public:
            
            XChLeftElevator ();
            virtual ~XChLeftElevator();
                
    };
};

