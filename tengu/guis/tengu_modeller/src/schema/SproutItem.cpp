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
    : ItemWithLinks ( sprout , parent )
{
    __width = 72;
    __height = 33;
    __orientation = SPO_0;    
    
    _iAmSprout = true;
    
    _boundingRect = QRect( 0, 0, __width, __height );
    _className = "SproutItem";   
    _canChangeExecuteMode = true;
    
    manualSignalNameSelection = false;
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                              Is this sprout item orphan?                                         *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                          Является ли данный sprout - сиротой?                                    *
// *                                                                                                                  *
// ********************************************************************************************************************

bool tengu::SproutItem::isOrphan() {
    
    Sprout * sp = sprout();
    if ( ( ! sp ) || ( ! sp->owner() ) ) return true;
    return false;
        
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
    
    Q_UNUSED( option );
    Q_UNUSED( widget );
    
    _storePainterSettings( painter );
    
    // Draw the picture at the in-memory pixmap
    // Рисуем картинку в памяти.
    
    QPixmap pixmap( __width + 1, __height + 1 );    
    
    QPainter p( & pixmap );
    
    // Cleaning. No color but alpha-layer
    // Очистка. Цвета нет, но есть альфа-слой.
    
    p.setBrush( QColor( 0, 0, 0, 255 ) );
    p.eraseRect( 0, 0, __width + 1, __height + 1 );
    
    QPen borderPen = _processDiagram_borderPen();
    QColor brightColor = _processDiagram_brightFillColor();
    QColor darkColor = _processDiagram_darkFillColor();
    
    if ( isOrphan() ) {
        // For orphanded sprout colors will be demonstrate it's inaccessibility.
        // Для "осиротевшего" sprout'а цвета показывают его недоступность.
        
        borderPen.setColor( QColor( 128, 128, 128 ) );        
        brightColor = QColor( 232, 232, 232 );
        darkColor = QColor( 192, 192, 192 );
    };
    
    p.setPen( borderPen );
    
    QLinearGradient gradient( _boundingRect.topLeft(), _boundingRect.bottomLeft() );
    gradient.setColorAt(0, brightColor );
    gradient.setColorAt(1, darkColor );
    gradient.setStart( 1, _boundingRect.height() / 3 );
    QBrush brush( gradient );
    p.setBrush( brush );
    
    int noseWidth = ( _boundingRect.height() - 2 ) / 2;
    
    bool isOut = ( ( getSproutType() == Sprout::IN_PROCESS_OUTPUT ) || ( getSproutType() == Sprout::EXTERNAL_OUTPUT ) );
    
    if ( isOut ) {
        
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
    return Sprout::EXTERNAL_INPUT;
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
        if ( s->isExternal() ) {
            Task * task = dynamic_cast<Task * >( s->owner() );
            if ( ( task ) && ( ( ! task->isSubscriberConnected() ) || ( ! task->isPublisherConnected() ) ) ) {
                task->connect();
            };
        };
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
    _somethingChanged();
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                 Get signal name                                                  *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                               Вернуть имя сигнала.                                               *
// *                                                                                                                  *
// ********************************************************************************************************************

QString tengu::SproutItem::getSignalName() {
    if ( sprout() ) return sprout()->getSignalName();
    return QString("");
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                 Set signal name.                                                 *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                             Установить имя сигнала.                                              *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::SproutItem::setSignalName ( QString name ) {
    if ( sprout() ) {
        sprout()->setSignalName( name );
        _somethingChanged();
    };
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                      Get minimal allowed value for this sprout.                                  *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                        Получить минимально допустимое значение для данного "росточка".                           *
// *                                                                                                                  *
// ********************************************************************************************************************

float tengu::SproutItem::getMinimalValue() {
    if ( sprout() ) return sprout()->getMinimalValue();
    return MINIMUM_CONSTRAINT;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                      Set minimal allowed value for this sprout.                                  *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                           Установить минимально допустимое значение данного "росточка".                          *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::SproutItem::setMinimalValue ( float min ) {
    if ( sprout() ) {
        sprout()->setMinimalValue( min );
        _somethingChanged();
    }
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                    Get maximal allowed value for this sprout                                     *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                          Получить максимально допустимое значение для данного "росточка"                         *
// *                                                                                                                  *
// ********************************************************************************************************************

float tengu::SproutItem::getMaximalValue() {
    if ( sprout() ) return sprout()->getMaximalValue();
    return MINIMUM_CONSTRAINT;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                    Set maximal allowed value for this sprout.                                    *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                        Установить максимально допустимое значение для данного "росточка".                        *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::SproutItem::setMaximalValue ( float max ) {
    
    if ( sprout() ) {
        sprout()->setMaximalValue( max );
        _somethingChanged();
    };
    
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
    //                                    Rotation angle on the chart 
    //                                     Угол поворота на диаграмме 
    // --------------------------------------------------------------------------------------------
    
    QList<AgentPropertyElement> angleElement;
    
    angleElement.append( AgentPropertyElement::captionElement( tr("Rotation agnle") ) );
    
    AgentPropertyElement angleValue;
    angleValue.value = QVariant( ( int ) getOrientation() );
    angleValue.propertyName = "orientation";
    angleValue.readOnly = false;
    angleValue.type = AgentPropertyElement::SproutAngleSelector;
    angleValue.backgroundColor = angleValue.widgetBackground();
    angleElement.append( angleValue );
    
    p.append( angleElement );
    
    // --------------------------------------------------------------------------------------------
    //                                        Type of this sprout
    //                                        Тип данного Sprout'а.
    // --------------------------------------------------------------------------------------------
    
    QList<AgentPropertyElement> typeElement;    
    typeElement.append( AgentPropertyElement::captionElement( tr("Signal type") ) );
    
    AgentPropertyElement typeValue;
    typeValue.value = QVariant( (int) getSproutType() );
    typeValue.readOnly = false;
    typeValue.type = AgentPropertyElement::SproutTypeSelector;
    typeValue.propertyName = "sprout_type";
    typeValue.backgroundColor = typeValue.widgetBackground();
    typeElement.append( typeValue );
    
    p.append( typeElement );
    
    // --------------------------------------------------------------------------------------------
    //                                     Signal name of this sprout. 
    //                                     Имя сигнала данного "ростка". 
    // --------------------------------------------------------------------------------------------
    
    QList< AgentPropertyElement > signalNameElement;
    signalNameElement.append( AgentPropertyElement::captionElement( tr("Signal name") ));
    
    AgentPropertyElement signalNameValue;
    signalNameValue.readOnly = true;
    signalNameValue.backgroundColor = signalNameValue.widgetBackground();
    signalNameValue.value = getSignalName();
    signalNameValue.propertyName = "signal_name";
    signalNameElement.append( signalNameValue);
    
    p.append( signalNameElement );
    
    // --------------------------------------------------------------------------------------------
    //                                   Minimal allowed value
    //                               Минимально допустимое значение. 
    // --------------------------------------------------------------------------------------------
    
    QList<AgentPropertyElement> minElement;
    minElement.append( AgentPropertyElement::captionElement( tr("Minimal value") ));
    
    AgentPropertyElement minValue;
    minValue.type = AgentPropertyElement::Float;
    minValue.readOnly = false;
    minValue.propertyName = "minimal_value";
    minValue.value = getMinimalValue();
    minElement.append( minValue );
    
    p.append( minElement );
    
    // --------------------------------------------------------------------------------------------
    //                                  Maximal allowed value 
    //                              Максимально допустимое значение.
    // --------------------------------------------------------------------------------------------
    
    QList<AgentPropertyElement> maxElement;
    maxElement.append( AgentPropertyElement::captionElement( tr("Maximal value")) );
    AgentPropertyElement maxValue;
    maxValue.value = getMaximalValue();
    maxValue.readOnly = false;
    maxValue.propertyName = "maximal_value";
    maxValue.type = AgentPropertyElement::Float;
    maxElement.append( maxValue );
    
    p.append( maxElement );
    
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

