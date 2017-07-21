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
        
    if ( __vertical_correction() ) {
        
        int minY = 0;
        int maxY = 0;
        bool need = false;
                
        foreach ( LinkItem * oneLink, _linksIncommingToThis ) {
            AbstractEntityItem * from = oneLink->getFrom();
            if ( from ) __arrange_vertically( from, minY, maxY );
            need = true;
        }
    
        foreach( LinkItem * oneLink, _linksOutgoingFromThis ) {
            AbstractEntityItem * to = oneLink->getTo();
            if ( to ) __arrange_vertically( to, minY, maxY );
            need = true;
        }
                
                
        if ( need ) {

            QGraphicsItem::setY( minY );
                
            if ( maxY - minY > 144 ) {                    
                _boundingRect.setHeight( maxY - minY );
            };
        };

    };
    
    return _boundingRect;    
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                Add an incomming link.                                            *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                               Добавить входящую связь.                                           *
// *                                                                                                                  *
// ********************************************************************************************************************
/*
void tengu::ResizeableItemWithLinks::addIncommingLink ( tengu::LinkItem* link ) {
    tengu::ItemWithLinks::addIncommingLink ( link );
    // link->__calcPosTo = false;
}
*/
// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                Add an outgoing link.                                             *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                              Добавить исходящую связь.                                           *
// *                                                                                                                  *
// ********************************************************************************************************************
/*
void tengu::ResizeableItemWithLinks::addOutgoingLink ( tengu::LinkItem* link ) {
    tengu::ItemWithLinks::addOutgoingLink ( link );
    // link->__calcPosFrom = false;
}
*/
// ********************************************************************************************************************
// *                                                                                                                  *
// *                                     Need this item vertically size correction?                                   *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                      Нужна ли коррекция размеров по вертикали?                                   *
// *                                                                                                                  *
// ********************************************************************************************************************

bool tengu::ResizeableItemWithLinks::__vertical_correction() {
    
    if ( _entity ) {
        
        AbstractEntity::entity_types_t type = _entity->entityType();
        
        if ( 
            ( type == AbstractEntity::ET_ANDor )
            || ( type == AbstractEntity::ET_ORer ) 
        ) return true;
    };
    
    return false;
        
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                               Recalculate coordinates.                                           *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                               Пересчитать координаты                                             *
// *                                                                                                                  *
// ********************************************************************************************************************

/*
void tengu::ResizeableItemWithLinks::recalculate() {
    
    qDebug() << "ResizeableItemWithLinks::recalculate";
    
    foreach ( LinkItem * link, _linksIncommingToThis ) {
        link->recalculate();
        link->__posTo.setY( link->__posFrom.y() );        
    };
    
    foreach ( LinkItem * link, _linksOutgoingFromThis ) {
        link->recalculate();
        link->__posFrom.setY( link->__posTo.y() );        
    };
    
}
*/

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                        Update this element, overrided function.                                  *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                       Обновление элемента, перекрытая функция.                                   *
// *                                                                                                                  *
// ********************************************************************************************************************
/*
void tengu::ResizeableItemWithLinks::update( const QRectF & rect ) {
        
    qDebug() << "ResizeableItemWithLinks::update()";
    
    foreach ( LinkItem * link, _linksIncommingToThis ) {
        link->recalculate();
        link->__posTo.setY( link->__posFrom.y() );
        link->update();
    };
    
    foreach ( LinkItem * link, _linksOutgoingFromThis ) {
        link->recalculate();
        link->__posFrom.setY( link->__posTo.y() );
        link->update();
    };
    
    AbstractEntityItem::update( rect );    
}
*/
// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                  The destructor.                                                 *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                    Деструктор.                                                   *
// *                                                                                                                  *
// ********************************************************************************************************************


tengu::ResizeableItemWithLinks::~ResizeableItemWithLinks() {

}
