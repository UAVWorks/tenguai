// ********************************************************************************************************************
// *                                                                                                                  *
// *                                       The graphical representation for the process.                              *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                             Графическое представление процесса.                                  *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 17 jun 2017 at 15:34 *
// ********************************************************************************************************************

#pragma once

#include "ItemWithLinks.h"

namespace tengu {
    
    class ProcessItem : public ItemWithLinks {
        
        Q_OBJECT
        
        public:
            
            ProcessItem ( AbstractEntity* entity = Q_NULLPTR, QGraphicsItem * parent = Q_NULLPTR );
            virtual ~ProcessItem();
            
            void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = Q_NULLPTR);
            
            void checkEntity();
            
    };
};

