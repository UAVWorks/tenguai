// ********************************************************************************************************************
// *                                                                                                                  *
// *                   The representation X-Plane simulator as the "agent" using XTengu plugin.                       *
// * ---------------------------------------------------------------------------------------------------------------- *
// *             Представление симулятора X-Plane в качестве агента с использованием плагина XTengu.                  *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 11 jun 2017 at 20:22 *
// ********************************************************************************************************************

#pragma once

#include <QSettings>
#include <QString>

#include "AbstractAgent.h"

// @todo remake that is the same with X-Tengu.
// Переделать так, чтобы было одинаково с X-Tengu.

#define CREATE_XTENGU_SETTINGS      QSettings settings( "/etc/tengu/xtengu.conf", QSettings::IniFormat )

namespace tengu {

    class XPlaneAgent : public AbstractAgent {
        
        Q_OBJECT
        
        public:
            
            XPlaneAgent();
            virtual ~XPlaneAgent();
            
        protected:
            
        private:
            QString __controlGroup;
            QString __conditionGroup;
            
    };
};

