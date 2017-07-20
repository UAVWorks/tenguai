// ********************************************************************************************************************
// *                                                                                                                  *
// *                                  The link representation. Arrows between agents.                                 *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                              Графическое представление стрелок (связей между агентами).                          *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 18 jun 2017 at 13:19 *
// ********************************************************************************************************************

#include "LinkItem.h"

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                The constructor.                                                  *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                  Конструктор.                                                    *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::LinkItem::LinkItem() 
    : AbstractEntityItem()
{
    _boundingRect = QRect(0, 0, 64, 22);
    _class_name = "LinkItem";    
    _canChangeExecuteMode = true;
    
    __from = nullptr;
    __to = nullptr;
    __posFrom = QPoint( MINIMUM_CONSTRAINT, MINIMUM_CONSTRAINT );
    __posTo = QPoint( MINIMUM_CONSTRAINT, MINIMUM_CONSTRAINT );
    __tempTo = QPoint( MINIMUM_CONSTRAINT, MINIMUM_CONSTRAINT );
    
    // __withSproutFrom = false;
    // __withSproutTo = false;
    
    __system_name = QString("Link");
    __human_name = QString("Link");
    __comment = QString("");
    __execution_mode = AbstractEntity::EM_ALWAYS;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                         Entity for link is always empty.                                         *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                     "Сущность" для стрелки - всегда пустая.                                      *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::LinkItem::checkEntity() {
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                   Get system name                                                *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                Получить системное имя.                                           *
// *                                                                                                                  *
// ********************************************************************************************************************

QString tengu::LinkItem::getSystemName() {
    return __system_name;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                    Set system name                                               *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                Установить системное имя.                                         *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::LinkItem::setSystemName ( QString name ) {
    __system_name = name;
    _somethingChanged();
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                    Get human name                                                *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                               Получить "человеческое" имя.                                       *
// *                                                                                                                  *
// ********************************************************************************************************************

QString tengu::LinkItem::getHumanName() {
    return __human_name;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                     Set human name                                               *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                Установить человеческое имя.                                      *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::LinkItem::setHumanName ( QString name ) {
    
    __human_name = name;
    _somethingChanged();
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                       Get comment                                                *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                   Получить комментарий.                                          *
// *                                                                                                                  *
// ********************************************************************************************************************

QString tengu::LinkItem::getComment() {
    return __comment;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                        Set comment                                               *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                   Установить комментарий.                                        *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::LinkItem::setComment ( QString comment ) {
    
    __comment = comment;
    _somethingChanged();
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                     Get execution mode                                           *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                  Получить режим выполнения.                                      *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::AbstractEntity::execution_mode_t tengu::LinkItem::getExecutionMode() {
    return __execution_mode;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                      Set execution mode                                          *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                  Установить режим выполнения.                                    *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::LinkItem::setExecutionMode ( tengu::AbstractEntity::execution_mode_t mode ) {
    __execution_mode = mode;
    _somethingChanged();
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                          Set the item link comes from.                                           *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                       Установить элемент, откуда идет связь.                                     *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::LinkItem::__setFrom ( tengu::AbstractEntityItem * entity ) {
    
    __from = entity;
    // __withSproutFrom = withSproutFrom;
    __checkSproutInsideTask();
    
    // if ( entity ) entity->addOutgoingLink( this );
    
    recalculate();
    update();
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                            Set the item link goes to.                                            *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                       Установить элемент, куда идет связь.                                       *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::LinkItem::__setTo ( tengu::AbstractEntityItem * entity ) {
    
    __to = entity;
    // __withSproutTo = withSproutTo;
    __checkSproutInsideTask();
    
    // if ( entity ) entity->addIncommingLink( this );
    
    // Set the link between agents.
    // Установка связи между агентами.
    /*
    
    if ( ( __from ) && ( __to ) ) {
        
        // We will not draw a nose of arrow if at least one of the link's participants is Sprout.
        // Мы не будем рисовать носик связи, если хоть один из участников - это Sprout.
        
        // SproutItem * prevIsSprout = dynamic_cast< SproutItem * >( __from );
        // SproutItem * nextIsSprout = dynamic_cast< SproutItem * >( __to );
        
        // __withSprout = prevIsSprout || nextIsSprout;
        
        if ( ! __withSprout ) {
            
            // If this is link between agents, we need to set previous/next focus flow elements in agents.
            // Если это связь между агентами - нужно установить предыдущий-следующий по фокусу у самих агентов.
            
            AbstractAgent * toAgent = dynamic_cast< AbstractAgent * >(  __to->entity() );
            AbstractAgent * fromAgent = dynamic_cast<AbstractAgent * >( __from->entity() );
            if ( ( toAgent ) && ( fromAgent ) ) {
                toAgent->addPreviousByFocus( fromAgent );
                fromAgent->addNextByFocus( toAgent );
            };
            
        } else {
            
            // If this is link between sprout and task, we need add sprout to task.
            // Если это связь между Sprout и задачей, то нужно добавить Sprout к задаче.
        }
    };
    */
    
    __tempTo = QPoint( 0, 0 );
    
    recalculate();
    update();
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                  check we have an sprout and it is inside task                                   *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                      Проверка того, что у нас есть sprout, и если да - то он внутри задачи.                      *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::LinkItem::__checkSproutInsideTask() {
            
    if ( ( __from != nullptr ) && ( __to != nullptr ) &&          
        ( ( withSproutFrom() ) || ( withSproutTo() ) ) 
    ) {
        __from->checkEntity();
        __to->checkEntity();
        
        SproutableAgent * agent = nullptr;
        Sprout * sprout = nullptr;
        
        if ( withSproutFrom() ) {
            agent = dynamic_cast< SproutableAgent * > ( __to->entity() );
            sprout = dynamic_cast < Sprout * > ( __from->entity() );
        };
        
        if ( withSproutTo() ) {
            agent = dynamic_cast < SproutableAgent * > ( __from->entity() );
            sprout = dynamic_cast< Sprout * > ( __to->entity() );
        };
        
        if ( ( agent ) && ( sprout ) ) {
            agent->addSprout( sprout );
        }
    };
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                      Set the coordinates link goes to.                                           *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                   Установить координаты, куда идет связь.                                        *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::LinkItem::setTo ( QPoint to ) {
    __tempTo = to;
    recalculate();
    update();    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                         The link have not any entities.                                          *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                       Связь вообще не имеет "сущностей"                                          *
// *                                                                                                                  *
// ********************************************************************************************************************

bool tengu::LinkItem::isEmpty() {
    return ( ( ! __from ) && ( ! __to ) );
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                 The create link process is still not finished.                                   *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                   Процесс создания связи все еще не закончен.                                    *
// *                                                                                                                  *
// ********************************************************************************************************************

bool tengu::LinkItem::semiCreated() {
    return ( ( !__from ) || ( ! __to ) );
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                      From-point and to-point correction in case one of linked items is sprout.                   *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                  Коррекция начальной и конечной точек, если один из связанных элементов - это Sprout.            *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::LinkItem::__correctPointsForSprouts( int x, QRect fromRect, QRect toRect ) {
        
    if ( withSproutTo() ) {
        __posFrom.setX( __posTo.x() );
        if ( __posFrom.x() < __from->x() + fromRect.topLeft().x() - x ) __posFrom.setX( __from->x() + fromRect.topLeft().x() - x );
        if ( __posFrom.x() > __from->x() + fromRect.topRight().x() - x ) __posFrom.setX( __from->x() + fromRect.topRight().x() - x  );
    };
    
    if ( withSproutFrom() ) {
        __posTo.setX( __posFrom.x() );
        if ( __posTo.x() < __to->x() + toRect.topLeft().x() - x ) __posTo.setX( __to->x() + toRect.topLeft().x() - x );
        if ( __posTo.x() > __to->x() + toRect.topRight().x() - x ) __posTo.setX( __to->x() + toRect.topRight().x() - x );
    };

}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                    Re-calculate graphics representation of link.                                 *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                    Пересчитать графическое представление связи.                                  *
// *                                                                                                                  *
// *********************************************надстоящий***********************************************************************

void tengu::LinkItem::recalculate() {
        
    int x = 0, y = 0, w = 0, h = 0;
        
    if ( __from ) {        
        
        QRect fromRect = __from->boundingRect().toRect();
        // LinkItem * fromLink = dynamic_cast<LinkItem * > ( __from );
        
        if ( __to ) {
            
            // ------------------------------------------------------------------------------------
            //                      We have an finished element for this link.
            //                             У нас есть конечный элемент.
            // ------------------------------------------------------------------------------------
            
            QRect toRect = __to->boundingRect().toRect();
            
            // SproutItem * sproutFrom = dynamic_cast< SproutItem * > ( __from ) ;
            // SproutItem * sproutTo = dynamic_cast< SproutItem * > ( __to );
            
            if ( toRect.topLeft().x() + __to->x() > fromRect.topRight().x() + __from->x() ) {
                
                // __to is the right of X that __from
                // __to правее по X, чем __from.
                
                x = fromRect.topRight().x() + __from->x();
                y = fromRect.topRight().y() + __from->y();
                if ( y > toRect.topLeft().y() + __to->y() ) {
                    y = toRect.topLeft().y() + __to->y();
                };
                
                w = ( toRect.topLeft().x() + __to->x() ) - ( fromRect.topRight().x() + __from->x() );                
                h = fromRect.bottomRight().y() + __from->y() - y;
                if ( h < toRect.bottomRight().y() + __to->y() - y ) h = toRect.bottomRight().y() + __to->y() - y;
                
                __posFrom.setX( 1 );
                __posFrom.setY( ( fromRect.topRight().y() + __from->y() - y ) + fromRect.height() / 2 );
                __posTo.setX( w - 1 );
                __posTo.setY( ( __to->y() + toRect.height() / 2 ) - y );
                
            }  else if ( toRect.topRight().x() + __to->x() < fromRect.topLeft().x() + __from->x() ) {
                
                // We are to the left of the rectangle of the first element.
                // Мы находимся левее, чем прямоугольник первого элемента.                
                
                x = 0;
                y = 0;
                w = 0;
                h = 0;
                __posFrom.setX( MINIMUM_CONSTRAINT ); __posFrom.setY( MINIMUM_CONSTRAINT );
                __posTo.setX( MINIMUM_CONSTRAINT ); __posTo.setY( MINIMUM_CONSTRAINT );
                
            } else if ( toRect.bottomRight().y() + __to->y() < fromRect.topLeft().y() + __from->y() ) {                
                
                // We are above the block __from
                // Мы находимся выше блока __from
                                
                x = __to->x(); 
                if ( x > __from->x() ) x = __from->x();                
                y = __to->y() + toRect.height();
                                
                w = toRect.bottomRight().x() + __to->x() - x; 
                if ( w < fromRect.topRight().x() + __from->x() - x ) w = fromRect.topRight().x() + __from->x() - x;
                h = fromRect.topLeft().y() + __from->y() - y;
                                
                __posFrom.setX( __from->x() + fromRect.topLeft().x() - x + fromRect.width() / 2 );
                __posFrom.setY( h - 1 );
                __posTo.setX( __to->x() + toRect.topLeft().x() - x + toRect.width() / 2 );
                __posTo.setY( 1 );
                
                __correctPointsForSprouts( x, fromRect, toRect );
                
                /*
                if ( sproutTo ) {
                    __posFrom.setX( __posTo.x() );
                    if ( __posFrom.x() < __from->x() + fromRect.topLeft().x() - x ) __posFrom.setX( __from->x() + fromRect.topLeft().x() - x );
                    if ( __posFrom.x() > __from->x() + fromRect.topRight().x() - x ) __posFrom.setX( __from->x() + fromRect.topRight().x() - x  );
                };
                
                if ( sproutFrom ) {
                    __posTo.setX( __posFrom.x() );
                    if ( __posTo.x() < __to->x() + toRect.topLeft().x() - x ) __posTo.setX( __to->x() + toRect.topLeft().x() - x );
                    if ( __posTo.x() > __to->x() + toRect.topRight().x() - x ) __posTo.setX( __to->x() + toRect.topRight().x() - x );
                };
                */
                                
                
            } else if ( toRect.topLeft().y() + __to->y() > fromRect.bottomLeft().y() + __from->y() ) {
                
                // We are bellow the block __from
                // Мы находимся ниже блока __from
                
                x = __from->x() + fromRect.bottomLeft().x();
                if ( x > __to->x() + toRect.topLeft().x() ) x = __to->x() + toRect.topLeft().x();
                y = __from->y() + fromRect.bottomLeft().y();
                w = toRect.bottomRight().x() + __to->x() - x; 
                if ( w < fromRect.topRight().x() + __from->x() - x ) w = fromRect.topRight().x() + __from->x() - x;
                h = __to->y() - y;
                
                __posFrom.setX( __from->x() + fromRect.topLeft().x() - x + fromRect.width() / 2  );
                __posFrom.setY( 1 );
                __posTo.setX( __to->x() + toRect.topLeft().x() - x + toRect.width() / 2 );
                __posTo.setY( h-1 );
                
                __correctPointsForSprouts( x, fromRect, toRect );
                               
                
            } else {
                
                // We do not understand where we are. It looks like inside the block.
                // Непонятно, где находимся. Похоже, что внутри блока.
                /*
                x = fromRect.topLeft().x() + __from->x();
                y = fromRect.topLeft().y() + __from->y();
                w = fromRect.width();
                h = fromRect.height();
                __posFrom.setX( fromRect.width() / 2 );
                __posFrom.setY( fromRect.height() / 2 );
                __posTo.setX( __tempTo.x() - x );
                __posTo.setY( __tempTo.y() - y );
                */
            };
                        
        } else if ( ( __tempTo.x() > MINIMUM_CONSTRAINT ) && ( __tempTo.y() > MINIMUM_CONSTRAINT ) ) {
            
            // ------------------------------------------------------------------------------------
            //                           We navigate the point of the mouse
            //                              Мы ориентируемся по точке мыши.
            // ------------------------------------------------------------------------------------
            
            if ( __tempTo.x() > fromRect.topRight().x() + __from->x() ) {
                
                // We are to the right of the rectangle of the first element.
                // Мы находимся правее, чем прямоугольник первого элемента.
                
                x = fromRect.topRight().x() + __from->x();
                y = fromRect.topRight().y() + __from->y();
                if ( y > __tempTo.y() ) {
                    y = __tempTo.y();
                };
                
                w = __tempTo.x() - ( fromRect.topRight().x() + __from->x() );
                h = fromRect.bottomRight().y() + __from->y() - y;
                if ( h < __tempTo.y() - y ) h = __tempTo.y() - y;
                
                __posFrom.setX( 1 );
                __posFrom.setY( ( fromRect.topRight().y() + __from->y() - y ) + fromRect.height() / 2 );
                __posTo.setX( __tempTo.x() - x );
                __posTo.setY( __tempTo.y() - y );                
                                
            } else if ( __tempTo.x() < fromRect.topLeft().x() + __from->x() ) {
                
                // We are to the left of the rectangle of the first element.
                // Мы находимся левее, чем прямоугольник первого элемента.                
                
                x = 0;
                y = 0;
                w = 0;
                h = 0;
                __posFrom.setX( MINIMUM_CONSTRAINT ); __posFrom.setY( MINIMUM_CONSTRAINT );
                __posTo.setX( MINIMUM_CONSTRAINT ); __posTo.setY( MINIMUM_CONSTRAINT );
                
            } else if ( __tempTo.y() < fromRect.topLeft().y() + __from->y() ) {                
                
                // We are above the block __from
                // Мы находимся выше блока __from
                
                x = __from->x() + fromRect.topLeft().x();                
                y = fromRect.topLeft().y() + __from->y(); if ( y > __tempTo.y() ) y = __tempTo.y();                
                w = fromRect.width(); 
                h = fromRect.topLeft().y() + __from->y() - y;
                __posFrom.setX( w / 2 );
                __posFrom.setY( h - 1 );
                __posTo.setX( __tempTo.x() - __from->x() );
                __posTo.setY( 1 );
                
            } else if ( __tempTo.y() > fromRect.bottomLeft().y() + __from->y() ) {
                
                // We are bellow the block __from
                // Мы находимся ниже блока __from
                
                x = __from->x() + fromRect.bottomLeft().x();
                y = __from->y() + fromRect.bottomLeft().y();
                w = fromRect.width();
                h = __tempTo.y() - __from->y() - fromRect.bottomLeft().y();
                __posFrom.setX( fromRect.width() / 2 );
                __posFrom.setY( 1 );
                __posTo.setX( __tempTo.x() - __from->x() );
                __posTo.setY( h-1 );
                
            } else {
                
                // We do not understand where we are. It looks like inside the block.
                // Непонятно, где находимся. Похоже, что внутри блока.
                
                x = fromRect.topLeft().x() + __from->x();
                y = fromRect.topLeft().y() + __from->y();
                w = fromRect.width();
                h = fromRect.height();
                __posFrom.setX( fromRect.width() / 2 );
                __posFrom.setY( fromRect.height() / 2 );
                __posTo.setX( __tempTo.x() - x );
                __posTo.setY( __tempTo.y() - y );
            };
        
        }; // else, i.e. we have not a __to element. 
    };                
    
    // qDebug() << "Итого после recalculate x=" << x << ",y=" << y << ",w=" << w << ",h=" << h << ", fromPoint=" << __posFrom << ", toPoint=" << __posTo;
    
    QGraphicsObject::setX( x );
    QGraphicsObject::setY( y );
    _boundingRect.setWidth( w );
    _boundingRect.setHeight( h );
            
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                              The painting method.                                                *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                Метод рисования.                                                  *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::LinkItem::paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget ) {
            
    if ( ( __posFrom.x() <= MINIMUM_CONSTRAINT ) || ( __posFrom.y() <= MINIMUM_CONSTRAINT ) || ( __posTo.x() <= MINIMUM_CONSTRAINT ) || ( __posTo.y() <= MINIMUM_CONSTRAINT ) ) return;
    
    _storePainterSettings( painter );
    
    // -----------------------------------------------
    // For debug purposes, do not remove it.
    // Для отладки, не удаляй.
    // _drawBorderRect( painter );
    // -----------------------------------------------
    
    QPixmap pixmap( _boundingRect.width() + 1, _boundingRect.height() + 1 );
    QPainter p( & pixmap );
    QColor c(0,0,0,255);
    p.setBrush( QBrush( c ) );
    p.eraseRect( 0, 0, _boundingRect.width() + 1, _boundingRect.height() + 1 );
        
    QPen pen;
    
    if ( semiCreated() ) {
        
        // We are in the process of creating.
        // Мы находимся в процессе создания.
        
        pen.setColor( QColor( 128, 128, 128 ) );
        pen.setWidth( 2 );
        pen.setStyle( Qt::DashLine );
        
    } else {       
            
        if ( isSelected() ) {
            pen.setColor( QColor( 92, 32, 32 ) );       
            pen.setWidth( 6 );
        } else if (( withSproutFrom() ) || ( withSproutTo() ) ) {
            pen.setColor( QColor( 164, 164, 164 ) );
            pen.setWidth( 1 );
        } else  {
            pen.setColor( _processDiagram_borderColor() );
            pen.setWidth( 3 );
        };
        
    }
        
    p.setPen( pen );
    
    // The arrow's line
    // Линия стрелки.
    
    p.drawLine( __posFrom, __posTo );
    
    // The nose of the arrow
    // Носик стрелки.
    
    if ( ( ! withSproutFrom() ) && ( ! withSproutTo() ) ) {
    
        QLineF line( __posFrom, __posTo );
        QTransform transform;
        transform.rotate( - line.angle() );
    
        QPointF endT = ( transform.map( QPointF( -18.0, -10.0 ) ) + __posTo ).toPoint();
        QPointF endB = ( transform.map( QPointF( -18.0, 10.0 ) ) + __posTo ).toPoint();
    
        if ( endT.x() > _boundingRect.width() - 1 ) endT.setX( _boundingRect.width() - 1 );
        if ( endB.y() > _boundingRect.height() - 1 ) endB.setY ( _boundingRect.height() - 1  );
    
        p.drawLine( __posTo, endT );
        p.drawLine( __posTo, endB );
    }
    
    painter->drawPixmap(0, 0, pixmap );
    
    _restorePainterSettings( painter );
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                    Calculate distance to global schema point                                     *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                         Посчитать расстояние до точки в глобальных координатах всей схемы.                       *
// *                                                                                                                  *
// ********************************************************************************************************************

float tengu::LinkItem::distanceTo ( QPointF point ) {
        
    QPointF delta = point - QPoint(x(), y() ) - __posFrom;
    
    QPoint begin( x() + __posFrom.x(), y() + __posFrom.y() );
    QPoint end( x() + __posTo.x(), y() + __posTo.y() );
    QLineF line( begin, end );
    QLineF normal = line.normalVector();    
    
    normal.setP1( normal.p1() + delta );
    normal.setP2( normal.p2() + delta );
        
    QPointF intersect;    
    normal.intersect( line, & intersect );
    intersect = intersect - point;
    
    return intersect.manhattanLength();
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                        Goes this link from the sprout?                                           *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                        Идет ли эта связь из спраута?                                             *
// *                                                                                                                  *
// ********************************************************************************************************************


bool tengu::LinkItem::withSproutFrom() {
    return ( ( __from ) && ( __from->entityType() == AbstractEntityItem::ET_SproutItem ) );
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                          Comes this link into the sprout?                                        *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                           Входит ли данная связь в спраут?                                       *
// *                                                                                                                  *
// ********************************************************************************************************************

bool tengu::LinkItem::withSproutTo() {
    return ( ( __to ) && ( __to->entityType() == AbstractEntityItem::ET_SproutItem ) );
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                    Return the element which this links comes from.                               *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                   Вернуть элемент, откуда выходит данная связь.                                  *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::AbstractEntityItem * tengu::LinkItem::getFrom() {
    return __from;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                     Return the element which this links goes to.                                 *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                     Вернуть элемент, куда входит данная связь.                                   *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::AbstractEntityItem * tengu::LinkItem::getTo() {
    return __to;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                The destructor.                                                   *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                  Деструктор.                                                     *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::LinkItem::~LinkItem() {
        
    emit signalLinkRemoved( this->getUUID() );
    
    // if ( __from ) __from->removeLink( this );
    // if ( __to ) __to->removeLink( this );

    /*
    if ( ( __from ) && ( __to ) ) {
        AbstractAgent * fromAgent = dynamic_cast<AbstractAgent * >( __from->entity() );
        AbstractAgent * toAgent = dynamic_cast<AbstractAgent * > ( __to->entity() );
        if ( ( toAgent ) && ( fromAgent ) ) {
            toAgent->removeNeighborByFocus( fromAgent );
            fromAgent->removeNeighborByFocus( toAgent );
        };
    };
    */
        
}
