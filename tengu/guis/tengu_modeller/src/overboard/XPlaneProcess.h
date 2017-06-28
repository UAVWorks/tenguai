// ********************************************************************************************************************
// *                                                                                                                  *
// *                         Full-time invisible process for connection with an X-Plane simulator.                    *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                    Невидимый (постоянно невидимый) процесс для соединения с симулятором X-Plane                  *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 26 jun 2017 at 09:10 *
// ********************************************************************************************************************

#pragma once

#include <QList>
#include <QString>

#include "Process.h"
#include "XPlaneAircraft.h"

namespace tengu {
    
    class XPlaneProcess : public Process {
        
        Q_OBJECT
        
        static const QString XPLANE_PROCESS_UUID;        
        static const QList<QString> AIRCRAFTS_UUIDS;
        
        public:
            
            XPlaneProcess();
            virtual ~XPlaneProcess();
    };
    
};

