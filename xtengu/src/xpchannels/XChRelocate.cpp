// ********************************************************************************************************************
// *                                                                                                                  *
// *                                 The channel for aircraft relocation using one command.                           *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                             Канал для перемещения самолета с использованием одной команды.                       *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 25 may 2017 at 18:41 *
// ********************************************************************************************************************

#include "XChRelocate.h"

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                               The constructor.                                                   *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                  Конструктор.                                                    *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::XChRelocate::XChRelocate( int index )
    : XPlaneChannel("Relocate")
{
    if ( index == 0 ) {
        
        // User's aircraft.
        // Пользовательский самолет.
        
        __x_dataref = XPlaneChannel::find_dataref( "sim/flightmodel/position/local_x" );
        __y_dataref = XPlaneChannel::find_dataref( "sim/flightmodel/position/local_y" );
        __z_dataref = XPlaneChannel::find_dataref( "sim/flightmodel/position/local_z" );
        
        __heading_dataref = XPlaneChannel::find_dataref("sim/flightmodel/position/psi");
        
    } else {
        
        // Environment aircraft.
        // Самолет из окружения.
                
    }
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                         The usability of this channel.                                           *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                          Использование данного канала.                                           *
// *                                                                                                                  *
// ********************************************************************************************************************

bool tengu::XChRelocate::usable() {
    return ( __x_dataref && __y_dataref && __z_dataref && __heading_dataref );
}

// ********************************************************************************************************************
// *                                            Set value for this channel.                                           *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                       Установка значения для данного канала.                                     *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::XChRelocate::set ( QString message ) {
    
    if ( ! this->usable() ) return;
    
    QStringList contents = message.split(",");
    if ( contents.length() >= 4 ) {
        double latitude = contents.at(0).toDouble();
        double longitude = contents.at(1).toDouble();        
        double altitude = contents.at(2).toDouble();
        float heading = contents.at(3).toFloat();
        double x = 0.0, y=0.0, z=0.0;
        XPLMWorldToLocal( latitude, longitude, altitude, &x, &y, &z );
        XPLMSetDataf( __heading_dataref, (float) 10.0 );
        XPLMSetDatad( __x_dataref, x );
        XPLMSetDatad( __y_dataref, y );
        XPLMSetDatad( __z_dataref, z );
        
    };
}


// ********************************************************************************************************************
// *                                                                                                                  *
// *                                               The desctructor                                                    *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                 Деструктор.                                                      *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::XChRelocate::~XChRelocate() {

}

