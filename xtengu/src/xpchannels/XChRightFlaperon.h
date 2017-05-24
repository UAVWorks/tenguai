// ********************************************************************************************************************
// *                                                                                                                  *
// *                     Class for control right flaperon (or flap, this is depending on plane construction).         *
// * ---------------------------------------------------------------------------------------------------------------- *
// *       Класс для управления отклонением правого флаперона (или закрылка, это зависит от конструкции самолета )    *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 10 may 2017 at 11:26 *
// ********************************************************************************************************************

#pragma once

#include "XChFloatArray.h"

namespace tengu {
    
    class XChRightFlaperon : public XChFloatArray {
        
        public:
            
            XChRightFlaperon ();
            virtual ~XChRightFlaperon();
            
    };
};
