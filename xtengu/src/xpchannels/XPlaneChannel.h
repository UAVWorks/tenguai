// ********************************************************************************************************************
// *                                                                                                                  *
// *  One channel for control or surveillance condition of one aircraft's item (one element: aileron, speed e.t.c).   *
// * ---------------------------------------------------------------------------------------------------------------- *
// *     Один канал для управления или наблюдения состояния за одним элементом самолета (элерон, скорость и др.)      *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 06 may 2017 at 11:09 *
// ********************************************************************************************************************

#pragma once

#include <math.h>
#include <string>

#include <QString>

#include "LoRedis.h"
#include "XPLMDataAccess.h"


#define MINIMUM_CONSTRAINT -100500
#define MAXIMUM_CONSTRAINT 100500

// The number of all possible engines.
// Число всех возможных двигателей

#define XPLANE_ENGINES_COUNT 8

// The number of possible X-Plane control surfaces
// Число возможных поверхностей управления в X-Plane

#define XPLANE_SURFACES_COUNT 56

extern void xplugin__log( QString message );


namespace tengu {
    
    class XPlaneChannel {
        
        friend class AbstractAircraft;
        
        public:
            
            XPlaneChannel();
            XPlaneChannel( QString aName );
            XPlaneChannel( QString aName, QString path, int aMinValue = MINIMUM_CONSTRAINT, int aMaxValue = MAXIMUM_CONSTRAINT );
            virtual ~XPlaneChannel();
            
            static XPLMDataRef find_dataref( QString path );
            
            // Set x-plane dataref by path.
            // Установить ссылку на переменную X-Plane из ее имени.
            
            void setDataRef( QString path );
            
            virtual bool usable();
                        
            QString redisControlChannel;
            QString redisConditionChannel;
                        
            QString getName();
            
            int minValue, maxValue;
            
            virtual void set( int value );                        
            virtual void set( float value );
            virtual void set( QString message );
            
            float get_f();
            double get_d();
            
            
            // Publish value for this X-Plane channel. "The publis" is publish message and
            // simultaneously set data value in the Redis.
            
            // Опубликовать значение для этого канала X-Plane. "Опубликовать" - это не только
            // публикация сообщения в редисе, но и установка соответствующей переменной.
            
            virtual void publish( LoRedis * redis, QString value );
            
            // int get();

        protected:
            
            XPLMDataRef _dataref;
            float applyRestrictions( float value );
            int applyRestrictions( int value );
            
        private:
            
            // Channel name and section name in settings ( in config file ) are the same.
            // Имя канала и имя секции установок (конфигурационного файла) - это одно и то же.
            QString __name;
            
            bool __subscribed;
            bool __deferredSubscribe;

            
    };
    
}; // namespace
