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
    condition->setHumanName("condition");
    addChild( condition );
    
    control = new XPlaneProcess();
    control->setSystemName("control");
    control->setHumanName("control");
    addChild( control );
    
    CREATE_XTENGU_SETTINGS;
    
    settings.beginGroup("CommonControl");
    QString controlGroup = settings.value("ControlNameGroup", "xtengu.control").toString();
    QString conditionGroup = settings.value("ConditionNameGroup", "xtengu.condition").toString();
    settings.endGroup();
    
    // Create airplanes.
    // Создание самолетов.
    
    for ( int idx = 0; idx < XPlaneSimulator::CONTROL_AIRCRAFTS_UUIDS.size(); idx++ ) {        
        
        XPlaneAircraft * acf_control = new XPlaneAircraft( idx, controlGroup, Sprout::SPT__EXTERNAL_INPUT );
        acf_control->setUUID( XPlaneSimulator::CONTROL_AIRCRAFTS_UUIDS.at( idx ) );
        control->addChild( acf_control );
                
        XPlaneAircraft * acf_condition = new XPlaneAircraft( idx, conditionGroup, Sprout::SPT__EXTERNAL_OUTPUT );
        acf_condition->setUUID( XPlaneSimulator::CONDITION_AIRCRAFTS_UUIDS.at( idx ) );
        condition->addChild( acf_condition );
        
        // Activity of "condition airplanes". Lets make a conclusion about the presence of the simulator.
        // Активность "самолетов состояния". Позволяет сделать вывод о наличии симулятора.
        
        // QList<Sprout * > acf_condition_sprouts = acf_condition->sprouts();
        // for ( int spIndex=0; spIndex < acf_condition_sprouts.count(); spIndex ++ ) {
        //    Sprout * sp = acf_condition_sprouts.at( spIndex );
        //    QObject::connect( sp, SIGNAL(signalGotValue(QVariant)), this, SLOT( __on__some_sprout_got_value() ) );
        //};        
                
    };
    
    _changed = false;
    
    // Timer for XTengu activity.
    // Таймер активности XTengu
    
    __xtengu_presence_timer = new QTimer( this );
    QObject::connect( __xtengu_presence_timer, SIGNAL( timeout() ), this, SLOT( __on__xtengu_presence_timer() ) );
    __xtengu_presence_timer->start( 500 );
    
    _activity = false;
    emit signalActivated( false );
    
    // For garanted non-activity state
    // Для гарантированного не-активного состояния.
    
    __last_xtengu_activity = QDateTime::currentDateTime();
    __last_xtengu_activity.addSecs( -2000 );
    
    QObject::connect( _pub_redis, SIGNAL( signalGotValue( QString, QVariant ) ), this, SLOT( __on__redis_got_value( QString, QVariant ) ) );
    connect();
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                               XTengu presence timer timeout.                                     *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                           Срабатывание таймера присутствия XTengu                                *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::XPlaneSimulator::__on__xtengu_presence_timer() {
        
    if ( isPublisherConnected() ) {
        _pub_redis->get( XTENGU_PRESENCE_PATH );
    };
    
    if ( _activity ) {
        
        // De-activation goes only by time.
        // Деактивация делается только по времени.
        
        QDateTime curTime = QDateTime::currentDateTime();
        qint64 ms = __last_xtengu_activity.msecsTo( curTime );
        
        if ( ms > 3000 ) {
            _activity = false;
            emit signalActivated( false );
        };
        
    };
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                      A value has been received from the redis.io                                 *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                            От редиса было получено значение.                                     *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::XPlaneSimulator::__on__redis_got_value ( QString channel, QVariant value ) {
    
    if ( ( channel == XTENGU_PRESENCE_PATH ) && ( ! value.isNull() ) ) {
        
        // May be we can activate a simulator process.
        // Возможно, мы уже можем активировать процесс симулятора.
        
        bool ok = false;
        double t = value.toDouble( &ok );
        if ( ok ) {
            
            qint64 secs = ( int ) t;
            int ms = ( t - secs ) * 1000;
            
            QDateTime got_dt;
            got_dt.setTime_t( secs );
            got_dt.addMSecs( ms );
            
            __last_xtengu_activity = got_dt;
            
            if ( ! _activity ) {
                qint64 delta = __last_xtengu_activity.msecsTo( QDateTime::currentDateTime() );
                if ( delta < XTENGU_ACTIVATION_MS ) {
                    _activity = true;
                    emit signalActivated( true );
                };
            };
        };
        
    };
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

