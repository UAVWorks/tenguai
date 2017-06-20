// ********************************************************************************************************************
// *                                                                                                                  *
// *     Graphics representation of Sprout class (signal/slot mechanism, inter-agent redis-based communications).     *
// * ---------------------------------------------------------------------------------------------------------------- *
// *          Графическое представление класса Sprout ("росток' - механизм сигналов-слотов, межагентное общение,      *
// *                                                  основанное на redis'е.                                          *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 10 jun 2017 at 12:20 *
// ********************************************************************************************************************

#include "SproutItem.h"

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                   The constructor.                                               *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                     Конструктор.                                                 *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::SproutItem::SproutItem ( tengu::Sprout * sprout, QGraphicsItem* parent ) 
    : AbstractEntityItem ( sprout , parent )
{
    _boundingRect = QRect( 0, 0, 71, 33 );
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                   The paint method.                                              *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                    Метод отрисовки.                                              *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::SproutItem::paint ( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget ) {
    
    QPixmap pixmap( _boundingRect.width(), _boundingRect.height() ) ;
    QPainter pixPainter( & pixmap );
    pixPainter.eraseRect( _boundingRect );
    
    // For debug purposes only, for visibility.
    // Только для отладки, чтобы было видно.
    // pixPainter.fillRect( _boundingRect, QColor(192, 0, 0 ) );
    
    // --------------------------------------------------------------------------------------------
    //                                             Shadows.
    //                                              Теньки.
    // --------------------------------------------------------------------------------------------
    
    QColor shadowColor = _processDiagram_shadowColor(); 
    pixPainter.fillRect( QRect(9, 6, 63, 27 ), shadowColor );
    
    // Erasing shadow's angle
    // Очистка уголков теньки.
    
    for ( int i=0; i<20; i++ ) { 
        // bottom of shadow.
        pixPainter.eraseRect( QRect( 52+i, 32-i, 19-i, 1 ) );
        // top of shadow.
        if ( i < 10 ) pixPainter.eraseRect( QRect(58+i, 6+i, 13-i, 1 ) );
    };
    
    
    // --------------------------------------------------------------------------------------------
    //                                       The sprout's body.
    //                                          Тело "ростка". 
    // --------------------------------------------------------------------------------------------
    
    QLinearGradient gradientUP( 1, 1, 1, 12 ); 
    gradientUP.setColorAt(0, _processDiagram_darkFillColor() );
    gradientUP.setColorAt(1, _processDiagram_brightFillColor() );
    pixPainter.fillRect( QRect( 1, 1, 54, 12), gradientUP );
    
    QLinearGradient gradientDown ( 1, 12, 1, 24 );
    gradientDown.setColorAt( 0, _processDiagram_brightFillColor() );
    gradientDown.setColorAt( 1, _processDiagram_darkFillColor() );
    pixPainter.fillRect( QRect( 1, 12, 54, 12 ), gradientDown );
    
    // Erasing extra pieces of the body
    // Стирание лишних кусков тела.
    
    for ( int i=0; i<6; i++ ) {
        // right up corner
        // Правый верхний угол.
        pixPainter.eraseRect( 50-i, 5-i, 21+i, 1 );
        // right down corner
        // правый нижний угол.
        pixPainter.fillRect( 52-i, 18+i, 14, 1, shadowColor );
    };
    
    
    
    // One point as shadow
    // Одна точка - тенькой.
    pixPainter.fillRect( 54, 6, 1, 1, shadowColor );
    
    // --------------------------------------------------------------------------------------------
    //                                          The borders.
    //                                            Границы.
    // --------------------------------------------------------------------------------------------
        
    pixPainter.setPen( _processDiagram_borderPen() );        
    // Top line for nose begin.
    // Верхняя линия до начала носика.    
    pixPainter.drawLine( 1, 1, 46, 1 );    
    // Left line
    // Линия слева.    
    pixPainter.drawLine( 1, 1, 1, 24 );    
    // Down line for nose begin.
    // Нижняя линия до начала носика.    
    pixPainter.drawLine( 1, 24, 45, 24 );    
    // right top line of the nose.
    // правая верхняя линия носика.    
    // pixPainter.drawLine( 46, 1, 58, 13 );
    pixPainter.drawLine( 46, 1, 57, 12 );
    // right down line of the nose.
    // Правая нижняя линия носика.
    // pixPainter.drawLine( 46, 24, 58, 12 );
    pixPainter.drawLine( 46, 24, 57, 13 );
    
    
    painter->drawPixmap( 0, 0, pixmap );
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                            Check the entity is valid.                                            *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                      Проверяем, валидна ли ссылка на entity                                      *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::SproutItem::checkEntity() {
    if ( ! _entity ) _entity = new Sprout();
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                             Return sprout's properties.                                          *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                           Вернуть свойства данного "ростка".                                     *
// *                                                                                                                  *
// ********************************************************************************************************************

QList< QList<tengu::AgentPropertyElement> > tengu::SproutItem::properties() {
    return tengu::AbstractEntityItem::properties();
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                  The destructor.                                                 *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                     Деструктор.                                                  *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::SproutItem::~SproutItem() {

}

