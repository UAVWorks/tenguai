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
    __width = 72;
    __height = 33;
    __orientation = SPO_0;
    
    _boundingRect = QRect( 0, 0, __width, __height );
    _className = "SproutItem";    
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                          Make and return transformation matrix.                                  *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                        Сделать и вернуть матрицу трансформации.                                  *
// *                                                                                                                  *
// ********************************************************************************************************************

QTransform tengu::SproutItem::__transform() {
    QTransform transform;
    transform.translate( - __width/2, __height/2 );
    transform.rotate( 360.0 - (int) getOrientation() );
    transform.translate( __width/2, __height/2 );
    return transform;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                           Recalculate item's screen position.                                    *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                           Пересчет позиции элемента на экране.                                   *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::SproutItem::recalculate() {
    
    QPixmap pixmap( __width + 1, __height + 1 );            
    QPixmap rotated = pixmap.transformed( __transform() );    
    _boundingRect = rotated.rect();    
}


// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                   The paint method.                                              *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                    Метод отрисовки.                                              *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::SproutItem::paint ( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget ) {
    
    _storePainterSettings( painter );
    
    // Draw the picture at the in-memory pixmap
    // Рисуем картинку в памяти.
    
    QPixmap pixmap( __width + 1, __height + 1 );    
    
    QPainter p( & pixmap );
    
    // No color but alpha-layer
    // Цвета нет, но есть альфа-слой.
    
    p.setBrush( QColor( 0, 0, 0, 255 ) );
    p.eraseRect( 0, 0, __width + 1, __height + 1 );
    
    p.setPen( _processDiagram_borderPen() );
    
    QLinearGradient gradient( _boundingRect.topLeft(), _boundingRect.bottomLeft() );
    gradient.setColorAt(0, _processDiagram_brightFillColor() );
    gradient.setColorAt(1, _processDiagram_darkFillColor() );
    gradient.setStart( 1, _boundingRect.height() / 3 );
    QBrush brush( gradient );
    p.setBrush( brush );
    
    int noseWidth = ( _boundingRect.height() - 2 ) / 2;
    
    if ( getSproutType() == Sprout::SP_OUTPUT ) {
        
        QPointF points[5] = {
            QPointF(1,1),
            QPointF( __width - 1 - noseWidth, 1 ),
            QPointF( __width - 1, __height / 2 ),
            QPointF( __width - 1 - noseWidth, __height - 1 ),
            QPointF( 1, __height - 1 )
        
        };    
        p.drawPolygon( points, 5 );
        
    } else {
        
        QPointF points[5] = {
            QPointF(1,1),
            QPointF( __width - 1, 1 ),
            QPointF( __width - 1 - noseWidth, __height / 2 ),
            QPointF( __width - 1, __height - 1 ),
            QPointF( 1, __height - 1 )
        };
        p.drawPolygon( points, 5 );
    };
    
    // Execution mode pixmap.
    // Картинка режима выполнения.
    
    QPixmap emp = _executionModePixmap( false );
    // if ( getExecutionMode() == EM_XPLANE ) 
    //    p.drawPixmap( __width-40, 8, emp );
    // else 
        p.drawPixmap( 10, 8, emp );
    
    // Rotate in-memory picture to specified angle.
    // Поворот картинки в памяти на укзанный угол.
        
    QPixmap rotated = pixmap.transformed( __transform() );    
    
    // Output picture from memory to screen
    // вывод картинки из памяти на экран.
    
    painter->setCompositionMode( QPainter::CompositionMode_Multiply );
    painter->drawPixmap( 0, 0, rotated );
        
    _restorePainterSettings( painter );
    
    /*
    QPixmap pix1 = pixmap.transformed( QTransform().translate(-rRadius, -rRadius));
    QPixmap pix2 = m_pxOriginal.transformed(QTransform().rotate(m_iAn gle));
    QPixmap rotatedPix = pix2.transformed(QTransform().translate(rRadius, rRadius));
    m_pxItem->setPixmap(rotatedPix);
    */
    
    // _storePainterSettings( painter );
    
    
    // _restorePainterSettings( painter );
    
    /*
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
    */
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
// *                               Get type of sprout entity which this item based on.                                *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                           Вернуть тип Sprout'а, на котором базируется данный элемент.                            *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::Sprout::sprout_type_t tengu::SproutItem::getSproutType() {
    Sprout * s = sprout();
    if ( s ) return s->getSproutType();
    return Sprout::SP_INPUT;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                 Set type of sprout entity which this item based on.                              *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                           Установить тип Sprout'а, на котором базируется данный элемент.                         *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::SproutItem::setSproutType ( tengu::Sprout::sprout_type_t type ) {
    Sprout * s = sprout();
    if ( s ) {
        s->setSproutType( type );
        _somethingChanged();
    };
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                         Get this item's draft orientation.                                       *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                Вернуть ориентацию на диаграмме для данного элемента.                             *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::SproutItem::sprout_orientation_t tengu::SproutItem::getOrientation() {
    return __orientation;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                             Set orientation (rotation angle) of this item at the chart.                          *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                          Установить ориентацию (угол поворота) этого элемента на диаграмме.                      *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::SproutItem::setOrientation ( tengu::SproutItem::sprout_orientation_t orientation ) {
    __orientation = orientation;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                              Convert the "entity" to sprout pointer if it is possible.                           *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                       Преобразование "сущности" в Sprout, если такое преобразование возможно.                    *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::Sprout* tengu::SproutItem::sprout() {
    Sprout * sp = dynamic_cast< Sprout * > ( _entity );
    return sp;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                             Return sprout's properties.                                          *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                           Вернуть свойства данного "ростка".                                     *
// *                                                                                                                  *
// ********************************************************************************************************************

QList< QList<tengu::AgentPropertyElement> > tengu::SproutItem::properties() {
    
    QList<QList<tengu::AgentPropertyElement>> p = AbstractEntityItem::properties();
    
    // --------------------------------------------------------------------------------------------
    //                                        Type of this sprout
    //                                        Тип данного Sprout'а.
    // --------------------------------------------------------------------------------------------
    
    QList<AgentPropertyElement> typeElement;
    
    AgentPropertyElement typeName( tr("Type") );
    typeName.backgroundColor = typeName.widgetBackground();
    typeElement.append( typeName );
    
    AgentPropertyElement typeValue;
    typeValue.value = QVariant( (int) getSproutType() );
    typeValue.readOnly = false;
    typeValue.type = AgentPropertyElement::SproutTypeSelector;
    typeValue.propertyName = "sprout_type";
    typeValue.backgroundColor = typeValue.widgetBackground();
    typeElement.append( typeValue );
    
    p.append( typeElement );
    
    // --------------------------------------------------------------------------------------------
    //                                    Rotation angle on the chart 
    //                                     Угол поворота на диаграмме 
    // --------------------------------------------------------------------------------------------
    
    QList<AgentPropertyElement> angleElement;
    
    AgentPropertyElement angleName( tr("Rotation agnle") );
    angleName.backgroundColor = angleName.widgetBackground();
    angleElement.append( angleName );
    
    AgentPropertyElement angleValue;
    angleValue.value = QVariant( ( int ) getOrientation() );
    angleValue.propertyName = "orientation";
    angleValue.readOnly = false;
    angleValue.type = AgentPropertyElement::SproutAngleSelector;
    angleValue.backgroundColor = angleValue.widgetBackground();
    angleElement.append( angleValue );
    
    p.append( angleElement );
    
    return p;
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

