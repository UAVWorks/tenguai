// ********************************************************************************************************************
// *                                                                                                                  *
// *                                             Abstract reguator class.                                             *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                          Класс абстрактного регулятора.                                          *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 24 may 2017 at 10:59 *
// ********************************************************************************************************************

#include "AbstractRegulator.h"

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                 The constructor.                                                 *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                   Конструктор.                                                   *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::AbstractRegulator::AbstractRegulator( QString name, float min_value, float max_value ) 
    : AbstractAgent( name ) 
{
    
    //CREATE_SETTINGS_ONBOARD;
    //settings_onboard.beginGroup( _name );
    //_output_channel = settings_onboard.value( "output_channel", "" ).toString();
    //settings_onboard.endGroup();
    
    
    // The PID-regulator and it's values.
    // ПИД-регулятор и его значения.
    
    // _pid = new PID( section, 0.0, min_value, max_value );
    _input_value = 0.0;
    _desired_value = 0.0;
    __active = false;
    
        
    
}


// ********************************************************************************************************************
// *                                                                                                                  *
// *                                    Get redis prefix for this regulator's messages.                               *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                  Получить префикс сообщений этого регулятора в редисе.                           *
// *                                                                                                                  *
// ********************************************************************************************************************

QString tengu::AbstractRegulator::prefix( QString section ) {
    return ( QString("tengu.regulators.") + section.toLower() + "." );    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                   Subscribe on the common regulator's channels.                                  *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                      Подписка на общие каналы регуляторов.                                       *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractRegulator::__subscribe() {
    /*
    if ( ( __sub_redis ) && ( __sub_redis_connected ) ) {
        
        // The activity of this regulator.
        // Активность этого регулятора.
        
        __sub_redis->subscribe( AbstractRegulator::prefix( _section ) + "activity" );
        
        CREATE_SETTINGS_ONBOARD;
        settings_onboard.beginGroup( _section );
        _input_channel = settings_onboard.value( "input_channel", "").toString();
        _desired_channel = settings_onboard.value( "desired_channel", "" ).toString();
        qDebug() << "Desired channel=" << _desired_channel ;
        settings_onboard.endGroup();
        
        // Input value for this regulator
        // Входное значение для этого регулятора.
        
        if ( _input_channel.length() > 0 )    __sub_redis->subscribe( _input_channel );
        
        // Desired value for this regulator.
        // Желаемое значение для данного регулятора.
        
        if ( _desired_channel.length() > 0 ) __sub_redis->subscribe( _desired_channel );
        
        // Channels for P-I-D on-the-fly changes
        // Каналы для установки значений P-I-D "на лету".
        
        __sub_redis->subscribe( AbstractRegulator::prefix( _section ) + "P" );
        __sub_redis->subscribe( AbstractRegulator::prefix( _section ) + "I" );
        __sub_redis->subscribe( AbstractRegulator::prefix( _section ) + "D" );
        
    };
    */
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                          Do one step for this regulator.                                         *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                       Сделать один шаг данного регулятора.                                       *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractRegulator::__do_step() {

    /*
    if ( ( __active ) && ( _output_channel.length() > 0 ) && ( __pub_redis ) && ( __pub_redis_connected ) ) {
        
        float oval = _pid->step( _input_value, _desired_value );
        qDebug() << "Publish value " << oval;
        __pub_redis->publish( _output_channel, QString::number(oval) );
        
    };
    */
    
}



// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                  The destructor.                                                 *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                     Деструктор.                                                  *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::AbstractRegulator::~AbstractRegulator() {

}


