// ********************************************************************************************************************
// *                                                                                                                  *
// *  Any vehicle (mobile robot: truck, car, aircraft e.t.c.). This one have really board (plate, main controller).   *
// *                                          This one can move in the environment space.                             *
// * ---------------------------------------------------------------------------------------------------------------- *
// *     Любой мобильный робот (грузовик, машинка, самолет и др.). Эта штучка реально имеет на борту управляющий      *
// *              контроллер (физическую плату). Плюс, эта штучка может передвигаться в пространстве.                 *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 29 may 2017 at 09:39 *
// ********************************************************************************************************************

#pragma once

#include <QSettings>
#include <Processor.h>

#define VEHICLE_CONFIG_FILE         "/etc/tengu/vehicle.conf"
#define VEHICLE_PID_FILE            "/var/run/tengu_vehicle.pid"
#define TEMP_PID_FILE               "/var/run/tentu_temp.pid"

namespace tengu {
    
    class Vehicle : public Processor {
        
        Q_OBJECT
        
        public:
            
            // Only for displaying, for human perception.
            // Только для отображения, для человеческого восприятия. 
            
            enum vehicle_types_t {                
                VT_NONE             = 0, 
                VT_AIRCRAFT         = 20,
                VT_HELICOPTER       = 30,
                VT_QUADROCOPTER     = 40
            };
            
            Vehicle ();
            virtual ~Vehicle();
            
            vehicle_types_t vehicleType();
            QString vehicleTypeAsString();
            
            virtual QJsonObject toJSON();
            virtual bool fromJSON( QJsonObject o );
            
            void addChild( AbstractAgent * child );
            
        protected:
            
            vehicle_types_t _vehicleType;
            
    };
    
};

