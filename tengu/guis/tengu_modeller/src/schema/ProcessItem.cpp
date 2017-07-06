// ********************************************************************************************************************
// *                                                                                                                  *
// *                                       The graphical representation for the process.                              *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                             Графическое представление процесса.                                  *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 17 jun 2017 at 15:36 *
// ********************************************************************************************************************

#include "ProcessItem.h"

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                    The constructor.                                              *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                       Конструктор.                                               *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::ProcessItem::ProcessItem ( tengu::AbstractEntity * entity, QGraphicsItem * parent ) 
    : ItemWithLinks ( entity, parent )
{
    _boundingRect = QRectF( 0.0, 0.0, 440.0, 164.0 );
    _className = "ProcessItem";
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                Check core process entity                                         *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                          Проверка "сущности" корневого процесса.                                 *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::ProcessItem::checkEntity() {
    if ( ! _entity ) _entity = new Process();
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                  Painting the process.                                           *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                   Рисование процесса.                                            *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::ProcessItem::paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget ) {
    
    Q_UNUSED( option );    
    Q_UNUSED( widget );
    
    _storePainterSettings( painter );
    
    QPixmap pixmap( _boundingRect.width(), _boundingRect.height() );
    QPainter p ( & pixmap );    
    
    p.setBrush( QColor(0, 0, 0, 255) );
    p.eraseRect( 0, 0, _boundingRect.width(), _boundingRect.height() );
    
    int size3d = 18;
    
    // Filling the body of process.
    // Заливка тела процесса.
    
    QLinearGradient gradient( _boundingRect.topLeft(), QPoint( 0, _boundingRect.height() - size3d) );
    gradient.setColorAt(0, _processDiagram_brightFillColor() );
    gradient.setColorAt(1, _processDiagram_darkFillColor() );
    gradient.setStart( 1, _boundingRect.height() / 3 );
    QRect r( 1, 1, _boundingRect.width() - size3d, _boundingRect.height() - size3d );
    p.fillRect( r, gradient );
    
    // Filling right 3d wall
    // Заполнение правой стенки 3d.
    
    QColor cl = _processDiagram_darkFillColor();
    cl = QColor( _darker( cl.red() ), _darker( cl.green() ), _darker( cl.blue() ) );
    p.setBrush( QBrush( cl ) );
    QPointF pr[4] = {
        QPointF( _boundingRect.width() - size3d, 1 ),
        QPointF( _boundingRect.width() - 1, size3d ),
        QPointF( _boundingRect.width() - 1, _boundingRect.height() - 1 ),
        QPointF( _boundingRect.width() - size3d, _boundingRect.height() - size3d )
    };
    p.drawPolygon( pr, 4 );
    
    // Filling bottom 3d wall.
    // Заполнение нижней стенки 3d.
    
    cl = QColor( _darker( cl.red() ), _darker( cl.green() ), _darker( cl.blue() ) );
    p.setBrush( QBrush( cl ) );
    QPointF pb[4] = {
        QPoint( 1, _boundingRect.height() - size3d ),
        QPoint( size3d, _boundingRect.height() - 1 ),
        QPoint( _boundingRect.width() - 1, _boundingRect.height() - 1 ),
        QPoint( _boundingRect.width() - size3d, _boundingRect.height() - size3d )
    };
    p.drawPolygon( pb, 4 );
        
    // Borders
    // Границы.
    
    p.setPen( _processDiagram_borderPen() );
    p.drawLine( 1, 1, _boundingRect.width() - size3d, 1 );
    p.drawLine( _boundingRect.width() - size3d, 1, _boundingRect.width() - size3d, _boundingRect.height() - size3d );
    p.drawLine( 1, 1, 1, _boundingRect.height() - size3d );
    p.drawLine( 1, _boundingRect.height() - size3d, _boundingRect.width() - size3d, _boundingRect.height() - size3d );
    
    p.drawLine( _boundingRect.width() - size3d, 1, _boundingRect.width() - 1, size3d );
    p.drawLine( _boundingRect.width() - 1, size3d, _boundingRect.width() -1, _boundingRect.height() - 1 );
    p.drawLine( _boundingRect.width() - size3d, _boundingRect.height() - size3d, _boundingRect.width() - 1, _boundingRect.height() - 1 );
    p.drawLine( size3d, _boundingRect.height() - 1, _boundingRect.width() - 1, _boundingRect.height() - 1 );
    p.drawLine( 1, _boundingRect.height() - size3d, size3d, _boundingRect.height() - 1 );
        
    /*
    _drawTaskRectangle( & p );
    
    // Execution mode picture
    // Картинка режима выполнения.
    
    QPixmap pm = _executionModePixmap();
    if ( getExecutionMode() == AbstractAgent::EM_XPLANE ) p.drawPixmap( _boundingRect.width() - 42, 6, pm );
    else p.drawPixmap( _boundingRect.width() - 36, 6, pm );
    
    */
    
    // Name of this process.
    // Имя данного процесса.
    
    p.setPen( QPen( _processDiagram_borderColor() ) );
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
        int y = mcs.height() + ( _boundingRect.height() - ( size3d ) - mcs.height() ) / 2; 
        p.drawText( QPoint(x,y), name );
    };
    
    
    // The pictures for process success or failure end
    // Картинки успешности или провала процесса.
    
    int y = qRound( ( _boundingRect.height() / 2 ) - size3d );
    int x = _boundingRect.width() - size3d - 30;
    p.setCompositionMode( QPainter::CompositionMode_Multiply );
    p.drawPixmap( x, y-32, QPixmap( ":accept_button_32.png" ) );
    p.drawPixmap( x, y+32, QPixmap( ":cancel_32.png" ) );

    painter->drawPixmap( 0, 0, pixmap );
    
    _restorePainterSettings( painter );

}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                    The destructor.                                               *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                       Деструктор.                                                *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::ProcessItem::~ProcessItem() {

}

