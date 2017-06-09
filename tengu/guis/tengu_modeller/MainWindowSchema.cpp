// ********************************************************************************************************************
// *                                                                                                                  *
// *                            UML-like "schema", the graphics representation of model.                              *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                             UML-подобная "схема", графическое представление модели.                              *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 28 may 2017 at 15:48 *
// ********************************************************************************************************************

#include "MainWindowSchema.h"

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                    The constructor.                                              *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                       Конструктор.                                               *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::MainWindowSchema::MainWindowSchema( QGraphicsScene * scene )
: QGraphicsView( scene )
{
    // This is not good idea.
    // Не очень хорошая идея.
    // setViewport(new QGLWidget( QGLFormat(QGL::SampleBuffers)) );
    
    // Scale center is center of view by default, will be changed by first mouse move event.
    // Центр масштабирования - это по умолчанию центр виджита. Будет изменен первым же движением мыши.
    
    __scaleCenter = QPoint( width() / 2, height() / 2 );
    
    // widget receives mouse move events even if no buttons are pressed.
    // Виджит принимает события от мыши даже если ни одна из кнопок не была нажата.
    
    setMouseTracking( true );
    __leftMouseButtonPressed = false;
    
    __createMenus();
    
    // __scaleFactor = 1.0;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                               The menu's constructor.                                            *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                   Конструктор меню.                                              *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::MainWindowSchema::__createMenus() {
    
    __contextMenu = new QMenu( this );
    
    __contextMenu__create = new QMenu( tr("Create") );
    __contextMenu__create->setIcon( QIcon( QPixmap(":bricks_16.png") ) );
    
    __actionCreateTask = new QAction( QIcon(QPixmap("page_gear_16.png")), QString("Task"), this );
    QObject::connect( __actionCreateTask, SIGNAL( triggered()), this, SLOT( __on_action_create_task() ) );
    __contextMenu__create->addAction( __actionCreateTask );
    
    __contextMenu->addMenu( __contextMenu__create );
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                             Slot: we want create a task.                                         *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                           Слот: мы хотим создать задачу.                                         *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::MainWindowSchema::__on_action_create_task() {
    qDebug() << "Мы хотим создать задачу";
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                               Mouse wheel event handler.                                         *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                          Обработчик события колесика мышки.                                      *
// *                                                                                                                  *
// ********************************************************************************************************************

#ifndef QT_NO_WHEELEVENT
void tengu::MainWindowSchema::wheelEvent(QWheelEvent* event) {
    
    // _view->centerOn(target_scene_pos);        
  
    // QPointF view_center = mapToScene( __scaleCenter );    
    // centerOn( view_center );
    
    // centerOn( 0.0, 0.0 );
    // scrollContentsBy( 100, 100 );
    
    
    // QPointF target_scene_pos = mapToScene( __scaleCenter );
    // QPointF delta_viewport_pos = __scaleCenter - QPointF( viewport()->width() / 2.0, viewport()->height() / 2.0);    
    // QPointF view_center = mapFromScene( target_scene_pos ) - delta_viewport_pos;
    // centerOn( mapToScene( view_center.toPoint()));
    
    if (event->delta() > 0) {
        scale( 0.8, 0.8 );
    } else {
        scale(1.2, 1.2 );
    }
    
    // centerOn( 1000.0, 1000.0 );
    
    // event->accept();
    
    QGraphicsView::wheelEvent( event );        
}
#endif

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                Mouse press event handler.                                        *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                             Обработчик события нажатия мыши.                                     *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::MainWindowSchema::mousePressEvent ( QMouseEvent * event ) {
    
    QGraphicsView::mousePressEvent( event );
    
    if ( event->buttons() & Qt::LeftButton ) {
        qDebug() << "MainWindowSchema::mousePressEvent()";
        __leftMouseButtonPressed = true;
    };
    
    QGraphicsItem * item = itemAt( event->pos());
    
    if ( item ) {
        emit signalItemPressed( ( AbstractAgentItem * ) item );
    };
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                              Mouse release event handler.                                        *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                          Обработчик события отпускания мыши.                                     *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::MainWindowSchema::mouseReleaseEvent ( QMouseEvent * event ) {
    
    if ( Qt::LeftButton & event->buttons() ) {    
        qDebug() << "MainWindowSchema::mouse release event";    
        __leftMouseButtonPressed = false;    
    };
    QGraphicsView::mouseReleaseEvent ( event );
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                              Mouse move event handler.                                           *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                         Обработчик события перемещения мыши.                                     *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::MainWindowSchema::mouseMoveEvent ( QMouseEvent * event ) {
    // __scaleCenter = event->pos();
    // qDebug() << "MainWindowSchema::mouseMove()";
    QGraphicsView::mouseMoveEvent ( event );
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                              Context menu event handler                                          *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                         Обработчик событий контекстного меню.                                    *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::MainWindowSchema::contextMenuEvent ( QContextMenuEvent* event ) {
    
    QGraphicsView::contextMenuEvent ( event );    
    
    qDebug() << "Context menu event handler";
    QGraphicsItem * item = itemAt( event->pos());
    
    if ( item ) {
        qDebug() << "have item under mouse";
        // __contextMenu__create->hide();
        // __contextMenu__create->setEnabled( false );
        
    } else {
        __contextMenu->exec( event->globalPos() );
        qDebug() << "Have NOT item, free";
        // __contextMenu__create->show();
        // __contextMenu__create->setEnabled( true );        
    };
}


// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                    The destructor.                                               *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                       Деструктор                                                 *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::MainWindowSchema::~MainWindowSchema() {
}
