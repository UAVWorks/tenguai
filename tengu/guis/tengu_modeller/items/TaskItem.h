// ********************************************************************************************************************
// *                                                                                                                  *
// *                               The graphics representation of tengu.core.Task class                               *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                 Графическое представление класса tengu.core.Task                                 *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 09 jun 2017 at 10:56 *
// ********************************************************************************************************************

#pragma once

#include "AbstractAgentItem.h"
#include "Task.h"

namespace tengu {
    
    class TaskItem : public AbstractAgentItem {
    
        Q_OBJECT
        
        public:
            
            TaskItem ( Task * task, QGraphicsItem * parent = Q_NULLPTR );
            virtual ~TaskItem();
            
            void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = Q_NULLPTR);
        
        private:
            
            Task * __task();            
            // QRectF boundingRect() const;
            
    };
    
};

