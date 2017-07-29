// ********************************************************************************************************************
// *                                                                                                                  *
// *                             An X-Plane simulator as a top level entity in the WorkSpace.                         *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                   Симулятор X-Plane как "сущность верхнего уровня" внутри рабочего пространства.                 *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 05 jun 2017 at 15:27 *
// ********************************************************************************************************************

#include "XPlaneSimulator.h"

const QString tengu::XPlaneSimulator::XPLANE_SIMULATOR_UUID = QString("9b914362-07a6-471b-ad64-f8028cfdc97e");

const QList<QString> tengu::XPlaneSimulator::CONTROL_AIRCRAFTS_UUIDS = QList<QString>({
    "{7dc03d65-c4e6-40fc-9b57-c85aac787cb0}"      // 0 - user aircraft
    , "{5d04260f-898b-472c-9421-fd996b70fbd1}"    // 1
});

const QList<QString> tengu::XPlaneSimulator::CONDITION_AIRCRAFTS_UUIDS = QList<QString>({
    "{f598aa98-df4c-46cc-9cec-0e59240b51f4}",
    "{2f3d7153-185f-43c0-ae3e-4ea8fe9ca869}"
});

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                    The constructor.                                              *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                      Конструктор.                                                *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::XPlaneSimulator::XPlaneSimulator() 
    : TopLevelEntity()
{
    _class_name = "XPlaneSimulator";
    AbstractEntity::setUUID( XPLANE_SIMULATOR_UUID );
    
    setSystemName( tr("xtengu") );
    setHumanName( tr("X-Plane simulation process"));
            
    setSystemName("xtengu");
    setHumanName( tr("X-Plane simulator") );
    
    condition = new XPlaneProcess();
    condition->setSystemName("condition");
    condition->setHumanName("Condition");
    addChild( condition );
    
    control = new XPlaneProcess();
    control->setSystemName("control");
    control->setHumanName("Control");
    addChild( control );
    
    CREATE_XTENGU_SETTINGS;
    settings.beginGroup("CommonControl");
    QString controlGroup = settings.value("ControlNameGroup", "xtengu.control").toString();
    QString conditionGroup = settings.value("ConditionNameGroup", "xtengu.condition").toString();
    settings.endGroup();
    
    for ( int idx = 0; idx < XPlaneSimulator::CONTROL_AIRCRAFTS_UUIDS.size(); idx++ ) {        
        
        XPlaneAircraft * acf_control = new XPlaneAircraft( idx, controlGroup, Sprout::SPT__EXTERNAL_INPUT );
        acf_control->setUUID( XPlaneSimulator::CONTROL_AIRCRAFTS_UUIDS.at( idx ) );
        control->addChild( acf_control );
        
        XPlaneAircraft * acf_condition = new XPlaneAircraft( idx, conditionGroup, Sprout::SPT__EXTERNAL_OUTPUT );
        condition->addChild( acf_condition );
                
    };
    
    _changed = false;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                     The destructor.                                              *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                        Деструктор.                                               *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::XPlaneSimulator::~XPlaneSimulator() {

}

