// ********************************************************************************************************************
// *                                                                                                                  *
// *                             An X-Plane simulator as a top level entity in the WorkSpace.                         *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                   Симулятор X-Plane как "сущность верхнего уровня" внутри рабочего пространства.                 *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 05 jun 2017 at 15:25 *
// ********************************************************************************************************************

#pragma once

#include "TopLevelEntity.h"
#include "XPlaneProcess.h"
#include "XTenguSettings.h"

#define XTENGU_ACTIVATION_MS 1500

namespace tengu {
    
    class XPlaneSimulator : public TopLevelEntity {
        
        Q_OBJECT
        
        public:
            
            static const QString XPLANE_SIMULATOR_UUID;        
            static const QList<QString> CONTROL_AIRCRAFTS_UUIDS;
            static const QList<QString> CONDITION_AIRCRAFTS_UUIDS;

            XPlaneSimulator();
            virtual ~XPlaneSimulator();
            
            XPlaneProcess * condition;
            XPlaneProcess * control;
            
        private:
            
            QTimer * __xtengu_presence_timer;
            QDateTime __last_xtengu_activity;
        
        private slots:
            
            void __on__xtengu_presence_timer();
            void __on__redis_got_value( QString channel, QVariant value );
            // void __on__some_sprout_got_value();
            
    };
    
};

