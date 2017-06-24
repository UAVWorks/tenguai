// ********************************************************************************************************************
// *                                                                                                                  *
// *                                   The graphics representation of top-level entity                                *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                  Графическое представление сущности верхнего уровня.                             *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 24 jun 2017 at 16:12 *
// ********************************************************************************************************************

#pragma once

#include "AbstractEntityItem.h"

namespace tengu {
    
    class TopLevelIem : public AbstractEntityItem {
        
        Q_OBJECT
        
        public:
            
            TopLevelIem ( AbstractEntity * entity = nullptr, QGraphicsItem* parent = nullptr );
            virtual ~TopLevelIem();
    };
    
};

