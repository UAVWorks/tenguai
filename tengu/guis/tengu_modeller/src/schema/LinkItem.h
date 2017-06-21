// ********************************************************************************************************************
// *                                                                                                                  *
// *                                  The link representation. Arrows between agents.                                 *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                              Графическое представление стрелок (связей между агентами).                          *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 18 jun 2017 at 13:16 *
// ********************************************************************************************************************

#pragma once

#include "AbstractEntityItem.h"
#include "SproutItem.h"
#include "Constants.h"

namespace tengu {
    
    class LinkItem : public AbstractEntityItem {
        
        Q_OBJECT
        public:
            
            explicit LinkItem ();
            virtual ~LinkItem();
            
            void checkEntity();
            
            void setFrom( AbstractEntityItem * entity );
            void setTo( AbstractEntityItem * entity );
            void setTo( QPoint to );
            bool semiCreated();
            bool isEmpty();
            void recalculate();
            
            
        protected:
            
            void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = Q_NULLPTR);
            
        private:
            
            AbstractEntityItem * __from;
            AbstractEntityItem * __to;
            
            QPoint __posFrom;
            QPoint __posTo;
            
            QPoint __tempTo;
            
            bool __withSprout;
            
    };
    
};

