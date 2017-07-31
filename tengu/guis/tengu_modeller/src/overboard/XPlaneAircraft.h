// ********************************************************************************************************************
// *                                                                                                                  *
// *                   The representation X-Plane simulator as the "agent" using XTengu plugin.                       *
// * ---------------------------------------------------------------------------------------------------------------- *
// *             Представление симулятора X-Plane в качестве агента с использованием плагина XTengu.                  *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 11 jun 2017 at 20:22 *
// ********************************************************************************************************************

#pragma once

#include <QSettings>
#include <QString>
#include <QDebug>

#include "Constants.h"
#include "Task.h"
#include "SproutProxy.h"
#include "XTenguSettings.h"

// @todo remake that is the same with X-Tengu.
// Переделать так, чтобы было одинаково с X-Tengu.

// #define CREATE_XTENGU_SETTINGS      QSettings settings( "/etc/tenguai/xtengu.conf", QSettings::IniFormat )
// #define TOTAL_AIRCRAFTS_COUNT 20

namespace tengu {

    class XPlaneAircraft : public Task {
        
        Q_OBJECT
        
        public:
            
            XPlaneAircraft( int acf_index, QString group, Sprout::sprout_type_t sproutsType );
            virtual ~XPlaneAircraft();
            
            void setName( QString name );
            void setExecutionMode( execution_mode_t mode );
            void connect();
            void disconnect();
            
        protected:
            
            virtual QString _ping_path();
            
        private:
            
            QString __group;
            
            int __aircraft_index;
            
            void __init_sprout( QString settingsGroup, Sprout::sprout_type_t type, QString name, float min = MINIMUM_CONSTRAINT, float max = MAXIMUM_CONSTRAINT );
            
    };
};

