// ********************************************************************************************************************
// *                                                                                                                  *
// *                                             The main class of X-Tengu.                                           *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                          Главный класс плагина X-Tengu.                                          *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 03 may 2017 at 09:43 *
// ********************************************************************************************************************

#pragma once

// System includes
// Системные инклудники
#include <stdio.h>
#include <time.h>

#include <string>

// Qt includes
// Инклудники Qt

#include <QObject>
#include <QSettings>
#include <QList>
#include <QString>
#include <QStringList>
#include <QMutex>

// X-Plane plugin SDK includes
// Инклудники X-Plane plugin SDK

#include "XPLMDataAccess.h"
#include "XPLMDisplay.h"
#include "XPLMProcessing.h"
#include "XPLMScenery.h"

extern void xplugin__log( QString msg );


// My include files.
// Мои инклудники.

#include "LoRedis.h"

#include "LoRedisReactor.h"
#include "AbstractAircraft.h"
#include "UserAircraft.h"
#include "EnvironmentAircraft.h"
#include "XPlaneOverrided.h"

// To avoid difference in config file name
// Чтобы избежать различия в имени конфигурационного файла.

#define CREATE_XTENGU_SETTINGS QSettings settings( "/etc/tengu/xtengu.conf", QSettings::IniFormat )

// Total aircrafts in X-Plane, user's and environment together.
// Всего самолетов в X-Plane, пользовательский и окружающих вместе.

#define TOTAL_AIRCRAFTS_COUNT 20

// The name of log file for the plugin.
// Имя файла журнала для плагина.

#define XPLUGIN_LOG_FILE "Resources/plugins/xtengu/xtengu_log.txt"

namespace tengu {
    
    class XTengu  : public LoRedisReactor {
        
        // "The Slots". But they are not from Qt, they are from libevent re-actor. Although the meaning remains 
        // the same. The idea of running an instance of QApplication inside the plugin looks somewhat absurd, 
        // so we manage a lower level.
        
        // "Слоты". Но не Qt, а реакции libevent. Хотя смысл остается тот же самый. Мысль о запуске внутри плагина 
        // экземпляра QApplication выглядит несколько абсурдно, поэтому обходимся более низким уровнем.
        
        public:
            
            void cbConnected( LoRedis * redis );
            void cbDisconnected( LoRedis * redis );
            void cbError( LoRedis * redis, QString message );
            void cbSubscribed( LoRedis * redis, QString channel );
            void cbUnsubscribed( LoRedis * redis, QString channel );
            void cbGotMessage( LoRedis * redis, QString channel, QString message );
            void cbSetted( LoRedis * redis, int status );
            void cbGotValue( LoRedis * redis, QString name, QVariant value );
            
            // Callback-procedure for observation aircrafts movement (position, speed, e.t.c.)
            // Callback-проецдура для наблюдения за движением самолетов (позицией, скоростью и др.)
            
            float cbObservingMovement();
            
            // Message about the aircraft was loaded (or re-loaded)
            // Сообщение о том, что был загружен (пере-загружен) самолет.
            
            void msg__AircraftLoaded( int index );
            
            void msg__AirplaneCountChanged();
            void msg__AirportLoaded();
            
        public:
            
            XTengu();
            virtual ~XTengu();
            void enable();
            void disable();
                        
        private:
            
            /**
             * @short Enabling of plugin running.
             */
            
            bool __enabled;
            
            /**
             * @short Redis "ordinary" connector.
             */
            
            LoRedis * __redis;
            
            /**
             * @short Is redis connected?
             */
            
            bool __redis__connected;
            
            /**
             * @short Redis subscribe connector.
             * 
             * This is an separate object because redis does not allow to do anything together with subscribing.
             * Отдельный объект, потому что редис не позволяет что-нибудь еще делать вместе с подпиской.
             */
            
            LoRedis * __sub_redis;
            
            /**
             * @short Is SubRedis connected?
             */
            
            bool __sub_redis__connected;
            
            // void __xplane_control_off();
            // void __xplane_control_on();
            
            // XPlaneOverrided * __xp_control;
                        
            QString __controlNameGroup;
            QString __conditionNameGroup;
            
            // Array of aircrafts. Index 0 mean user (basic) aircraft. The other indices are environment airplanes.
            // Массив самолетов. Индекс 0 - это пользовательский (основной) самолет, остальные - это самолеты окружения.
            QList <AbstractAircraft * > __acfs;
                        
            // Read config file and subscribe to control channels.
            // Читаем конфигурационный файл, подписываемся на каналы управления.
            
            void __init_settings();
            
            // Unsubscribe all subscribed redis channels, set subscribed channel values to empty string.
            // This is backward matter of __init_settings()
            
            // Отписаться от всех подписанных каналов редиса. Установить значение подписанных каналов в пустые строки.
            // Обратное действие от __init_settings()
            
            void __unsibscribe();
            
            // Take settings for one channel, memorize them and subscribe to receive messages for this one channel.
            // Взять установки для одного канала, запомнить их и подписаться на получение сообщений для этого канала.
            
            void __take_settings_and_subscribe( QString section, QString inputDefault = QString(""), QString outputDefault = QString("") );
            
            // The frequency ( times per second ) of call observing aircrafts state (position, location, condition e.t.c.) callback.
            
            // Частота (раз в секунду) вызова callback-процедуры наблюдения за состоянием самолетов (позицией, скоростью, положением 
            // органов управления и др).
            
            float __observingAircraftsFrequency;
            
            int __totalAircrafts, __activeAircrafts;
            
            bool __in_observation;
                        
    }; // end of class
    
}; // namespace tengu
