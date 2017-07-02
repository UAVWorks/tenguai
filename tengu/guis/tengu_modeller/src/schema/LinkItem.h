// ********************************************************************************************************************
// *                                                                                                                  *
// *                                  The link representation. Arrows between agents.                                 *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                              Графическое представление стрелок (связей между агентами).                          *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 18 jun 2017 at 13:16 *
// ********************************************************************************************************************

#pragma once

#include <QPoint>
#include <QPointF>

#include "AbstractEntityItem.h"
#include "Constants.h"

namespace tengu {
    
    
    class LinkItem : public AbstractEntityItem {
    
        friend class ItemWithLinks;
        
        Q_OBJECT
        
        signals:
            
            void signalLinkRemoved( QString uuid );
            
        public:
            
            explicit LinkItem ();
            virtual ~LinkItem();
            
            void checkEntity();
            
            void setTo( QPoint to );
            
            bool semiCreated();
            bool isEmpty();
            void recalculate();
            float distanceTo( QPointF point );
            
            
        protected:
            
            void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = Q_NULLPTR);
            
        private:
            
            AbstractEntityItem * __from;
            AbstractEntityItem * __to;
            
            void __setFrom( AbstractEntityItem * entity, bool withSproutFrom );
            void __setTo( AbstractEntityItem * entity, bool withSproutTo );     
            
            void __correctPointsForSprouts( int x, QRect fromRect, QRect toRect );
            
            QPoint __posFrom;
            QPoint __posTo;
            
            QPoint __tempTo;
            
            bool __withSproutFrom;
            bool __withSproutTo;
            
            void __checkSproutInsideTask();
            
    };
    
};

