// ********************************************************************************************************************
// *                              Ground speed regulator for taxing and run-out stages.                               *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                            Регулятор скорости относительно земли в фазе руления и разбега.                       *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 24 may 2017 at 08:25 *
// ********************************************************************************************************************

#pragma once

#include <QObject>
#include <QTimer>
#include <QThread>
#include <QDebug>

#include "../tengu_onboard_common/AbstractRegulator.h"

namespace tengu {

    class TaxingSpeedRegulator : public AbstractRegulator {
        
        Q_OBJECT
        
        public:
            
            TaxingSpeedRegulator();
            ~TaxingSpeedRegulator();
        
        private:
                        
        private slots:
                        
            
    };
    
};

