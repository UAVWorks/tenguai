// ********************************************************************************************************************
// *                                                                                                                  *
// *                               The graphical representation for logical AND branching.                            *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                 Графическое представление ветвления с логическим И.                              *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 18 jun 2017 at 09:29 *
// ********************************************************************************************************************

#pragma once

#include "ANDor.h"
#include "AbstractEntityItem.h"

namespace tengu {
    
    class ANDorItem : public AbstractEntityItem {
        Q_OBJECT
        public:
            
            ANDorItem ( ANDor * entity = nullptr, QGraphicsItem* parent = nullptr );
            virtual ~ANDorItem();
            
            void checkEntity();
            void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = Q_NULLPTR);
            
        protected:
        private:
    };
    
};

