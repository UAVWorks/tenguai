// ********************************************************************************************************************
// *                                                                                                                  *
// *                   External settings for X-Plane plugin to use it together with another software                  *
// * ---------------------------------------------------------------------------------------------------------------- *
// *      Внешние установки плагина X-Plane для его совместного использования с другим программным обеспечением.      *
// *                                                                                                                  *
// ********************************************************************************************************************

#pragma once

// Last external programs activity time (the path in in the redis) to take a control for user aircraft.
// Последнее время активности внешних программ (путь в редисе) для перехвата управления пользовательским самолетом.

#define AIRCRAFT_CONTROL_PATH                   "xtengu.control_presence."
#define USER_AIRCRAFT_CONTROL_PING_PATH         AIRCRAFT_CONTROL_PATH"0"

#define XTENGU_PRESENCE_PATH                    "xtengu.presence"

// To avoid difference in config file name
// Чтобы избежать различия в имени конфигурационного файла.

#define CREATE_XTENGU_SETTINGS QSettings settings( "/etc/tenguai/xtengu.conf", QSettings::IniFormat )
