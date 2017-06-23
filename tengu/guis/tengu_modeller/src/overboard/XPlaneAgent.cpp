// ********************************************************************************************************************
// *                                                                                                                  *
// *                   The representation X-Plane simulator as the "agent" using XTengu plugin.                       *
// * ---------------------------------------------------------------------------------------------------------------- *
// *             Представление симулятора X-Plane в качестве агента с использованием плагина XTengu.                  *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 11 jun 2017 at 20:24 *
// ********************************************************************************************************************

#include "XPlaneAgent.h"

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                 The constructor.                                                 *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                   Конструктор.                                                   *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::XPlaneAgent::XPlaneAgent() 
    : tengu::AbstractAgent()
{
    
    AbstractEntity::setSystemName("X-Plane");
    AbstractEntity::setHumanName("X-Plane");
    
    AbstractEntity::setExecutionMode( AbstractEntity::EM_XPLANE );
    
    CREATE_XTENGU_SETTINGS;
    settings.beginGroup("CommonControl");
    __controlGroup = settings.value("ControlNameGroup", "xtengu.control").toString();
    __conditionGroup = settings.value("ConditionNameGroup", "xtengu.condition").toString();
    settings.endGroup();
    
    __init_sprout( "LeftAileron", Sprout::EXTERNAL_OUTPUT, tr("Left aileron") );
    __init_sprout( "LeftFlaperon", Sprout::EXTERNAL_OUTPUT, tr("Left flaperon") );
    __init_sprout( "LeftElevator", Sprout::EXTERNAL_OUTPUT, tr("Left elevator" ) );
    __init_sprout( "LeftRudder", Sprout::EXTERNAL_OUTPUT, tr("Left rudder" ) );    
    __init_sprout( "LeftBrake", Sprout::EXTERNAL_OUTPUT, tr("Left brake" ) );
    
    __init_sprout( "RightAileron", Sprout::EXTERNAL_OUTPUT, tr("Right aileron" ) );
    __init_sprout( "RightFlaperon", Sprout::EXTERNAL_OUTPUT, tr("Right flaperon" ) );
    __init_sprout( "RightElevator", Sprout::EXTERNAL_OUTPUT, tr("Right elevator" ) );
    __init_sprout( "RightRudder", Sprout::EXTERNAL_OUTPUT, tr("Right rudder" ) );
    __init_sprout( "RightBrake", Sprout::EXTERNAL_OUTPUT, tr("Right brake" ) );
       
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                               Translation one section of settings to set of sprouts.                             *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                            Перевод одной секции установок в некое множество "ростков".                           *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::XPlaneAgent::__init_sprout ( QString settingsGroup, tengu::Sprout::sprout_type_t type, QString name ) {
    
    CREATE_XTENGU_SETTINGS;
    settings.beginGroup( settingsGroup );
    QString input = settings.value("input_channel", "").toString();
    QString output = settings.value("output_channel", "").toString(); 
    settings.endGroup();
    
    for ( int i=0; i<TOTAL_AIRCRAFTS_COUNT; i++ ) {
        Sprout * sp = new Sprout( this );
        sp->setSystemName( name );
        sp->setHumanName( name );
        sp->setSproutType( type );
        
        // Crossing. Because output for sprout is an input for X-Plane.
        // Переворот, потому что выход для "ростка" - это вход для X-Plane.
        
        if ( type == Sprout::EXTERNAL_INPUT ) 
            sp->setSignalName( output ); 
        else 
            sp->setSignalName( input );
        
        addSprout( sp );
    };
    
    // qDebug() << "XPlaneAgent::__init_sprout, total sprouts=" << sproutsCount();
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                            Overrided setting name method to avoid change predefined name.                        *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                  Перекрытый метод установки имени, чтобы избежать изменения предопределенного имени.             *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::XPlaneAgent::setName ( QString name ) {
    qDebug() << "XPlaneAgent::setName() does not have any effect";
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                            Overrided setting execution mode method to avoid it's change.                         *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                   Перекрытый метод установки режима выполнения, чтобы избежать его изменение.                    *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::XPlaneAgent::setExecutionMode ( tengu::AbstractEntity::execution_mode_t mode ) {
    qDebug() << "XPlaneAgent::setExecutioMode() does not have any effect";
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                  The destructor.                                                 *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                     Деструктор.                                                  *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::XPlaneAgent::~XPlaneAgent() {
}



