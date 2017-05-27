// ********************************************************************************************************************
// *                                                                                                                  *
// *                                              Simple reguator class.                                              *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                            Класс простого регулятора.                                            *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 24 may 2017 at 10:59 *
// ********************************************************************************************************************

#include "SimpleRegulator.h"

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                 The constructor.                                                 *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                   Конструктор.                                                   *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::SimpleRegulator::SimpleRegulator( QString name, float min_value, float max_value ) 
    : PrefixedAgent( QString("tengu.regulators.") + name + ".", name ) 
{

    // The PID-regulator and it's values.
    // ПИД-регулятор и его значения.
    
    _P = 1.0;
    _I = 0.0;
    _D = 0.0;
    
    _input_value = 0.0;
    _desired_value = 0.0;
    _output_value = 0.0;
    _min_value = min_value;
    _max_value = max_value;
    
    // The settings is more preferred that the values from mongodb.
    // Установки - более предпочтительны для канала, чем значения из монги.
    
    CREATE_SETTINGS_ONBOARD;
    settings_onboard.beginGroup( _name );
    _output_channel = settings_onboard.value( "output_channel", "" ).toString();
    _input_channel = settings_onboard.value( "input_channel", "" ).toString();
    _desired_channel = settings_onboard.value( "desired_channel", "" ).toString();
    bool ok = false;
    float p = settings_onboard.value("P", "").toFloat( & ok );
    if ( ok ) _P = p;
    float i = settings_onboard.value("I", "").toFloat( & ok );
    if ( ok ) _I = i;
    float d = settings_onboard.value("D", "").toFloat( & ok );
    if ( ok ) _D = d;
    settings_onboard.endGroup();
    
    // Add reactions for channels ( input, output, desired value )
    // Добавление реакций для каналов ( вход, выход, желаемое значение )
    
    // addReactionFor( _input_channel, reinterpret_cast<AbstractAgent::reaction_callback_t>( & (this->__on_input_received ) ) );
    // addReactionFor( _desired_channel, reinterpret_cast<AbstractAgent::reaction_callback_t>( & (this->__on_desired_received ) ) );
}


// ********************************************************************************************************************
// *                                                                                                                  *
// *                                    Get redis prefix for this regulator's messages.                               *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                  Получить префикс сообщений этого регулятора в редисе.                           *
// *                                                                                                                  *
// ********************************************************************************************************************
/*
QString tengu::SimpleRegulator::prefix( QString section ) {
    return ( QString("tengu.regulators.") + section.toLower() + "." );    
}
*/

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                   Subscribe on the common regulator's channels.                                  *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                      Подписка на общие каналы регуляторов.                                       *
// *                                                                                                                  *
// ********************************************************************************************************************

/*
void tengu::AbstractRegulator::_subscribe() {
    
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
}
*/

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                           Usability of this regulator.                                           *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                   Возможность использования данного регулятора.                                  *
// *                                                                                                                  *
// ********************************************************************************************************************

bool tengu::SimpleRegulator::usable() {
    return ( ( _input_channel.length() > 0 ) 
        && ( _output_channel.length() > 0 ) 
        && ( _desired_channel.length() > 0 ) 
        && ( isPublisherConnected() ) 
    );
}


// ********************************************************************************************************************
// *                                                                                                                  *
// *                                          Do one step for this regulator.                                         *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                       Сделать один шаг данного регулятора.                                       *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::SimpleRegulator::_do_step() {

    if ( ( usable() ) && ( isActive() ) ) {
        
        float error = _output_value - _desired_value;
        
        
        // __pub_redis->publish( _output_channel, QString::number(oval) );
        
    };
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                           Input value has been received.                                         *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                           Было получено входное значение.                                        *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::SimpleRegulator::__on_input_received( QString channel, QString message ) {
    Q_UNUSED( channel );
    bool ok = false;
    float val = message.toFloat( & ok );
    if ( ok ) _input_value = val;
    _do_step();
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                        The desired value has been received.                                      *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                          Было получено желаемое значение                                         *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::SimpleRegulator::__on_desired_received( QString channel, QString message ) {
    Q_UNUSED( channel );
    bool ok = false;
    float val = message.toFloat( & ok );
    if ( ok ) _desired_value = val;
    _do_step();
}


// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                  The destructor.                                                 *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                     Деструктор.                                                  *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::SimpleRegulator::~SimpleRegulator() {

}


