// ********************************************************************************************************************
// *                                                                                                                  *
// *                               The graphics representation of tengu.core.Task class                               *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                 Графическое представление класса tengu.core.Task                                 *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 09 jun 2017 at 10:58 *
// ********************************************************************************************************************

#include "TaskItem.h"

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                   The constructor.                                               *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                     Конструктор.                                                 *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::TaskItem::TaskItem ( tengu::Task * task, QGraphicsItem * parent ) 
    : ItemWithLinks ( task , parent )
{
    _boundingRect = QRectF( 0.0, 0.0, 400.0, 144.0 );
    _class_name = "TaskItem";
    _canChangeExecuteMode = true;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                  The paint method.                                               *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                   Метод отрисовки.                                               *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::TaskItem::paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget ) {
    
    Q_UNUSED( option );
    Q_UNUSED( widget );
    
    _storePainterSettings( painter );
    
    QPixmap pixmap( _boundingRect.width() + 1, _boundingRect.height() + 1 );
    QPainter p ( & pixmap );    
    
    // p.setBrush( QColor(0, 0, 0, 255) );
    // p.eraseRect( 0, 0, _boundingRect.width(), _boundingRect.height() );
    
    _drawTaskRectangle( & p );
    
    // Execution mode picture
    // Картинка режима выполнения.
    
    QPixmap pm = _executionModePixmap();
    if ( getExecutionMode() == AbstractAgent::EM_XPLANE ) p.drawPixmap( _boundingRect.width() - 42, 6, pm );
    else p.drawPixmap( _boundingRect.width() - 36, 6, pm );
    
    // Name of this task.
    // Имя данной задачи.
    
    QString name = getHumanName();
    if ( ! name.isEmpty() ) {
        
        int pixels = 22;
        QFont font("Tahoma", pixels );
        font.setBold( true );
        QFontMetrics mcs( font );
        while ( (pixels > 0 ) && ( mcs.width( name ) >= _boundingRect.width() ) ) {
            pixels --;
            font.setPixelSize( pixels );
            mcs = QFontMetrics( font );
        };
        p.setFont( font );
        int x = ( _boundingRect.width() - mcs.width( name ) ) / 2;
        int y = mcs.height() + ( _boundingRect.height() - mcs.height() ) / 2; 
        p.drawText( QPoint(x,y), name );
    };
    
    painter->drawPixmap( 0, 0, pixmap );
    
    _restorePainterSettings( painter );
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                            Check the entity is valid.                                            *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                      Проверяем, валидна ли ссылка на entity                                      *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::TaskItem::checkEntity() {
    if ( ! _entity ) _entity = new Task();
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                          Get pointer to the represented task.                                    *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                      Получить указатель на представляемую задачу.                                *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::Task * tengu::TaskItem::task() {
    return ( dynamic_cast < Task * > ( _entity ) );
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                       JSON representation of this object.                                        *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                    Представление данного объекта в виде JSON                                     *
// *                                                                                                                  *
// ********************************************************************************************************************
/*
QJsonObject tengu::TaskItem::toJSON() {
    
    QJsonObject o = AbstractEntityItem::toJSON();
    o["class_name"] = "TaskItem";
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
bool tengu::TaskItem::fromJSON ( QJsonObject json ) {
    
    if ( json.contains("class_name") ) {
        
        QString className = json.value("class_name").toString();
        
        if ( className == "TaskItem" ) {
            return tengu::AbstractEntityItem::fromJSON ( json );
        };
    }
    return false;
}
*/
// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                   The destructor.                                                *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                      Деструктор.                                                 *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::TaskItem::~TaskItem() {

}


