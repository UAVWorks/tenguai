// ********************************************************************************************************************
// *                                                                                                                  *
// *                                The graphics item which can change executable mode.                               *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                            Графический элемент, который может менять режим выполнения.                           *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 05 jul 2017 at 21:21 *
// ********************************************************************************************************************

#pragma once

#include "AbstractEntityItem.h"

namespace tengu {
    
    class EMChangableItem : public AbstractEntityItem {
        
        Q_OBJECT
        
        public:
        
            EMChangableItem ( AbstractEntity* entity = nullptr, QGraphicsItem* parent = nullptr );
            virtual ~EMChangableItem();
            virtual QList< QList<AgentPropertyElement> > properties();
    };
};