// ********************************************************************************************************************
// *                                                                                                                  *
// *                                            The user ( the base ) aircraft.                                       *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                          Пользовательский (основной) самолет.                                    *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 03 may 2017 at 10:46 *
// ********************************************************************************************************************

#include "UserAircraft.h"

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                   The constructor.                                               *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::UserAircraft::UserAircraft() 
    : tengu::AbstractAircraft()
{
    
    setChannel( new XChLeftAileron() );    
    setChannel( new XChLeftFlaperon() );
    setChannel( new XChLeftElevator() );
    setChannel( new XChLeftRudder() );
    
    // The global position of aircraft (WGS-84).
    // Глобальное положение самолета (WGS-84)
    
    // setChannel( new XPlaneChannel("Altitude", "sim/flightmodel/position/elevation" ) );
    // setChannel( new XPlaneChannel("Longitude", "sim/flightmodel/position/longitude" ) );
    // setChannel( new XPlaneChannel("Latitude", "sim/flightmodel/position/latitude" ) );   
    
    // Left wheel brake force
    // Усилие торможения левого колеса.
    
    setChannel( new XPlaneChannel("LeftBrake" , "sim/flightmodel/controls/l_brake_add", 0, 100 ) );
    
    setChannel( new XChRightAileron() );
    setChannel( new XChRightFlaperon() );    
    setChannel( new XChRightElevator() );
    setChannel( new XChRightRudder() );
    
    // Right wheel brake force
    // Усилие торможения правого колеса.
    
    setChannel( new XPlaneChannel( "RightBrake" , "sim/flightmodel/controls/r_brake_add", 0, 100 ) );
        
    setChannel( new XChThrottle() );    
    setChannel( new XChSteeringWheel() );
    setChannel( new XPlaneChannel_0_100( "ParkingBrake", "sim/flightmodel/controls/parkbrake" ) );
    
    // Steering wheel angle restriction for minimum and maximum speed
    // We have switch off this parameter in the XPlane because we want to control using exterlan software.
    
    // Ограничение отклонения рулевого колеса на максимальной и минимальной скорости.
    // Этот параметр отключаем в X-Plane, потому что будем управлять внешним программным обеспечением.
    
    setChannel( new XPlaneChannel( "SteeringWheelLowSpeedAngle", "sim/aircraft/gear/acf_nw_steerdeg1", -60, 60 ) );
    setChannel( new XPlaneChannel( "SteeringWheelHighSpeedAngle", "sim/aircraft/gear/acf_nw_steerdeg2", -60, 60 ) );
    
    // Speeds (ground and indicated air)
    // Скорости (относительно земли и индикаторная относительно воздуха)
    
    setChannel( new XPlaneChannel( "IAS", "sim/flightmodel/position/indicated_airspeed" ) );
    setChannel( new XPlaneChannel( "GroundSpeed", "sim/flightmodel/position/groundspeed" ) );
    
    // Position of aircraft in the space.
    // Положение самолета в пространстве.
    
    setChannel( new XPlaneChannel("Heading", "sim/flightmodel/position/mag_psi" ) );
    setChannel( new XPlaneChannel("Pitch", "sim/flightmodel/position/theta" ) );
    setChannel( new XPlaneChannel("Roll", "sim/flightmodel/position/phi" ) );
    
    // The altitude above ground level.
    // Высота относительно земли
    
    setChannel( new XPlaneChannel("AGL", "sim/flightmodel/position/y_agl" ) );
    
    // 3d rotation speed rate (gyroscope)
    // скорость вращения в 3х осях (гироскоп)
    
    setChannel( new XPlaneChannel("PitchRate", "sim/flightmodel/position/Q" ) );
    setChannel( new XPlaneChannel("RollRate", "sim/flightmodel/position/P" ) );
    setChannel( new XPlaneChannel("YawRate", "sim/flightmodel/position/R" ) );
    
    setChannel( new XChRelocate(0) );
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                Overrided function for publish movement information.                              *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                              Перекрытая функция для публикации информации о движении.                            *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::UserAircraft::publishMovement ( LoRedis* redis ) {
    
    // Call abstract aircraft's publishing.
    // Вызов публикации абстрактного самолета.
    
    tengu::AbstractAircraft::publishMovement ( redis );
    
    // Altitude above the ground level.
    // Высота относительно уровня земли.
    
    float agl = get_f("AGL");
    publish( redis, "AGL", QString::number( agl, 'f', 4 ) );
    
    // 3d rotation speed rate (gyroscope)
    // 3д скорость вращения (гироскоп)
    
    float pitch_rate = get_f("PitchRate");
    publish( redis, "PitchRate", QString::number( pitch_rate, 'f', 6 ) );
    
    float roll_rate = get_f("RollRate");
    publish( redis, "RollRate", QString::number( roll_rate, 'f', 6 ) );
    
    float yaw_rate = get_f("YawRate");
    publish( redis, "YawRate", QString::number( yaw_rate, 'f', 6 ) );
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                   The desctructor.                                               *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                      Деструктор.                                                 *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::UserAircraft::~UserAircraft() {

}

