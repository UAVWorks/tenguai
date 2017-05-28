// ********************************************************************************************************************
// *                                                                                                                  *
// *                                         X-Plane's control overrided elements                                     *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                        Перекрытые элементы управления X-Plane                                    *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 06 may 2017 at 13:24 *
// ********************************************************************************************************************

#include "XPlaneOverrided.h"

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                   The constructor                                                *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::XPlaneOverrided::XPlaneOverrided() {
    
    __joystick = XPlaneChannel( "OverridedJoystick", "sim/operation/override/override_joystick" );
    __artstab = XPlaneChannel( "OverridedArtificalStability", "sim/operation/override/override_artstab" );
    __flight_control = XPlaneChannel("OverridedFlightControl", "sim/operation/override/override_flightcontrol");
    __autopilot = XPlaneChannel("OverridedAutopilot" , "sim/operation/override/override_autopilot" );
    __throttle = XPlaneChannel( "OverridedThrotthe", "sim/operation/override/override_throttles" );
    __control_surfaces = XPlaneChannel("OverridedControlSurfaces", "sim/operation/override/override_control_surfaces" );    

}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                      Override (take away) the control from X-Plane                               *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                    Перекрыть ( забрать на себя ) управление у X-Plane                            *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::XPlaneOverrided::override() {

    __joystick.set( 1 );
    __artstab.set( 1 );
    __flight_control.set( 1 );
    __autopilot.set( 1 );
    __throttle.set( 1 );
    __control_surfaces.set( 1 );
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                       Release (to give back) control to X-Plane                                  *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                   Отпустить (отдать ему обратно) управление X-Plane                              *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::XPlaneOverrided::release() {
    
    __joystick.set( 0 );
    __artstab.set( 0 );
    __flight_control.set( 0 );
    __autopilot.set( 0 );
    __throttle.set( 0 );
    __control_surfaces.set( 0 );
    
}


// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                   The desctructor                                                *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::XPlaneOverrided::~XPlaneOverrided() {
}

