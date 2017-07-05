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

#include "Vehicle.h"

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                    The constructor.                                              *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                      Конструктор.                                                *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::Vehicle::Vehicle () 
    : Processor ()
{
    
    _vehicleType = VT_NONE;
    
    // If configuration was not loaded from the processor.conf file, then load it from the vehicle.conf file
    // Если из файла processor.conf загружено не было, то загружаем из файла vehicle.conf
    
    if ( ! _configLoaded ) {
        _configLoaded = _loadConfig( VEHICLE_CONFIG_FILE );
    };
        
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                              Overrided function for add a child to vehicle.                                      *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                            Перекрытая функция добавления ребенка в "самоходку"                                   *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::Vehicle::addChild ( tengu::AbstractAgentKernel * child ) {
    
    tengu::AbstractAgentKernel::addChild ( child );
    
    QList<AbstractAgentKernel * > hisChildren = children();
    
    Process * process = dynamic_cast< Process * > ( child );
    if ( process ) {
        
        // Automatic naming the process
        // Автоматическое именование процесса.
        
        int processCount = 0;
        
        for ( int i=0; i<hisChildren.count(); i++ ) {
            AbstractAgentKernel * och = hisChildren.at(i);
            Process * pch = dynamic_cast<Process *>( och );
            if ( pch ) processCount ++;
        };
        
        if ( process->getSystemName().isEmpty() ) process->setSystemName( tr("New_Process_") + QString::number( processCount ) );
        if ( process->getHumanName().isEmpty() ) process->setHumanName( tr("New Process ") + QString::number( processCount) );
    };
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                 Get type of this vehicle.                                        *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                              Получить тип этой "самоходки".                                      *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::Vehicle::vehicle_types_h tengu::Vehicle::vehicleType() {
    return _vehicleType;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                      Return vehicle type as human readable string.                               *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                  Вернуть тип этой "самоходки" в виде читаемой строки.                            *
// *                                                                                                                  *
// ********************************************************************************************************************

QString tengu::Vehicle::vehicleTypeAsString() {
    
    switch( _vehicleType ) {
        case VT_NONE: return QString( tr( "Unknown" ) );
        case VT_AIRCRAFT: return QString( tr( "Airplane" ) );
        case VT_HELICOPTER: return QString( tr( "Helicopter" ) );
        case VT_QUADROCOPTER: return QString( tr( "Quadrocopter" ) );
    };
    return QString( tr("Unhandled type:") + QString::number( _vehicleType ) );
}


// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                     The destructor.                                              *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                       Деструктор.                                                *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::Vehicle::~Vehicle() {

}

