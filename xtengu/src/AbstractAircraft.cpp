// ********************************************************************************************************************
// *                                                                                                                  *
// *                                 An abstract aircraft, the ancestor of all real aircrafts.                        *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                   Абстрактный самолет, предок всех реальных самолетов.                           *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 03 may 2017 at 10:32 *
// ********************************************************************************************************************

#include "AbstractAircraft.h"

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                   The constructor.                                               *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::AbstractAircraft::AbstractAircraft() {

    // Local aircraft position
    // Локальная позиция самолета.
    
    setChannel( new XPlaneChannel( "Local_X", "sim/flightmodel/position/local_x") );
    setChannel( new XPlaneChannel( "Local_Y", "sim/flightmodel/position/local_y") );
    setChannel( new XPlaneChannel( "Local_Z", "sim/flightmodel/position/local_z") );
    
    // Local angles of aircraft
    // Локальные углы самолета.
    
    setChannel( new XPlaneChannel( "Local_PSI", "sim/flightmodel/position/psi") );
    setChannel( new XPlaneChannel( "Local_THETA", "sim/flightmodel/position/theta" ) );
    setChannel( new XPlaneChannel( "Local_PHI", "sim/flightmodel/position/phi") );
    
    // The global position of aircraft (WGS-84).
    // Глобальное положение самолета (WGS-84)
    
    setChannel( new XPlaneChannel("Altitude") );
    setChannel( new XPlaneChannel("Longitude") );
    setChannel( new XPlaneChannel("Latitude" ) );   
    
    // Speeds. Will be override by childrens classes.
    // Скорости. Будут перекрыты дочерними классами.
    
    // setChannel( new XPlaneChannel("IAS") );
    // setChannel( new XPlaneChannel("GroundSpeed" ) );
    
    // The position of aircraft in the space. Will be override by children's classes.
    // Положение самолета в пространстве. Будут перекрыты дочерними классами.
    
    // setChannel( new XPlaneChannel("Heading") );
    // setChannel( new XPlaneChannel("Pitch") );
    // setChannel( new XPlaneChannel("Roll") );
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                   Find control channel by name and return it.                                    *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                               Найти канал управления по его имени и вернуть его.                                 *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::XPlaneChannel * tengu::AbstractAircraft::getChannel(QString channelName) {
        
    return _channels.value( channelName, nullptr );
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                    Get physical model file and path for the specific aircraft from X-Plane.                      *
// * ---------------------------------------------------------------------------------------------------------------- *
// *             Получить из X-Plane имя файла и путь для физической модели самолета с данным индексом.               *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractAircraft::getXPlaneModel ( int index, QString & file, QString & path ) {
    
    char outFileName[1024];
    memset( outFileName, 0, sizeof( outFileName ) );
    char outPath[4096];
    memset( outPath, 0, sizeof( outPath ) );    
    
    XPLMGetNthAircraftModel( index, outFileName, outPath );
    file = QString( outFileName );
    path = QString( outPath );
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                          Get the total number of aircrafts presented in the X-Plane                              *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                             Получить число самолетов, представленных в X-Plane.                                  *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractAircraft::getXPAircraftsNumber( int * total, int * active ) {
    ( * total ) = 0; ( * active ) = 0;
    XPLMPluginID pid;
    XPLMCountAircraft( total, active, & pid );    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                     Get float value of channel identified by his name.                           *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                 Получить вещественное значение для канала по его имени.                          *
// *                                                                                                                  *
// ********************************************************************************************************************

float tengu::AbstractAircraft::get_f ( QString xpChannelName ) {
    
    float result = 0.0;
    XPlaneChannel * ch = getChannel( xpChannelName );
    if ( ( ch ) && ( ch->usable() ) ) {
        result = ch->get_f();
    };
    return result;
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                   Get double value for the specified channel.                                    *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                              Получить число с удвоенной точностью из данного канала.                             *
// *                                                                                                                  *
// ********************************************************************************************************************

double tengu::AbstractAircraft::get_d ( QString xpChannelName ) {
    double result = 0.0;
    XPlaneChannel * ch = getChannel( xpChannelName );
    if ( ( ch ) && ( ch->usable() ) ) {
        result = ch->get_d();
    };
    return result;
}


// ********************************************************************************************************************
// *                                                                                                                  *
// *                               Clear all subscribtion elements for this channel.                                  *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                               Очистка всех элементов подписки для данного канала.                                *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractAircraft::clearSubscribtion ( tengu::XPlaneChannel* channel ) {
    
    __mutex.lock();
    channel->__subscribed = false;
    channel->__deferredSubscribe = false;
    channel->redisControlChannel = QString("");
    channel->redisConditionChannel = QString("");
    __mutex.unlock();
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                        Defer subscribtion to this channel because redis was not connected.                       *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                         Отложить подписку на этот канал, потому что редис не был соединен.                       *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractAircraft::setDeferredSubscribe ( tengu::XPlaneChannel* channel ) {
    
    __mutex.lock();
    channel->__deferredSubscribe = true;
    __mutex.unlock();
    
}


// ********************************************************************************************************************
// *                                                                                                                  *
// *                                             Get local aircraft's position.                                       *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                          Получить локальную позицию самолета.                                    *
// *                                                                                                                  *
// ********************************************************************************************************************
/*
QVector3D tengu::AbstractAircraft::getLocalPosition() {
    QVector3D result( get_f("Local_X"), get_f("Local_Y"), get_f("Local_Z") );
    return result;
}
*/

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                 Publish value for the cannel specified by his name.                              *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                       Публикация значения канала по его имени.                                   *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractAircraft::publish ( LoRedis * redis, QString channelName, QString value ) {
    
    if ( redis ) {
        XPlaneChannel * ch = getChannel( channelName );
        if ( ch ) ch->publish( redis, value );
    };
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                 Publish movement's parameter of this aircraft.                                   *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                Публикация параметров движения данного самолета.                                  *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractAircraft::publishMovement ( LoRedis * redis ) {
    
    // --------------------------------------------------------------------------------------------
    // Global aircraft's position (WGS-84)
    // Глобальная позиция самолета (WGS-84)
    // --------------------------------------------------------------------------------------------
    
    double x = get_d( QString( "Local_X" ) );
    double y = get_d( QString( "Local_Y" ) );
    double z = get_d( QString( "Local_Z" ) );
        
    // Transfer aircraft's position to global coordinates.
    // Перевод позиции самолета в глобальные координаты.
    
    double lat = 0.0,  lon = 0.0, alt = 0.0;
    XPLMLocalToWorld( x, y, z, & lat, & lon, & alt );
        
    publish( redis, QString("Latitude"),  QString::number( lat, 'f', 9 ) );
    publish( redis, QString("Longitude"), QString::number( lon, 'f', 9 ) );
    
    // for the altitude ( which is in meters) the 3 digits of precision is enought.
    // Для высоы ( она - в метрах ) 3 цифры после запятой вполне достаточная точностью.
    
    publish( redis, QString("Altitude"), QString::number( alt, 'f', 3 ) );    
    
    // Speeds - ground and instrumental.
    // Скорости - относительно земли и инструментальная.
    float gs = get_f( QString("GroundSpeed") );    
    publish( redis, QString("GroundSpeed"), QString::number( gs, 'f', 6 ) ) ;
    
    float ias = get_f( QString( "IAS" ) );
    publish( redis, QString("IAS"), QString::number( ias, 'f', 6 ) );
    
    // Position of this aircraft in the space
    // Положение самолета в пространстве.
    
    float header = get_f( QString("Heading") );
    publish( redis, QString("Heading"), QString::number( header, 'f', 5 ) );
    
    float pitch = get_f( QString("Pitch") );
    publish( redis, QString("Pitch"), QString::number( pitch, 'f', 5 ) );
    
    float roll = get_f( QString("Roll") );
    publish( redis, QString("Roll"), QString::number( roll, 'f', 5 ) );
        
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                   Set (append or replace) control channel.                                       *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                             Установить (добавить или заменить) канал управления.                                 *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractAircraft::setChannel( tengu::XPlaneChannel * ch) {
    
    _channels[ ch->getName() ] = ch;

}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                Set float value to the specific channel if exists.                                *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                          Установить вещественное значение в указанный канал, если он есть.                       *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractAircraft::set ( QString channelName, float value ) {
    
    XPlaneChannel * ch = getChannel( channelName );
    if ( ( ch ) && ( ch->getName() == channelName ) && ( ch->usable() ) ) ch->set( value );
    
}


// ********************************************************************************************************************
// *                                                                                                                  *
// *                    Return the channel which has been subscribed to this redis message's channel.                 *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                          Вернуть канал, который имеет подписку на данный канал в редисе.                         *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::XPlaneChannel * tengu::AbstractAircraft::getChannelSubscribedTo ( QString redisChannel ) {
    
    foreach ( XPlaneChannel * channel, _channels ) {
        
        if ( channel->redisControlChannel == redisChannel ) {            
            return ( channel );
        };
        
    };
    return nullptr;
    
}


// ********************************************************************************************************************
// *                                                                                                                  *
// *                                 Get all channels which have deferred subscribtion                                *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                               Вернуть все каналы, которые имеют отложенную подписку.                             *
// *                                                                                                                  *
// ********************************************************************************************************************

QVector<tengu::XPlaneChannel * > tengu::AbstractAircraft::getDeferredSubscribtion() {
    
    QVector< XPlaneChannel * > result;
    
    foreach( XPlaneChannel * channel, _channels ) {
        if ( channel->__deferredSubscribe ) result.append( channel );
    }
    
    return result;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                      Set "subscribed" flat to these channels                                     *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                  Установить флаг "подписались" на данные каналы.                                 *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractAircraft::hasBeenSubscribed(QStringList channels) {

    for ( int i=0; i<channels.size(); i++ ) {
        
        if ( _channels.contains( channels[i] ) ) {
            
            __mutex.lock();
            _channels[ channels[i] ]->__subscribed = true;
            _channels[ channels[i] ]->__deferredSubscribe = false;
            __mutex.unlock();
            
        }
    }
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                      Unsubscribe from all redis'es channels.                                     *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                         Отписаться от всех каналов редиса.                                       *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractAircraft::clearSubscribtion() {
    
        
    foreach( XPlaneChannel * channel, _channels ) {
        
        if ( channel->redisControlChannel.length() > 0 ) {
            // xplugin__log( QString("AbstractAircraft::unsubscribe(") + channel->redisControlChannel + ")" );
            // redis->unsubscribe( channel->redisControlChannel  );
            channel->redisControlChannel = "";
        }
            
        clearSubscribtion( channel );            
    };
    
}


// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                  The desctructor.                                                *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::AbstractAircraft::~AbstractAircraft() {

}

