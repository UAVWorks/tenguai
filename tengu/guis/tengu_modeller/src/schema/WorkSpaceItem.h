// ********************************************************************************************************************
// *                                                                                                                  *
// *                                       Graphical representation of entire workspace                               *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                   Графическое представление рабочего пространства целиком.                       *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 05 jul 2017 at 12:14 *
// ********************************************************************************************************************

#pragma once

#include "AbstractEntityItem.h"

namespace tengu {
    
    class WorkSpaceItem : public AbstractEntityItem {
        
        Q_OBJECT
        
        public:
            WorkSpaceItem ( AbstractEntity * entity = nullptr, QGraphicsItem * parent = nullptr );
            virtual ~WorkSpaceItem();
            
    };
    
};

