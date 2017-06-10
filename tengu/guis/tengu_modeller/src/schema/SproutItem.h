// ********************************************************************************************************************
// *                                                                                                                  *
// *     Graphics representation of Sprout class (signal/slot mechanism, inter-agent redis-based communications).     *
// * ---------------------------------------------------------------------------------------------------------------- *
// *          Графическое представление класса Sprout ("росток' - механизм сигналов-слотов, межагентное общение,      *
// *                                                  основанное на redis'е.                                          *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 10 jun 2017 at 12:20 *
// ********************************************************************************************************************

#pragma once

#include "AbstractAgentItem.h"
#include "Sprout.h"

namespace tengu {
    
    class SproutItem : public AbstractAgentItem {
        
        Q_OBJECT
        
        public:
            
            SproutItem ( Sprout * sprout, QGraphicsItem * parent = nullptr );
            virtual ~SproutItem();
            
            void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = Q_NULLPTR);
            
        private:
    };
    
};

