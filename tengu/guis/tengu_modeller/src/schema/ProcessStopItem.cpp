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
    : ItemWithLinks ( entity, parent )
{
    // 3 точки граница + 2 точки между кругом и границей.
    int borderWidth = 3;
    int spaceWidth = 2;
    
    __radius = 38 - borderWidth*2 - spaceWidth*2;
    
    int rectSize = __radius * 2 + borderWidth * 2 + spaceWidth * 2;
    
    _boundingRect = QRect( 0,0, rectSize, rectSize );
    
    _class_name = "ProcessStopItem";
    _entity_type = ET_ProcessStopItem;
    _canChangeExecuteMode = true;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                The painting method.                                              *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                  Метод рисования.                                                *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::ProcessStopItem::paint ( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget ) {
    
    _storePainterSettings( painter );
    QPixmap pixmap( _boundingRect.width(), _boundingRect.height() );
    QPainter p( & pixmap );
    p.setBrush( QColor( 0, 0, 0, 255 ) );
    p.eraseRect( 0, 0, _boundingRect.width(), _boundingRect.height() );
    
    
    QPen borderPen = _processDiagram_borderPen();
    p.setPen( borderPen );
    
    QPoint center( __radius + 5, __radius + 5 );
    int focalShift = __radius + 5 - __radius / 3;
    QPoint focal( focalShift, focalShift );
    
    QRadialGradient gradient( center, __radius, focal );
    gradient.setColorAt(0, _processDiagram_brightFillColor() );
    gradient.setColorAt(1, _processDiagram_darkFillColor() );
    QBrush brush = QBrush( gradient );    
    p.setBrush( brush );
    p.drawEllipse( 6, 6, _boundingRect.width() - 12, _boundingRect.height() - 12 );
    
    // Circle around element
    // Окружность вокруг элемента.
    
    QRect r( 1,1, _boundingRect.width() - 2, _boundingRect.height() - 2 );
    p.drawArc( r, 0, 16*360 );       
    
    // Execute mode icon.
    // Картинка режима выполнения.
    
    QPixmap pm = _executionModePixmap();
    p.drawPixmap( _boundingRect.width() - 34, 1, pm );
    
    painter->drawPixmap( 0, 0, pixmap );
    _restorePainterSettings( painter );
    
    
    /*
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
    */    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                            Check the entity is valid.                                            *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                      Проверяем, валидна ли ссылка на entity                                      *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::ProcessStopItem::checkEntity() {
    if ( ! _entity ) _entity = new ProcessStop();
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                       JSON representation of this object.                                        *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                    Представление данного объекта в виде JSON                                     *
// *                                                                                                                  *
// ********************************************************************************************************************
/*
QJsonObject tengu::ProcessStopItem::toJSON() {
    
    QJsonObject o = AbstractEntityItem::toJSON();
    o["class_name"] = "ProcessStopItem";
    return o;
    
}
*/
// ********************************************************************************************************************
// *                                                                                                                  *
// *                                        Conversion from JSON to this object.                                      *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                          Преобразование из JSON'а в объект.                                      *
// *                                                                                                                  *
// ********************************************************************************************************************
/*
bool tengu::ProcessStopItem::fromJSON ( QJsonObject json ) {
    
    if ( json.contains("class_name") ) {
        
        QString className = json.value("class_name").toString();
        
        if ( className == "ProcessStopItem" ) {
            return tengu::AbstractEntityItem::fromJSON ( json );
        };
    }
    return false;
}
*/
// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                  The destructor.                                                 *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                    Деструктор.                                                   *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::ProcessStopItem::~ProcessStopItem() {

}
