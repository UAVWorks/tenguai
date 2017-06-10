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

#define ORDINAR_BORDER_COLOR            QColor(56,37,9)
#define ORDINAR_SELECTED_BORDER_COLOR   QColor(87,64,30)
#define ORDINAR_FILL_COLOR              QColor(143,107,50) 
#define ORDINAR_SELECTED_FILL_COLOR     QColor(179,146,93)

#define ACTIVE_BORDER_COLOR             QColor(77,38,0) 
#define ACTIVE_SELECTED_BORDER_COLOR    QColor(89,0,0)
#define ACTIVE_FILL_COLOR               QColor(255,126,0)
#define ACTIVE_SELECTED_FILL_COLOR      QColor(242,155,104)


#define FOCUSED_BORDER_COLOR            QColor(117,81,26)
#define FOCUSED_SELECTED_BORDER_COLOR   QColor(143,107,50)
#define FOCUSED_FILL_COLOR              QColor(179,146,93)
#define FOCUSED_SELECTED_FILL_COLOR     QColor(222,188,133)

namespace tengu {
    
    class TaskItem : public AbstractAgentItem {
    
        Q_OBJECT
        
        public:
            
            TaskItem ( Task * task, QGraphicsItem * parent = Q_NULLPTR );
            virtual ~TaskItem();
            
            void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = Q_NULLPTR);
        
        private:
            
            Task * __task();                        
            
    };
    
};

