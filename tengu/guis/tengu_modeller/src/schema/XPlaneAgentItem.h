// ********************************************************************************************************************
// *                                                                                                                  *
// *                                Graphics representation of X-Plane as the agent.                                  *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                 Графическое представление X-Plane как агента.                                    *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 12 jun 2017 at 08:58 *
// ********************************************************************************************************************

#pragma once

#include "AbstractEntityItem.h"
#include "XPlaneAgent.h"

namespace tengu {
    
    class XPlaneAgentItem : public AbstractEntityItem {
        
        Q_OBJECT
        
        public:
            
            XPlaneAgentItem ( XPlaneAgent * entity, QGraphicsItem* parent = nullptr );
            virtual ~XPlaneAgentItem();
            void checkEntity();
            
        private:
    };
    
};

