// ********************************************************************************************************************
// *                                                                                                                  *
// *                                        Process stop graphics representation.                                     *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                   Графическое представление окончания процесса.                                  *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 16 jun 2017 at 12:57 *
// ********************************************************************************************************************

#include "ProcessStopItem.h"

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                 The constructor.                                                 *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                   Конструктор.                                                   *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::ProcessStopItem::ProcessStopItem ( tengu::ProcessStop * entity, QGraphicsItem* parent ) 
    : AbstractEntityItem ( entity, parent )
{
    _boundingRect = QRect(0,0,76,76);
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                The painting method.                                              *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                  Метод рисования.                                                *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::ProcessStopItem::paint ( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget ) {
    
    int shadowSize = 8;
    
    QPixmap pixmap( _boundingRect.width(), _boundingRect.height() );
    QPainter pixPainter( & pixmap );
    pixPainter.eraseRect( _boundingRect );
    
    // The shadow
    // Тенька.
    
    QColor shadowColor = _processDiagram_shadowColor();
    QPen shadowPen( shadowColor );
    QBrush shadowBrush( shadowColor );
    pixPainter.setBrush( shadowBrush );
    pixPainter.setPen( shadowPen );
    pixPainter.drawEllipse( shadowSize, shadowSize, _boundingRect.width() - shadowSize - 1, _boundingRect.height()-shadowSize-1 );
    
    // Stop process item element body.
    // Тело элемента "конец процесса".
    
    QPen borderPen = _processDiagram_borderPen();
    pixPainter.setPen( borderPen );
    
    int radius = qRound( ( _boundingRect.width() - shadowSize ) / 2.0 );    
    radius -= 7;
    QPoint center( radius, radius );
    QPoint focal( radius-shadowSize, radius-shadowSize );
    
    // The circle around this element.
    // Окружность вокруг элемента.
    
    pixPainter.setBrush( painter->brush() );
    pixPainter.drawEllipse( 1, 1, _boundingRect.width() - shadowSize, _boundingRect.height() - shadowSize );
    
    QRadialGradient gradient( center, radius - 1, focal );
    gradient.setColorAt(0, _processDiagram_brightFillColor() );
    gradient.setColorAt(1, _processDiagram_darkFillColor() );
    QBrush brush = QBrush( gradient );    
    pixPainter.setBrush( brush );
    pixPainter.drawEllipse( 8, 8, radius * 2, radius * 2 ) ; // _boundingRect.width() - shadowSize, _boundingRect.height() - shadowSize );
        
    painter->drawPixmap( 0, 0, pixmap );
    
}


// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                  The destructor.                                                 *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                    Деструктор.                                                   *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::ProcessStopItem::~ProcessStopItem() {

}
