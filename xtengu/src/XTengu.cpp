// ********************************************************************************************************************
// *                                                                                                                  *
// *                                             The main class of X-Tengu.                                           *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                            Главный класс плагина X-Tengu.                                        *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 03 may 2017 at 09:31 *
// ********************************************************************************************************************

#include "XTengu.h"


// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                   The constructor.                                               *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::XTengu::XTengu() 
    : __enabled( false )
    , __redis( new LoRedis( this ) )
    , __redis__connected( false )
    , __sub_redis( new LoRedis( this ) )
    , __sub_redis__connected( false )
    , __observingAircraftsFrequency( 2.0 )
    , __in_observation(false)
{
        
    // Fill array of aircrafts.
    // Заполнение массива самолетов.
    
    __acfs.append( new UserAircraft() );
    for ( int i=1; i<TOTAL_AIRCRAFTS_COUNT; i++ ) {
        __acfs.append( new EnvironmentAircraft( i ) );
    };
    

    // Connect both adapters to Redis
    // Соединение обоих адаптеров с Redis-сервером.
    
    __redis->connect();
    __sub_redis->connect();
        
    // Truncate log file
    // Обрезаем файл журнала.
    
    FILE * lf = fopen( XPLUGIN_LOG_FILE, "w");
    if ( lf ) {
       fclose( lf );
    };
    
    xplugin__log( QString( "Plugin started." ) );
    
    // We will wait for permission from X-Plane in an explicit form.
    // Будем ждать разрешения от X-Plane в явном виде.
    
    XPlaneOverrided xpo;
    xpo.release();
    
       
    // --------------------------------------------------------------------------------------------
    // Not here! At the time of the plug-in's enabling!
    // Не здесь! На момент разрешения работы плагина!
    // __init_settings();
    // --------------------------------------------------------------------------------------------
    
    __totalAircrafts = 0; __activeAircrafts = 0;
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                            LoRedis connect event handlers.                                       *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::XTengu::cbConnected( LoRedis * redis ) {
    
    if ( redis == __redis ) __redis__connected = true;
    else if ( redis == __sub_redis ) {
        
        __sub_redis__connected = true;
            
        // Apply the settings and subscribe to channels.
        // Принимаем установки и подписываемся на каналы.
        
        __init_settings();                        
        
    }

}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                         LoRedis disconnect event handlers.                                       *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::XTengu::cbDisconnected( LoRedis * redis ) {
        
    if ( redis == __redis ) {
        __redis__connected = false;
    }
    
    else if ( redis == __sub_redis ) {
        __sub_redis__connected = false;
    };    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                             LoRedis errors event handlers.                                       *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::XTengu::cbError ( LoRedis * redis, QString error ) {
    xplugin__log( QString("--- Redis error: ") + error );
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                       We has been subscribed to specific channel.                                * 
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                           Нас подписали на определенный канал.                                   *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::XTengu::cbSubscribed ( LoRedis* redis, QString channel ) {
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                     We has been unsubscribed from specific channel                               *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                         Нас отписали от определенного канала.                                    *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::XTengu::cbUnsubscribed ( LoRedis* redis, QString channel ) {
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                       Setting of variable was successfull.                                       *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                       Установка переменной прошла успешно.                                       *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::XTengu::cbSetted ( LoRedis* redis, int status ) {
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                     We got the message from specific channel which we are subscribed to.                         *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                   Мы получили сообщение с определенного канала, на который мы были подписаны.                    *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::XTengu::cbGotMessage ( LoRedis * redis, QString channel, QString message ) {
    
    if ( ! __enabled ) return;
    
    xplugin__log( QString("--- in cb: We got a message. Channel=") + channel + ", msg=" + message );
    bool ok = false;
    float val = message.toFloat( &ok );
    if ( ok ) {
        
        // Value was transformed to float. Looking for an aircraft which was subscribed on this channel.        
        // Значение было преобразовано в число с плавающей точкой. Ищем самолет, который был подписан на этот канал.
        
        for ( int i=0; i<__acfs.length(); i++ ) {
            
            XPlaneChannel * pch = __acfs.at(i)->getChannelSubscribedTo( channel );
            
            if ( ( pch ) && ( pch->usable() ) ) {
                xplugin__log( QString("We got one channel, acf=") + QString::number(i) + ", name=" + pch->getName() );
                pch->set( val );
            };
        };
        
    } else {
        xplugin__log( QString("Got mesage but can't transform it to float value, channel=") 
            + channel + ", message=" + message );
    };
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                       We got the value of specific variable.                                     *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                    Мы получили значение определенной переменной.                                 *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::XTengu::cbGotValue ( LoRedis* redis, QString name, QVariant value ) {

}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                              Callback procedure for observing aircraft's movement.                               *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                             Callback-процедура для наблюдения за движением самолетов.                            *
// *                                                                                                                  *
// ********************************************************************************************************************

float tengu::XTengu::cbObservingMovement() {

    if ( ! __enabled ) return ( float ) 0.5 ;
    if ( __in_observation ) return ( float ) 0.5;
    __in_observation = true;
        
        
    // User aircraft must be existing in any case
    // Пользовательский самолет должен присутствовать в любом случае.
    
    int count = 1;
    if ( __activeAircrafts > count ) count = __activeAircrafts;
    
    // The loop for every each active aircraft.
    // Цикл по активным самолетам.
    
    if ( ( __redis ) &&  ( __redis__connected ) ) {

        for ( int i=0; i<count; i++ ) {
            __acfs.at(i)->publishMovement( __redis );
        };
                    
    };
    
    // For only one from the redis adapters, this is enought 
    // Только для одного из редис-адаптеров, этого будет достаточно.
        
    LoRedis::processEvents();
                
    // The next time to call this procedure, in the seconds.
    // Следующее время вызова этой процедуры в секундах.
    
    __in_observation = false;
    return ( 1.0 / __observingAircraftsFrequency );
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *   Output warning / error / information message to log file inside X-Plane/Resources/plugins/xtengu directory     *
// * ---------------------------------------------------------------------------------------------------------------- *
// *              Выводе предупреждения / ошибки / информационного сообщения в файл журнала, внутри папки             *
// *                                          X-Plane/Resources/plugins/xtengu                                        *
// *                                                                                                                  *
// ********************************************************************************************************************

/*
void tengu::XTengu::log ( QString message ) {
    
    FILE * fd = fopen( XPLUGIN_LOG_FILE, "a" );
    if ( fd ) {
        time_t now;
        char c_time[128];
        time( & now );
        struct tm * now_tm = localtime( &now );
        fprintf( fd, "%4d-%02d-%02d %02d:%02d:%02d  %s\n", 
                now_tm->tm_year + 1900, now_tm->tm_mon + 1, now_tm->tm_mday, 
                now_tm->tm_hour, now_tm->tm_min, now_tm->tm_sec, 
                 message.toLocal8Bit().constData() 
        );
        fclose( fd );
    };

}
*/

// ********************************************************************************************************************
// *                                                                                                                  *
// *                   Take settings for one channel, subscribe to receive messages from this channel.                *
// * ---------------------------------------------------------------------------------------------------------------- *
// *      Взять установки для одного канала, запомнить их, подписаться на получение сообщений в этом канале.          *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::XTengu::__take_settings_and_subscribe( QString section, QString inputDefault, QString outputDefault ) {
    
    CREATE_XTENGU_SETTINGS;
    settings.beginGroup( section );
    
    QString controlChannel = settings.value("input_channel", inputDefault ).toString();
    QString conditionChannel = settings.value("output_channel", outputDefault ).toString();
    
    // Cycle for each one aircraft.
    // Цикл по самолетам.
    
    // xplugin__log( QString("Looking at the ") + section );
    
    for ( int i=0; i<__acfs.size(); i++ ) {
        
        QString fullControlName = QString(__controlNameGroup) + ".acf_" + QString::number(i) + "." + controlChannel;
        QString fullConditionName = QString( __conditionNameGroup ) + ".acf_" + QString::number(i) + "." + conditionChannel;
        
        AbstractAircraft * acf = __acfs.at(i);
        
        // We have exactly one channel. Try to find this channel for this aircraft.
        // У нас есть в точности один канал. Пытаемся найти данный канал для данного самолета.
        
        XPlaneChannel * acf_channel = acf->getChannel( section );
                
        if ( ( acf_channel ) && ( acf_channel->getName() == section )) {
        
            // xplugin__log( QString("Got channel for subscribtion, acf index=") + QString::number( i ) );
            
            acf->clearSubscribtion( acf_channel );
            
            // There is such channel for this aircraft.
            // Такой канал есть для данного самолета.
            
            acf_channel->redisControlChannel = fullControlName;
            acf_channel->redisConditionChannel = fullConditionName;
            
            if ( __sub_redis__connected ) {
                
                // If we are connected - doing the subscribtion right now.
                // Если мы соединены - подписка на канал идет сразу же
                
                __sub_redis->subscribe( fullControlName );
                
            };
            
            /*else {
                
                // We will postpone the subscription if we do not yet have a connection
                // Если мы еще не соединены - откладываем подписку на канал.
                
                xplugin__log( QString("We are not connected, deffer subscribtion") );
                acf->setDeferredSubscribe( acf_channel );
                
                // acf_channel->deferredSubscribe = true;
            }
            */
                        
        }
        
    };
    
    settings.endGroup();
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                Reading config file.                                              *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                            Читаем конфигурационный файл.                                         *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::XTengu::__init_settings() {    
    
    CREATE_XTENGU_SETTINGS;
    
    // Reading common settings.
    // Читаем общие установки.
    
    settings.beginGroup("CommonControl");    
    
    __controlNameGroup = settings.value("ControlNameGroup", "xtengu.control" ).toString();
    __conditionNameGroup = settings.value("ConditionNameGroup", "xtengu.condition" ).toString();    
    __observingAircraftsFrequency = settings.value("ObservingMovementsFrequency", 2.0 ).toFloat();
    
    settings.endGroup();
    
    // channel-by-channel taking and redis subscribing
    // поканально берем установки и подписываемся в редисе.
    
    __take_settings_and_subscribe( "LeftAileron", "left_aileron", "left_aileron" );
    __take_settings_and_subscribe( "LeftFlaperon", "left_flaperon", "left_flaperon" );
    __take_settings_and_subscribe( "LeftElevator", "left_elevator", "left_elevator" );
    __take_settings_and_subscribe( "LeftRudder", "left_rudder", "left_rudder" );
    __take_settings_and_subscribe( "LeftBrake", "left_brake", "left_brake" );
    
    __take_settings_and_subscribe( "RightAileron", "right_aileron", "right_aileron" );
    __take_settings_and_subscribe( "RightFlaperon", "right_flaperon", "right_flaperon" );
    __take_settings_and_subscribe( "RightElevator", "right_elevator", "right_elevator" );
    __take_settings_and_subscribe( "RightRudder", "right_rudder", "right_rudder" );
    __take_settings_and_subscribe( "RightBrake", "right_brake", "right_brake" );
    
    __take_settings_and_subscribe( "Throttle", "throttle", "throttle" );    
    __take_settings_and_subscribe( "SteeringWheel", "steering_wheel", "steering_wheel" );
    __take_settings_and_subscribe( "ParkingBrake", "parking_brake", "parking_brake" );
    
    __take_settings_and_subscribe( "AGL", "agl", "agl" );
    __take_settings_and_subscribe( "PitchRate", "pitch_rate", "pitch_rate" );
    __take_settings_and_subscribe( "RollRate", "roll_rate", "roll_rate" );
    __take_settings_and_subscribe( "YawRate", "yaw_rate", "yaw_rate" );
    
    // Position, condition - for all aircrafts.
    // Положение, состояние - для всех самолетов.
    
    __take_settings_and_subscribe( "Longitude", "longitude", "longitude" );
    __take_settings_and_subscribe( "Latitude", "latitude", "latitude" );
    __take_settings_and_subscribe( "Altitude", "altitude", "altitude" );
    
    __take_settings_and_subscribe( "IAS", "ias", "ias" );    
    __take_settings_and_subscribe("GroundSpeed", "ground_speed", "ground_speed" );
        
    __take_settings_and_subscribe("Heading", "heading", "heading" );
    __take_settings_and_subscribe("Pitch", "pitch", "pitch" );
    __take_settings_and_subscribe("Roll", "roll", "roll" );
        
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                 Unsubscribe from all subscribed redis channels.                                  *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                        Отписаться от всех каналов редиса, на которые мы были подписаны.                          *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::XTengu::__unsibscribe() {
    
    if ( __sub_redis ) { // && ( __sub_redis__connected ) ) {
        
        // Отписка от всех каналов сразу и одной командой без параметров.
        
        __sub_redis->unsubscribe();
        
        for ( int i=0; i<__acfs.size(); i++ ) {
            __acfs.at(i)->clearSubscribtion();
        };
        
        /*
        for ( int i=0; i<__acfs.size(); i++ ) {
            AbstractAircraft * acf = __acfs.at(i);
            acf->unsubscribe( __sub_redis );
        }
        */
    };    
}



// ********************************************************************************************************************
// *                                                                                                                  *
// *                             Switch on all aircraft's control from the X-Plane side.                              *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                              Включить все управление самолетами со стороны X-Plane.                              *
// *                                                                                                                  *
// ********************************************************************************************************************

/*
void tengu::XTengu::__xplane_control_on() {
    
    if ( __override_joy_dataref ) XPLMSetDatai( __override_joy_dataref, 0 );
    if ( __override_artstab_dataref ) XPLMSetDatai( __override_artstab_dataref, 0 );
    if ( __override_flight_control_dataref ) XPLMSetDatai( __override_flight_control_dataref, 0 );
    if ( __override_autopilot_dataref ) XPLMSetDatai( __override_autopilot_dataref, 0 );
    if ( __override_throttle_dataref ) XPLMSetDatai( __override_throttle_dataref, 0 );
    if ( __override_control_surfaces_dataref ) XPLMSetDatai( __override_control_surfaces_dataref, 0 );

    // !!! Это - не надо !!! Оно влияет только на просчет сил от двигателя (моментов
    // инерции). Устанавливать обороты не получается, а моменты инерции надо бы сохранить.
    // if ( _override_engines_dataref ) XPLMSetDatai( _override_engines_dataref, 0 );

    // if ( control_wheel_dataref ) XPLMSetDatai( control_wheel_dataref, 0 );
    // if ( tail_lock_ratio_dataref ) XPLMSetDataf( tail_lock_ratio_dataref, 1.0 );

}
*/

// ********************************************************************************************************************
// *                                                                                                                  *
// *                            Switch off all aircraft's control from the x-plane side.                              *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                              Выключить все управление самолетами со стороны X-Plane.                             *
// *                                                                                                                  *
// ********************************************************************************************************************

/*
void tengu::XTengu::__xplane_control_off() {
    

    // Override joystic control.
    // Перекрытие управления при помощи джойстика.
    
    __override_joy_dataref = AircraftChannel::find_dataref("sim/operation/override/override_joystick");
    if ( __override_joy_dataref ) XPLMSetDatai( __override_joy_dataref, 1 );

    // Не влияет на колесико.
    // XPLMDataRef joy_heading_dataref = xplane__find_dataref("override_joystick_heading");
    // XPLMSetDatai( joy_heading_dataref, 1 );

    // Отключение системы Artifical Stability
    __override_artstab_dataref = AircraftChannel::find_dataref("sim/operation/override/override_artstab");
    if ( __override_artstab_dataref ) XPLMSetDatai( __override_artstab_dataref, 1 );

    // Отключение встроенной системы управления полетом.
    __override_flight_control_dataref =  AircraftChannel::find_dataref("sim/operation/override/override_flightcontrol");
    if ( __override_flight_control_dataref ) XPLMSetDatai( __override_flight_control_dataref, 1 );

    // Отключение встроенного автопилота.
    __override_autopilot_dataref = AircraftChannel::find_dataref("sim/operation/override/override_autopilot");
    if ( __override_autopilot_dataref ) XPLMSetDatai( __override_autopilot_dataref, 1 );

    // отключение встроенной системы управления оборотами двигателя.
    __override_throttle_dataref = AircraftChannel::find_dataref("sim/operation/override/override_throttles");
    if ( __override_throttle_dataref ) XPLMSetDatai( __override_throttle_dataref, 1 );


    // Обеспечение со стороны плагина возможности управления рулевыми поверхностями - не слаженно,
    // а каждой из них по отдельности, как угодно.
    __override_control_surfaces_dataref = AircraftChannel::find_dataref("sim/operation/override/override_control_surfaces");
    if ( __override_control_surfaces_dataref ) XPLMSetDatai( __override_control_surfaces_dataref, 1 );
    
    
    // !!! Это - не надо !!! Оно влияет только на просчет сил от двигателя (моментов
    // инерции). Устанавливать обороты не получается, а моменты инерции надо бы сохранить.
    // _override_engines_dataref = xplugin__find_dataref("sim/operation/override/override_engines");
    // if ( _override_engines_dataref ) XPLMSetDatai( _override_engines_dataref, 1 );


    // control_wheel_dataref = xplane__find_dataref("sim/operation/override/override_wheel_steer");
    // if ( control_wheel_dataref ) XPLMSetDatai( control_wheel_dataref, 1 );

    // XPLMDataRef gear_brake = xplane__find_dataref("sim/operation/override/override_gearbrake");
    // XPLMSetDatai( gear_brake, 1 );

    // tail_lock_ratio_dataref = xplane__find_dataref("sim/flightmodel/controls/tail_lock_rat");
    // if ( tail_lock_ratio_dataref ) XPLMSetDataf( tail_lock_ratio_dataref, 0.0 );


    // XPLMDataRef tlr = xplane__find_dataref("sim/multiplayer/controls/tail_lock_ratio");
    // float f[20]; memset( &f, 0, sizeof( float ) * 20 );
    // XPLMSetDatavf( tlr, &f[0], 0, 20 );

    // XPLMDataRef wheel_on = xplane__find_dataref("sim/cockpit2/controls/nosewheel_steer_on");
    // XPLMSetDatai( wheel_on, 1 );

}
*/

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                  Enable plugin.                                                  *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::XTengu::enable() {
    
    AbstractAircraft::getXPAircraftsNumber( & __totalAircrafts, & __activeAircrafts );

    XPlaneOverrided xpo;
    xpo.override();
    
    __enabled = true;
            
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                  Disable plugin.                                                 *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::XTengu::disable() {
    
    __enabled = false;
    
    XPlaneOverrided xpo;
    xpo.release();
    
    __totalAircrafts = 0; __activeAircrafts = 0;                
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                            Message abount airplane count has been changed in the X-Plane.                        *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                            Сообщение о том, что в X-Plane было изменено число самолетов.                         *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::XTengu::msg__AirplaneCountChanged() {
    AbstractAircraft::getXPAircraftsNumber( & __totalAircrafts, & __activeAircrafts );
}


// ********************************************************************************************************************
// *                                                                                                                  *
// *                              Message from X-Plane: the aircraft was loaded (re-loaded).                          *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                              Сообщение от X-Plane: самолет был загружен (пере-загружен).                         *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::XTengu::msg__AircraftLoaded( int index ) {
    xplugin__log( QString("Aircraft was loaded, ") + QString::number( index ) );

    /*
    if ( ( index == 0 ) && ( __acfs.length() > 0 ) ) {
        
        // There was (re-)loaded the user aircraft.
        // Это был (пере-)загружен пользовательский самолет.
        
        // Change the maximum angles for steering wheel.
        // Переставляем углы поворота рулевого колеса.
        
        AbstractAircraft * user_acf = __acfs.at(0);
        if ( user_acf ) {
            user_acf->set("SteeringWheelLowSpeedAngle", 45.0 );
            user_acf->set("SteeringWheelHighSpeedAngle", 45.0 );
        };
        
    };
    
    if ( ( __redis ) && ( __redis__connected ) ) {
        
        // For any (re-)loaded aircrafts we need to publish message about his model.
        // Для каждого из (пере-) загруженных самолетов нам нужно опубликовать сообщение о его модели.
        
        QString file, path;
        AbstractAircraft::getXPlaneModel( index, file, path );
        xplugin__log( QString("Got aircraft index=" + QString::number(index) + ": ") + file + ", path=" + path );
    };
    */
            
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                     An airport has been loaded in the X-Plane.                                   *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                          В X-Plane был загружен аэропорт.                                        *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::XTengu::msg__AirportLoaded() {
    xplugin__log( QString("Airport was loaded.") );
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                   The destructor.                                                *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::XTengu::~XTengu() {
        
    if ( __redis ) {
        if ( __redis__connected ) __redis->disconnect();
        delete( __redis );
    }
    
    if ( __sub_redis) {
        if ( __sub_redis__connected ) {
            __unsibscribe();
            __sub_redis->disconnect();
        }
        delete( __sub_redis );
    }
    
    QThread::usleep( 200 );

}

