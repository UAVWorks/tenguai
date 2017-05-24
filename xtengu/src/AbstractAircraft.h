// ********************************************************************************************************************
// *                                                                                                                  *
// *                                 An abstract aircraft, the ancestor of all real aircrafts.                        *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                   Абстрактный самолет, предок всех реальных самолетов.                           *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 03 may 2017 at 10:32 *
// ********************************************************************************************************************

#pragma once

#include <stdio.h>

#include <QString>
#include <QStringList>
#include <QMap>
#include <QVector>
#include <QMutex>
// #include <QVector3D>

#include "XPLMDefs.h"
#include "XPLMDataAccess.h"
#include "XPLMPlanes.h"
#include "XPLMGraphics.h"

#include "XPlaneChannel.h"
#include "LoRedis.h"

extern void xplugin__log( QString msg );

namespace tengu {
    
    class AbstractAircraft {
        
        public:
            
            AbstractAircraft();
            virtual ~AbstractAircraft();
            
            // Return control channel by name.
            // Вернуть канал управления по его имени.
            
            XPlaneChannel * getChannel( QString channelName );
                        
            // Set (append or replace) control cannel.
            // Установить (добавить или заменить) канал управления.
            
            void setChannel( XPlaneChannel * ch );
            
            // Set float value to specific channel if exists.
            // Установить вещественное значение указанного канала, если он есть.
            
            void set( QString channelName, float value );
            
            // Get channel which subscribed to messages from this redis'es channel.
            // Вернуть канал, который подписался на данный канал в редисе.
            
            XPlaneChannel * getChannelSubscribedTo( QString redisChannel );
            
            // Get all channel which have deferred subscribtion
            // Вернуть все каналы, которые имеют отложенную подписку.
            
            QVector< XPlaneChannel * > getDeferredSubscribtion();
            
            // Defer subscribtion to this channel
            // Отложить подписку на этот канал.
            
            void setDeferredSubscribe( XPlaneChannel * channel );
            
            // Clear all subscribtion attribute.
            // Очистить все атрибуты подписки.
            
            void clearSubscribtion( XPlaneChannel * channel );
            
            // Set "subscribed" flag to these channels.
            // Установить флаг "подписались" на данные каналы.
            
            void hasBeenSubscribed( QStringList channelNames );
            
            // Unsubscribe all channels.
            // Отписаться от всех каналов.
            
            void clearSubscribtion();
            
            // publish value for the channel specifies by his name.
            // Опубликовать значение в канал по его имени.
            
            void publish( LoRedis * redis, QString channelName, QString value );
            
            // publish movement information for this aircraft.
            // публикация информации о движении этого самолета.
            
            virtual void publishMovement( LoRedis * redis );
            
            // Get from X-Plane physical model and path for specific aircraft.
            // Получить из X-Plane физическую модель для данного индекса самолета.
            
            static void getXPlaneModel( int index, QString & file, QString & path );
            
            // Get total number aircrafts in the X-Plane.
            // Получить общее число самолетов в X-Plane.
            
            static void getXPAircraftsNumber( int * total, int * active );
                                    
            // Get float value for channel identified by his name
            // Получить вещественное значение канала по его имени.
            
            float get_f( QString xpChannelName );
            
            // Get double value for channel idetified by his name
            // Получить значение двойной точности для канала по его имени.
            
            double get_d( QString xpChannelName );
            
            
        protected:
            
            // Collection of this aircraft's "control channels".
            // I need a pointers because I use polymorphyzm (virtual procedures)
            
            // Коллекция "каналов управления" данного самолета. 
            // Мне нужны именно указатели, т.к. я использую полиморфизм (виртуальные процедуры).
            
            QMap< QString, XPlaneChannel * > _channels;
            
            QMutex __mutex;
            
    };
    
}; // namespace tengu
