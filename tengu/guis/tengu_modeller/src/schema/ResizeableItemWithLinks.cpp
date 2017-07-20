// ********************************************************************************************************************
// *                                                                                                                  *
// *                      An item with a links which can change his size depended on linked agents.                   *
// * ---------------------------------------------------------------------------------------------------------------- *
// *           Элемент со связями, который может менять свои размеры в зависимости от связанных с ним элементов.      *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 20 jul 2017 at 17:38 *
// ********************************************************************************************************************

#include "ResizeableItemWithLinks.h"

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                 The constructor.                                                 *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                    Конструктор.                                                  *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::ResizeableItemWithLinks::ResizeableItemWithLinks ( tengu::AbstractEntity* entity, QGraphicsItem* parent ) 
    : ItemWithLinks ( entity, parent )
{

}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                              Set this element vertical size arranged to linked items.                            *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                      Установить вертикальные размеры в зависимости от связанных элементов.                       *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::ResizeableItemWithLinks::__arrange_vertically( AbstractEntityItem * item, int & minY, int & maxY  ) {
    
    
    QRectF br = item->boundingRect();
    
    int y = item->getY();    
    if ( item->entity() ) y = item->entity()->getY();
    if ( y < minY ) minY = y;    
    
    int ey = y + br.height();
    if ( ey > maxY ) maxY = ey;
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                     Calculate and return bounding rectangle.                                     *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                   Посчитать и вернуть описывающий прямоугольник.                                 *
// *                                                                                                                  *
// ********************************************************************************************************************

QRectF tengu::ResizeableItemWithLinks::boundingRect() const {
    
    if ( _entity ) {
    
        switch ( _entity->entityType() ) {
            case AbstractEntity::ET_ANDor:
            case AbstractEntity::ET_ORer:
            {                
                int minY = 0;
                int maxY = 0;
                
                foreach ( LinkItem * oneLink, _linksIncommingToThis ) {
                    AbstractEntityItem * from = oneLink->getFrom();
                    if ( from ) __arrange_vertically( from, minY, maxY );
                }
    
                foreach( LinkItem * oneLink, _linksOutgoingFromThis ) {
                    AbstractEntityItem * to = oneLink->getTo();
                    if ( to ) __arrange_vertically( to, minY, maxY );
                }
                
                QGraphicsItem::setY( minY );
                if ( maxY - minY > 144 ) {                    
                    _boundingRect.setHeight( maxY - minY );
                };
                

            }; break;
            
        };
    };
    
    return _boundingRect;    
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                  The destructor.                                                 *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                    Деструктор.                                                   *
// *                                                                                                                  *
// ********************************************************************************************************************


tengu::ResizeableItemWithLinks::~ResizeableItemWithLinks() {

}
