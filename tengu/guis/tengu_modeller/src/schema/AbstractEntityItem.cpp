// ********************************************************************************************************************
// *                                                                                                                  *
// *                              Abstract graphics object, representation of AbstractEntity.                         *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                            Абстрактный графический объект, представление AbstractEntity.                         *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 08 jun 2017 at 09:00 *
// ********************************************************************************************************************

#include "AbstractEntityItem.h"

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                  The constructor.                                                *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                    Конструктор.                                                  *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::AbstractEntityItem::AbstractEntityItem( AbstractEntity * entity, QGraphicsItem * parent ) 
    : QGraphicsObject( parent )
    , AbstractEntity()
{
    _entity = entity;
    _boundingRect = QRectF(0, 0, 20, 20);
    _selected = false;
    _decomposite = false;
    _changed = false;
    
    __mousePressed = false;        
    __mousePressedPoint = QPoint( 0, 0 );
    __uuid = QUuid::createUuid().toString();
    __lastModified = QDateTime::currentDateTimeUtc();
    
    // setAcceptHoverEvents( true );
    // setAcceptTouchEvents( true );
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                   Overrided function for bounding the rectangle.                                 *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                  Перекрытая функция для привязки прямоугольника.                                 *
// *                                                                                                                  *
// ********************************************************************************************************************

QRectF tengu::AbstractEntityItem::boundingRect() const {
    return _boundingRect;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                    Paint function for abstract graphics agent (only for demonstrate it is empty)                 *
// * ---------------------------------------------------------------------------------------------------------------- *
// *             Функция рисования абстрактного графического агента (только чтобы показать, что он пустой)            *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractEntityItem::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget) {
    painter->drawRect(  _boundingRect );
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                      Return agent this graphics item based on.                                   *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                        Вернуть агента, на котором основывается данный графический элемент.                       *
// *                                                                                                                  *
// ********************************************************************************************************************
/*
tengu::AbstractEntity * tengu::AbstractEntityItem::entity() {
    return _entity;
}
*/
// ********************************************************************************************************************
// *                                                                                                                  *
// *                                              Is this agent selected?                                             *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                    Является ли данный агент выбранным (отмеченным)?                              *
// *                                                                                                                  *
// ********************************************************************************************************************

bool tengu::AbstractEntityItem::isSelected() {
    return _selected;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                              Set this agent selection.                                           *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                        Установка выбранности данного агента.                                     *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractEntityItem::setSelected ( bool selection ) {
    _selected = selection;
    update();
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                    Make R-G-B constituent of color to some brighter.                             *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                     Сделать R-G-B составляющую цвета чуть поярче.                                *
// *                                                                                                                  *
// ********************************************************************************************************************

int tengu::AbstractEntityItem::_brighter ( int color ) {
    color = qRound( color * 1.4 );
    if ( color > 255 ) color = 255;
    return color;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                         Get border's color for process diagrams.                                 *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                       Получить цвет границ для диаграмм процессов.                               *
// *                                                                                                                  *
// ********************************************************************************************************************

QColor tengu::AbstractEntityItem::_processDiagram_borderColor() {
    
    QColor bgColor = ORDINAR_BORDER_COLOR;
    if ( _selected ) bgColor = ORDINAR_SELECTED_BORDER_COLOR;
    
    /*
    if ( entity()->isFocused() ) {
        if ( _selected ) bgColor = FOCUSED_SELECTED_BORDER_COLOR;
        else bgColor = FOCUSED_BORDER_COLOR;
    };
    
    if ( entity()->isActive() ) {
        if ( _selected ) bgColor = ACTIVE_SELECTED_BORDER_COLOR;
        else bgColor = ACTIVE_BORDER_COLOR;
    };
    */
    
    return bgColor;
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                   Get shadow's color for processes diagrams.                                     *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                   Получить цвет теней для диаграмм процессов.                                    *
// *                                                                                                                  *
// ********************************************************************************************************************

QColor tengu::AbstractEntityItem::_processDiagram_shadowColor() {
    QColor c = QColor( 240, 240, 240 );
    if ( _selected ) c = QColor( 220, 220, 220 );
    return c;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                     Get "dark" fill color for process diagrams.                                  *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                               Получить "темный" цвет заполнения для диаграмм процессов.                          *
// *                                                                                                                  *
// ********************************************************************************************************************

QColor tengu::AbstractEntityItem::_processDiagram_darkFillColor() {
    
    QColor c = ORDINAR_FILL_COLOR;
    if ( _selected ) c = ORDINAR_SELECTED_FILL_COLOR;
 
    /*
    if ( agent()->isFocused() ) {
        if ( _selected ) c = FOCUSED_SELECTED_FILL_COLOR;
        else c = FOCUSED_FILL_COLOR;
    };
    
    if ( agent()->isActive() ) {
        if ( _selected ) c = ACTIVE_SELECTED_FILL_COLOR;
        else c = ACTIVE_FILL_COLOR;
    };
    */
    
    return c;
    
}


// ********************************************************************************************************************
// *                                                                                                                  *
// *                                   Get "bright" fill color for process'es diagrams.                               *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                Получить "яркий" цвет заполнения для диаграмм процессов.                          *
// *                                                                                                                  *
// ********************************************************************************************************************

QColor tengu::AbstractEntityItem::_processDiagram_brightFillColor() {
    
    QColor d = _processDiagram_darkFillColor();
    int r = _brighter( d.red() );
    int g = _brighter( d.green() );
    int b = _brighter( d.blue() );
    return QColor( r, g, b );
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                       Get the "pen" for painting borders of elements of process diagram.                         *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                   Получить "карандаш" для рисования границ элементов на диаграммах процессов.                    *
// *                                                                                                                  * 
// ********************************************************************************************************************

QPen tengu::AbstractEntityItem::_processDiagram_borderPen() {
    QPen pen;
    pen.setWidth( 3 );
    pen.setStyle( Qt::SolidLine );
    pen.setColor( _processDiagram_borderColor() );
    return pen;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                 Store old painter's settings to later restoring.                                 *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                     Сохранение старых установок painter'а для последующего восстановления.                       *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractEntityItem::_storePainterSettings ( QPainter * painter ) {
    
    __oldPainterPen = painter->pen();
    __oldPainterBrush = painter->brush();
    __oldPainterFont = painter->font();
    __oldPainterCompositionMode = painter->compositionMode();
    painter->setCompositionMode( QPainter::CompositionMode_Multiply );
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                             Restore old painter settings.                                        *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                       Восстановление старых установок painter'а.                                 *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractEntityItem::_restorePainterSettings ( QPainter * painter ) {
    
    painter->setPen( __oldPainterPen );
    painter->setBrush( __oldPainterBrush );
    painter->setFont( __oldPainterFont );
    painter->setCompositionMode( __oldPainterCompositionMode );
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                         Draw border around bounding rectangle.                                   *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                Нарисовать границы вокруг обрамляющего прямоугольника.                            *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractEntityItem::_drawBorderRect ( QPainter* painter ) {
    
    painter->setPen( _processDiagram_borderPen() );
    painter->drawRect( 1, 1, _boundingRect.width() - 2, _boundingRect.height() - 2 );
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                   Draw color rectangle like "the task inside of the process" representation.                     *
// * ---------------------------------------------------------------------------------------------------------------- *
// *              Нарисовать цветной прямоугольник, подобный представлению "задаче внутри процесса"                   *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractEntityItem::_drawTaskRectangle ( QPainter * painter ) {
    
    QLinearGradient gradient( _boundingRect.topLeft(), _boundingRect.bottomLeft() );
    gradient.setColorAt(0, _processDiagram_brightFillColor() );
    gradient.setColorAt(1, _processDiagram_darkFillColor() );
    gradient.setStart( 1, _boundingRect.height() / 3 );
    painter->fillRect( _boundingRect, gradient );
    
    _drawBorderRect( painter );
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                             Get appropriate pixmap for execution mode of this element.                           *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                      Вернуть картинку, соответствующую режиму выполнения данного элемента.                       *
// *                                                                                                                  *
// ********************************************************************************************************************

QPixmap tengu::AbstractEntityItem::_executionModePixmap( bool forSize32 ) {
    
    execution_mode_t em = getExecutionMode();
    switch ( em ) {
        case EM_ALWAYS: if ( forSize32 ) return QPixmap(":attach_32.png"); else return QPixmap(":attach_16.png");
        case EM_REAL:   if ( forSize32 ) return QPixmap(":cog_32.png"); else return QPixmap(":cog_16.png");
        case EM_XPLANE: if ( forSize32 ) return QPixmap(":xplane10_32.png"); else return QPixmap(":xplane10_16.png");
    };
    return QPixmap();
}


// ********************************************************************************************************************
// *                                                                                                                  *
// *                                              Something has been changed.                                         *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                  Что-то изменилось.                                              *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractEntityItem::_somethingChanged() {
    AbstractEntity::_somethingChanged();    
    emit signalSomethingChanged();
    update();
}


// ********************************************************************************************************************
// *                                                                                                                  *
// *                                               Mouse press event handler.                                         *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                            Обработчик события нажатия мышки.                                     *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractEntityItem::mousePressEvent ( QGraphicsSceneMouseEvent * event ) {
    
    QGraphicsItem::mousePressEvent ( event );
    
    if ( event->buttons() & Qt::LeftButton ) {
        __mousePressed = true;
        __mousePressedPoint = event->pos().toPoint();
    }
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                     Get mouse position where press event occured.                                *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                   Получить позицию мышки, на которой было нажатие.                               *
// *                                                                                                                  *
// ********************************************************************************************************************

QPoint tengu::AbstractEntityItem::mousePressedPos() {
    return __mousePressedPoint;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                             Mouse release event handler.                                         *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                        Обработчик события отпускания мыши.                                       *
// *                                                                                                                  *
// ********************************************************************************************************************
/*
void tengu::AbstractEntityItem::mouseReleaseEvent ( QMouseEvent * event ) {
    qDebug() << "AbstractAgentItem::mouseReleaseEvent()" ;
    __mousePressed = false;
    // QGraphicsItem::mouseReleaseEvent ( event );
}
*/
// ********************************************************************************************************************
// *                                                                                                                  *
// *                                             Mouse move event handler.                                            *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                         Обработчик события перемещения мыши.                                     *
// *                                                                                                                  *
// ********************************************************************************************************************
/*
void tengu::AbstractAgentItem::mouseMoveEvent ( QGraphicsSceneMouseEvent * event ) {
    qDebug() << "AbstractAgentItem::mouseMoveEvent()";
    QGraphicsItem::mouseMoveEvent ( event );
}
*/
// ********************************************************************************************************************
// *                                                                                                                  *
// *                                   Return properties list for this abstract agent.                                *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                       Вернуть список свойств данного агента.                                     *
// *                                                                                                                  *
// ********************************************************************************************************************

QList< QList< tengu::AgentPropertyElement > > tengu::AbstractEntityItem::properties() {

    QList< QList<AgentPropertyElement> > result;
    
    // UUID
    
    QList<AgentPropertyElement > uuidElement;    
    uuidElement.append( AgentPropertyElement::captionElement("UUID") );    
    AgentPropertyElement uuidValue( getUUID() );
    uuidValue.backgroundColor = uuidValue.disabledBackground();
    uuidElement.append( uuidValue );    
    result.append( uuidElement );
    
    // System Name
    
    QList< AgentPropertyElement > systemNameElement;    
    systemNameElement.append( AgentPropertyElement::captionElement(tr("System name")) );    
    AgentPropertyElement systemNameValue( getSystemName() );
    systemNameValue.readOnly = false;
    systemNameValue.propertyName = QString("system_name");
    systemNameElement.append( systemNameValue );    
    result.append( systemNameElement );
    
    // Human name
    
    QList<AgentPropertyElement> humanNameElement;
    humanNameElement.append( AgentPropertyElement::captionElement( tr("Human name") ));
    AgentPropertyElement humanNameValue( getHumanName() );
    humanNameValue.readOnly = false;
    humanNameValue.propertyName = "human_name";
    humanNameElement.append( humanNameValue );    
    result.append( humanNameElement );
    
    // Comment
    
    QList < AgentPropertyElement > commentElement;    
    commentElement.append( AgentPropertyElement::captionElement( tr( "Comment" ) ) );    
    AgentPropertyElement commentValue( getComment() );
    commentValue.readOnly = false;
    commentValue.propertyName = QString("comment");
    commentElement.append( commentValue );    
    result.append( commentElement );
    
    // Agent execution mode selector. This is an combo-box.
    // Выбор режима выполнения данного агента. Это - комбо-бокс.
    
    QList<AgentPropertyElement> executionModeElement;    
    executionModeElement.append( AgentPropertyElement::captionElement(  tr("Execution mode") ) );    
    AgentPropertyElement executionModeValue;
    executionModeValue.type = AgentPropertyElement::ExecutionModeSelector;
    executionModeValue.readOnly = false;
    executionModeValue.propertyName = "execution_mode";
    executionModeValue.value = QVariant( (int) getExecutionMode() );
    executionModeValue.backgroundColor = executionModeValue.widgetBackground();
    
    // QComboBox * cb = new QComboBox();
    /*
    cb->addItem( tr("Always"), QVariant( AbstractAgent::EM_ALWAYS ) );
    cb->addItem( tr("Real"), QVariant( AbstractAgent::EM_REAL ) );
    cb->addItem( tr("X-Plane"), QVariant( AbstractAgent::EM_XPLANE ) );
    */
    // executionModeValue.editorWidget = cb; 
    
    
    executionModeElement.append( executionModeValue );
    
    result.append( executionModeElement );
        
    return result;
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                  Is this entity the subject for decomposition?                                   *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                   Является ли данная сущность декомпозируемой?                                   *
// *                                                                                                                  *
// ********************************************************************************************************************

bool tengu::AbstractEntityItem::isDecomposite() {
    
    // "Decomposite" mean either forcibly decomposition or this base is exactly an agent and he has children.
    // "Декомпозиция" означает либо принудительную декомпозицию, либо в основании лежит точно агент и он имеет детей.
    
    AbstractAgent * agent = dynamic_cast<AbstractAgent * >( _entity );
    bool agentHasChildren = false;
    if ( agent ) agentHasChildren = agent->hasChildren();
    return ( _decomposite || agentHasChildren  );
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *       Get UUID of this object. This is not the same as uuid of entity this object was based on.                  *
// * ---------------------------------------------------------------------------------------------------------------- *
// *     Вернуть UUID данного объекта. Это не то же самое, что UUID сущности, на которой данный объект базируется.    *
// *                                                                                                                  *
// ********************************************************************************************************************

QString tengu::AbstractEntityItem::getUUID() {
    return __uuid;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                  Return the name, wrapper for the entity object.                                 *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                    Вернуть имя, обертка для объекта-сущности.                                    *
// *                                                                                                                  *
// ********************************************************************************************************************

QString tengu::AbstractEntityItem::getSystemName() {
    
    if ( _entity ) return _entity->getSystemName();
    return QString("");
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                     Set the name. Wrapper for entity object.                                     *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                   Установить имя. Обертка для объекта-сущности.                                  *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractEntityItem::setSystemName ( QString name ) {
    
    if ( _entity ) {
        _entity->setSystemName( name );
        _somethingChanged();
    };
        
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                 Get human name                                                   *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                           Получить "человеческое" имя.                                           *
// *                                                                                                                  *
// ********************************************************************************************************************

QString tengu::AbstractEntityItem::getHumanName() {
    if ( _entity ) return _entity->getHumanName();
    return QString ("");    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                  Set human name                                                  *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                           Установить человеческое имя.                                           *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractEntityItem::setHumanName ( QString name ) {
    if ( _entity ) {
        _entity->setHumanName( name );
        _somethingChanged();
    };
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                Return comment. This is only wrapper for entity object.                           *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                             Вернуть комментарий. Просто обертка для объекта сущности.                            *
// *                                                                                                                  *
// ********************************************************************************************************************

QString tengu::AbstractEntityItem::getComment() {
    
    if ( _entity ) return _entity->getComment();    
    return QString("");
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                 Set comment, wrapper for the entity object.                                      *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                              Установить комментарий, обертка для объекта сущности.                               *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractEntityItem::setComment ( QString comment ) {
    if ( _entity ) {
        _entity->setComment( comment );
        _somethingChanged();
    };
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                  Execution mode is the same as for entity object.                                *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                          Режим выполнения - тот же самый, что и у объекта-сущности.                              *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::AbstractEntity::execution_mode_t tengu::AbstractEntityItem::getExecutionMode() {
    
    if ( _entity ) return _entity->getExecutionMode();
    return EM_ALWAYS;
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                               Set execution mode, the wrapper for the entity object.                             *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                           Установка режима выполнения, обертка для объекта-сущности.                             *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractEntityItem::setExecutionMode ( tengu::AbstractEntity::execution_mode_t mode ) {
        
    if ( _entity ) {
        _entity->setExecutionMode( mode );
        _somethingChanged();
    };
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                             Has been this object changed?                                        *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                             Был ли данный объект изменен?                                        *
// *                                                                                                                  *
// ********************************************************************************************************************

bool tengu::AbstractEntityItem::hasChanged() {
    if ( _entity ) return ( _entity->hasChanged() || _changed );
    return _changed;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                     The latest modification time of this object.                                 *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                         Последнее время модификации объекта.                                     *
// *                                                                                                                  *
// ********************************************************************************************************************

QDateTime tengu::AbstractEntityItem::lastModified() {
    
    if ( _entity ) {
        if ( _entity->lastModified() > __lastModified ) return _entity->lastModified();
        return __lastModified;
    };
    
    return __lastModified;
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                    Return entity pointer this element based on.                                  *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                       Вернуть указатель на ссылку, на основании которой создан этот элемент.                     *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::AbstractEntity * tengu::AbstractEntityItem::entity() {
    return _entity;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                           Conversion from object to json                                         *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                           Преобразование из объекта в JSON.                                      *
// *                                                                                                                  *
// ********************************************************************************************************************

QJsonObject tengu::AbstractEntityItem::toJSON() {
    
    QJsonObject o = AbstractEntity::toJSON();
    
    if ( _entity ) {
        QJsonObject e = _entity->toJSON();
        o["entity"] = e;                
    };
           
    return o;
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                            Conversion from JSON to object.                                       *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                           Преобразование из JSONа в объект.                                      *
// *                                                                                                                  *
// ********************************************************************************************************************

bool tengu::AbstractEntityItem::fromJSON ( QJsonObject json ) {

    bool result = AbstractEntity::fromJSON( json );
    
    if ( ( result ) && ( json.contains("entity") ) ) {
        
        QJsonValue vEntity = json["entity"];
                
        if ( vEntity.isObject() ) {
            QJsonObject oEntity = vEntity.toObject();
            qDebug() << "Have entity:" << oEntity;
        
            if ( ! _entity ) _entity = AgentFactory::createEntity( oEntity );
            else _entity->fromJSON( oEntity );
        }
        
        
    };
    
    return result;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                 Recalculate coordinates for this element visualization.                          *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                             Пересчитать координаты визуального отображения данного элемента.                     *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractEntityItem::recalculate() {
    
    // This method is empty for AbstractEntityItem
    // Данный метод пустой для класса AbstractEntityItem
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                Check item's entity.                                              *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                       Проверка "сущности" для данного элемента.                                  *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AbstractEntityItem::checkEntity() {

}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                   The destructor.                                                *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                      Деструктор.                                                 *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::AbstractEntityItem::~AbstractEntityItem() {
        
    if ( _entity ) {
        delete ( _entity );
        _entity = nullptr;
    };        
    
}


