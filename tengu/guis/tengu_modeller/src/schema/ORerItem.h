// ********************************************************************************************************************
// *                                                                                                                  *
// *                                  The graphical representation for logical OR branching.                          *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                  Графическое представление ветвления по логическому ИЛИ.                         *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 18 jun 2017 at 09:37 *
// ********************************************************************************************************************

#pragma once

#include "ORer.h"
#include "AbstractEntityItem.h"

namespace tengu {
    class ORerItem : public AbstractEntityItem {
        
        Q_OBJECT
        
        public:
            
            ORerItem ( ORer * entity = nullptr, QGraphicsItem * parent = nullptr );
            virtual ~ORerItem();
            
            void checkEntity();
            void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = Q_NULLPTR);
            
        protected:
        private:
    };
};

