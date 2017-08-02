// ********************************************************************************************************************
// *                                                                                                                  *
// * The realization of call-back X-Plane plugin functions, the transition from X-Plane to the structure of classes.  *
// * ---------------------------------------------------------------------------------------------------------------- *
// *              Реализация callback-процедур плагина X-Plane, переход между X-Plane и структурой классов.           *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       created 29 apr 2017 at 13:14 *
// ********************************************************************************************************************

#include <string.h>
#include <time.h>
#include <string>

#include "XPLMDefs.h"
#include "XPLMPlanes.h"
#include "XPLMPlugin.h"
#include "XPLMProcessing.h"
#include "XPLMScenery.h"

#include "XTengu.h"

// Pointer to XTengu main class.
// Указатель на главный класс XTengu.

tengu::XTengu * xtengu = nullptr;

// ********************************************************************************************************************
// *                                                                                                                  *
// *              X-Plane called callback procedure to observing aircraft's movements (position, speed...).           *
// * ---------------------------------------------------------------------------------------------------------------- *
// *    Callback-процедура, вызывающаяся из X-Plane с целью отслеживания движения самолетов (положения, скорости...)  *
// *                                                                                                                  *
// ********************************************************************************************************************

float __cb__xplane__observing_movements( float elapsedSinceLastCall, float elapsedTimeSinceLastFlightLoop, int counter, void * refcon ) {

    Q_UNUSED( elapsedSinceLastCall );
    Q_UNUSED( elapsedTimeSinceLastFlightLoop );
    Q_UNUSED( counter );
    Q_UNUSED( refcon );
    
    if ( xtengu ) return ( xtengu->cbObservingMovement() );
    
    // If this procedure called without pointer to the XTengu class, the next call occure throught half of second.
    // Если процедура была вызвана в отсутствии указателя на класс XTengu, то следующий вызов будет через пол-секунды.
    
    return (float) 0.5;
};

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                 The start of plugin.                                             *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                    Старт плагина.                                                *
// *                                                                                                                  *
// ********************************************************************************************************************

PLUGIN_API int XPluginStart( char * outName, char * outSig, char * outDesc ) {
    
    //
    // Filling information's parameter from this plugin - for simulator.
    //
    // Заполнение параметров информации о плагине - для симулятора.
    //
    
    strcpy( outName, "X-Tengu" );
    strcpy( outSig, "tengu.xplane.plugin" );
    strcpy( outDesc, "The Tengu plugin for X-Imitation system." );
    
    xtengu = new tengu::XTengu();

    return 1;
};

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                             Out text message to log file.                                        *
// *                                                                                                                  *
// *                                       Вывод текстового сообщения в файл журнала.                                 *
// *                                                                                                                  *
// ********************************************************************************************************************

void xplugin__log( QString message ) {

    FILE * fd = fopen( XPLUGIN_LOG_FILE, "a" );
    if ( fd ) {
        time_t now;
        // char c_time[128];
        time( & now );
        struct tm * now_tm = localtime( &now );
        fprintf( fd, "%4d-%02d-%02d %02d:%02d:%02d  %s\n", 
                now_tm->tm_year + 1900, now_tm->tm_mon + 1, now_tm->tm_mday, 
                now_tm->tm_hour, now_tm->tm_min, now_tm->tm_sec, message.toLocal8Bit().data()
        );
        fclose( fd );
    };    
};


// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                 The stop of plugin.                                              *
// *                                                                                                                  *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                                                                                  *
// *                                                  Остановка плагина.                                              *
// *                                                                                                                  *
// ********************************************************************************************************************

PLUGIN_API void XPluginStop(void) {
    
    if ( xtengu ) {
        try {
            delete ( xtengu );
        } catch ( ... ) {
            // @todo make error handler without any output streams.
        };
        xtengu = nullptr;
    };

}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                              The enabling of plugin running.                                     *
// *                                                                                                                  *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                                                                                  *
// *                                               Разрешение выполнения плагина.                                     *
// *                                                                                                                  *
// ********************************************************************************************************************

PLUGIN_API int XPluginEnable(void) {
    
    xtengu->enable();
    
    // Register callback procedures.
    // Регистрация callback-процедур.
    
    XPLMRegisterFlightLoopCallback( __cb__xplane__observing_movements, 0.5, NULL );
    
    return 1;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                              The disabling of plugin running.                                    *
// *                                                                                                                  *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                                                                                  *
// *                                              Запрещение выполнения плагина.                                      *
// *                                                                                                                  *
// ********************************************************************************************************************

PLUGIN_API void XPluginDisable(void) {
    
    // Unregister callback procedures
    // Отмена регистрации callback-процедур.
    
    XPLMUnregisterFlightLoopCallback( __cb__xplane__observing_movements, NULL );
    
    // Disabling the plugin activity
    // Запрещение активности плагина.
    
    xtengu->disable();
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                               Got some message from the X-Plane (or from another his plugin).                    *
// *                                                                                                                  *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                                                                                  *
// *                          От X-Plane (или от другого его плагина) было получено некое сообщение.                  *
// *                                                                                                                  *
// ********************************************************************************************************************

PLUGIN_API void XPluginReceiveMessage( XPLMPluginID inFromWho, long inMessage, void * inParam) {

    // 
    // There is a parameter for a lot of procedures, it is a number, not a pointer.
    // This is uncomfortable to convert it every each time. Therefore i make a conversion here.
    //
    // В куче процедур в качестве параметра идет - не указатель, а целое число.
    // Неудобно его каждый раз преобразовывать. Поэтому преобразование делаю - здесь.
    //

    size_t parameter = ( size_t ) inParam;

    switch ( inFromWho ) {

        case XPLM_PLUGIN_XPLANE : {

            // ------------------------------------------------------------------------------------
            //                    The message was received from the X-Plane itself. 
            //
            //                     Сообщение в плагин поступило - от X-Plane.
            // ------------------------------------------------------------------------------------

            switch ( inMessage ) {

                case XPLM_MSG_PLANE_LOADED : {

                    //
                    // An aircraft was loaded. The parameter is a index of loaded airplane.
                    //
                    // Был загружен самолет. Параметром идет номер загруженного самолета.
                    //
                    
                    if ( xtengu ) xtengu->msg__AircraftLoaded( parameter );
                    return;
                }; break;

                case XPLM_MSG_AIRPLANE_COUNT_CHANGED : {
                    //
                    // The amount of available aircrafts has been changed in the X-Plane.
                    //
                    // В X-Plane было изменено количество доступных самолетов.
                    
                    if ( xtengu ) xtengu->msg__AirplaneCountChanged();
                    return;
                }; break;

                case XPLM_MSG_PLANE_CRASHED : {
                    // msg__airplane_crashed( parameter );
                    return;
                }; break;

                case XPLM_MSG_PLANE_UNLOADED : {
                    //
                    // An aircraft was unloaded from X-Plane.
                    //
                    // Самолет был выгружен из X-Plane.
                    //
                    // msg__aircraft_unloaded( parameter );
                    return;
                }; break;

                case XPLM_MSG_AIRPORT_LOADED : {
                    
                    //
                    // An airport was loaded. This message does not appear in the time of game,
                    // it sends only when user change an aicraft or an airport by hands.
                    // 
                    // Был загружен аэропорт. Сообщение не возникает в процессе игры, оно появляется
                    // только тогда, когда пользователь вручную устанавливает самолет в X-Plane
                    //
                    
                    if ( xtengu ) xtengu->msg__AirportLoaded();
                    return;
                    
                }; break;

                case XPLM_MSG_SCENERY_LOADED : {
                    // msg__scenery_loaded();
                    return;
                }; break;

                case XPLM_MSG_LIVERY_LOADED : {
                    // msg__livery_loaded( parameter );
                    return;

                }; break;

                case XPLM_MSG_WILL_WRITE_PREFS : {
                    //
                    // X-Plane gonna to write himself settings and ask us to participate.
                    //
                    // X-Plane собрался записывать установки и дает возможность плагину поучаствовать
                    // в этом процессе.
                    //
                    // msg__will_write_prefs();
                    return;
                }; break;

                default: {
                    // char out[256];
                    // sprintf( out, "There was received, but unhandled a message %d from X-Plane.", parameter );
                    // xplugin__log ( out );
                };

            }; // switch inMessage (only from x-plane)

        }; break; // case messaage from xplane

        default : {
            // char out[256];
            // sprintf( out, "There was received message from another plugin. PluginID=%d, msgID=%d, param=%d.", inFromWho, inMessage, parameter );
            // xplugin__log ( out );
        };

    }; // end of switch inFromWho
}


