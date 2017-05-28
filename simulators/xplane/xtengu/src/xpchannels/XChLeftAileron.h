// ********************************************************************************************************************
// *                                                                                                                  *
// *  Class for control left-aileron deflection. Based on array because it controls all existing ailerons togehter.   *
// * ---------------------------------------------------------------------------------------------------------------- *
// *  Класс для управления отклонением левого элерона. Основан на массиве, потому что управляет всеми существующими   *
// *                                            у модели элеронами одновременно.                                      *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 08 may 2017 at 17:30 *
// ********************************************************************************************************************

#pragma once

#include "XChFloatArray.h"

namespace tengu {

    class XChLeftAileron : public XChFloatArray {
        
        public:
            
            XChLeftAileron ();
            virtual ~XChLeftAileron();
    };
};

