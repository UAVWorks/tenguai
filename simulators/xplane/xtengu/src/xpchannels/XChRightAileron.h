// ********************************************************************************************************************
// *                                                                                                                  *
// * Class for control right-aileron deflection. Based on array because it controls all existing ailerons togehter.   *
// * ---------------------------------------------------------------------------------------------------------------- *
// * Класс для управления отклонением правого элерона. Основан на массиве, потому что управляет всеми существующими   *
// *                                            у модели элеронами одновременно.                                      *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 10 may 2017 at 10:26 *
// ********************************************************************************************************************

#pragma once

#include "XChFloatArray.h"

namespace tengu {
    
    class XChRightAileron : public XChFloatArray {
        public:
            XChRightAileron ();
            virtual ~XChRightAileron();
    };    
}; // namespace tengu
