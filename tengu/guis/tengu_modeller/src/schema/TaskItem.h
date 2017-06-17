// ********************************************************************************************************************
// *                                                                                                                  *
// *                               The graphics representation of tengu.core.Task class                               *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                 Графическое представление класса tengu.core.Task                                 *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 09 jun 2017 at 10:56 *
// ********************************************************************************************************************

#pragma once

#include "AbstractEntityItem.h"
#include "Task.h"

namespace tengu {
    
    class TaskItem : public AbstractEntityItem {
    
        Q_OBJECT
        
        public:
            
            TaskItem ( Task * task = Q_NULLPTR, QGraphicsItem * parent = Q_NULLPTR );
            virtual ~TaskItem();
            
            void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = Q_NULLPTR);
            void checkEntity();
            
            // QJsonObject toJSON();
            // bool fromJSON( QJsonObject json );
            
        
        private:
            
            Task * __task();                        
            
    };
    
};

