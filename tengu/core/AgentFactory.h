// ********************************************************************************************************************
// *                                                                                                                  *
// *                                        Agent factory, agent's classes recognizer.                                *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                   "Фабрика агентов", распознавание классов агентов.                              *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 16 jun 2017 at 09:40 *
// ********************************************************************************************************************

#pragma once

#include <QJsonObject>
#include <QDebug>

#include "Constants.h"
#include "ProcessStart.h"
#include "ProcessStop.h"
#include "Process.h"
#include "Vehicle.h"
#include "Task.h"
#include "ANDor.h"
#include "ORer.h"

namespace tengu {
    
    class AgentFactory {
        
        public:
            // static bool hasAgent( QJsonObject json );
            static AbstractEntity * createEntity( QJsonObject json );
            static QString getClassName( QString collectionName );
        
        protected:
            static void _readFromJSON( AbstractEntity * entity, QJsonObject json );
            static void _append_object( AbstractEntity * entity, QString key, QJsonObject json );
        
    };
    
};

