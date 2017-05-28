// ********************************************************************************************************************
// *                                                                                                                  *
// *                      Class for control left flaperon (or flap, this is depending on plane construction).         *
// * ---------------------------------------------------------------------------------------------------------------- *
// *        Класс для управления отклонением левого флаперона (или закрылка, это зависит от конструкции самолета )    *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 10 may 2017 at 11:25 *
// ********************************************************************************************************************

#pragma once

#include "XChFloatArray.h"

namespace tengu {

    class XChLeftFlaperon : public XChFloatArray {
        
        public:
            
            XChLeftFlaperon();
            ~XChLeftFlaperon();
    };
};
