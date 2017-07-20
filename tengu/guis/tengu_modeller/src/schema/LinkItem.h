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
        friend class ResizeableItemWithLinks;
        
        Q_OBJECT
        
        signals:
            
            void signalLinkRemoved( QString uuid );
            
        public:
            
            explicit LinkItem ();
            virtual ~LinkItem();
            
            void checkEntity();
            
            virtual QString getSystemName();
            virtual void setSystemName( QString name );
            virtual QString getHumanName();
            virtual void setHumanName( QString name );
            virtual QString getComment();
            virtual void setComment( QString comment );
            virtual AbstractEntity::execution_mode_t getExecutionMode();
            virtual void setExecutionMode( AbstractEntity::execution_mode_t mode );
                        
            void setTo( QPoint to );
            
            AbstractEntityItem * getFrom();
            AbstractEntityItem * getTo();
            
            bool semiCreated();
            bool isEmpty();
            void recalculate();
            float distanceTo( QPointF point );
            
            bool withSproutFrom();
            bool withSproutTo();
                        
        protected:
            
            void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = Q_NULLPTR);
            
        private:
            
            AbstractEntityItem * __from;
            AbstractEntityItem * __to;
            
            QString __system_name;
            QString __human_name;
            QString __comment;
            AbstractEntity::execution_mode_t __execution_mode;
            
            void __setFrom( AbstractEntityItem * entity );
            void __setTo( AbstractEntityItem * entity );     
            
            void __correctPointsForSprouts( int x, QRect fromRect, QRect toRect );
            
            QPoint __posFrom;
            QPoint __posTo;
            
            QPoint __tempTo;
            
            // bool __withSproutFrom;
            // bool __withSproutTo;
            
            void __checkSproutInsideTask();
            
    };
    
};

