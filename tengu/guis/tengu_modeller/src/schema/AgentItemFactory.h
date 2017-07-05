// ********************************************************************************************************************
// *                                                                                                                  *
// *                             Agent item factory, the descedant from AgentFactory.                                 *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                        Фабрика графических элементов агентов, потомок от AgentFactory.                           *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 17 jun 2017 at 13:48 *
// ********************************************************************************************************************

#pragma once

#include <QJsonObject>
#include "AgentFactory.h"

#include "XPlaneAgentItem.h"
#include "XPlaneSimulatorItem.h"
#include "ProcessStartItem.h"
#include "ProcessStopItem.h"
#include "TaskItem.h"
#include "ProcessItem.h"
#include "ANDorItem.h"
#include "ORerItem.h"
#include "LinkItem.h"
#include "SproutItem.h"
#include "VehicleItem.h"
#include "WorkSpace.h"
#include "WorkSpaceItem.h"

namespace tengu {
    
    class AgentItemFactory : public AgentFactory {
        
        public:
            
            static AbstractEntity * createEntity( QJsonObject json );
            static AbstractEntityItem * createItem( AbstractAgentKernel * agent );
            
    };
    
};

