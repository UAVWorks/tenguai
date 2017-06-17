// ********************************************************************************************************************
// *                                                                                                                  *
// *                                       The graphical representation for the process.                              *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                             Графическое представление процесса.                                  *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 17 jun 2017 at 15:34 *
// ********************************************************************************************************************

#pragma once

#include "AbstractEntityItem.h"

namespace tengu {
    
    class ProcessItem : public AbstractEntityItem {
        
        Q_OBJECT
        
        public:
            
            ProcessItem ( AbstractEntity* entity = Q_NULLPTR, QGraphicsItem * parent = Q_NULLPTR );
            virtual ~ProcessItem();
            
            void checkEntity();
            
    };
};

