// ********************************************************************************************************************
// *                                                                                                                  *
// *                                            Control of left rudder's position.                                    *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                      Управление положением левого руля направления.                              *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 10 may 2017 at 15:23 *
// ********************************************************************************************************************

#pragma once

#include "XChFloatArray.h"

namespace tengu {
    class XChLeftRudder : public XChFloatArray {
        public:
            XChLeftRudder ();
            virtual ~XChLeftRudder();
    };
}; // namespace tengu


