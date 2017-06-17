// ********************************************************************************************************************
// *                                                                                                                  *
// *                            UML-like "schema", the graphics representation of model.                              *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                             UML-подобная "схема", графическое представление модели.                              *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 28 may 2017 at 15:48 *
// ********************************************************************************************************************

#include "SchemaView.h"

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                    The constructor.                                              *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                       Конструктор.                                               *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::SchemaView::SchemaView( QGraphicsScene * scene )
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
    
    // Tell the system that widget can receive Drag&Drop events.
    // Сообщает сисетме, что виджит может принимать события Drag&Drop.
    
    setAcceptDrops( true );
    
    __leftMouseButtonPressed = false;
    __entityDragged = nullptr;
    __entityDragInProcess = false;
    
    __createMenus();
    __mouseAtSchemaPos = QPoint( 0, 0 );
    
    // __scaleFactor = 1.0;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                               The menu's constructor.                                            *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                   Конструктор меню.                                              *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::SchemaView::__createMenus() {
    
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

void tengu::SchemaView::__on_action_create_task() {
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
void tengu::SchemaView::wheelEvent(QWheelEvent* event) {
    
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
        scale( 1.2, 1.2 );
    } else {
        scale( 0.8, 0.8 );
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

void tengu::SchemaView::mousePressEvent ( QMouseEvent * event ) {
    
    QGraphicsView::mousePressEvent( event );
    __entityDragged = nullptr;
    __entityDragInProcess = false;
    
    if ( event->buttons() & Qt::LeftButton ) {
        
        __leftMouseButtonPressed = true;
        __mousePressedPos = event->pos();
        
        QGraphicsItem * item = itemAt( event->pos());
        bool controlPressed = event->modifiers() & Qt::ControlModifier;
    
        if ( item ) {
            
            AbstractEntityItem * entityItem = dynamic_cast< AbstractEntityItem * >( item );
            if ( entityItem ) {
                __entityDragged = entityItem;                
            };
            
            emit signalItemPressed( ( AbstractEntityItem * ) item, controlPressed );
        };
        
    };    
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                              Mouse release event handler.                                        *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                          Обработчик события отпускания мыши.                                     *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::SchemaView::mouseReleaseEvent ( QMouseEvent * event ) {
    
    QGraphicsView::mouseReleaseEvent ( event );
    
    __entityDragged = nullptr;
    __entityDragInProcess = false;
    
    if ( Qt::LeftButton & event->buttons() ) {    
        __leftMouseButtonPressed = false;    
    };
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                          Mouse double click event handler.                                       *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                           Событие двойного щелчка мыши.                                          *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::SchemaView::mouseDoubleClickEvent ( QMouseEvent* event ) {
    QGraphicsView::mouseDoubleClickEvent ( event );
    QGraphicsItem * item = itemAt( event->pos() );
    bool controlPressed = event->modifiers() & Qt::ControlModifier;
    if ( item ) {
        emit signalItemDoubleClicked( (AbstractEntityItem * ) item, controlPressed );
    };
}


// ********************************************************************************************************************
// *                                                                                                                  *
// *                                              Mouse move event handler.                                           *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                         Обработчик события перемещения мыши.                                     *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::SchemaView::mouseMoveEvent ( QMouseEvent * event ) {
    // __scaleCenter = event->pos();
    // qDebug() << "SchemaView::mouseMove()";
    QGraphicsView::mouseMoveEvent ( event );
    __mouseAtSchemaPos = mapToScene( event->pos() ).toPoint();
    
    if ( event->buttons() & Qt::LeftButton ) {
        
        if ( __entityDragged ) {
            if ( ! __entityDragInProcess )  {
            
                // May be we need to switch on the drag process.
                // Может быть, необходимо включить перетаскивание.
            
                QPoint delta = event->pos() - __mousePressedPos;
                if ( delta.manhattanLength() > QApplication::startDragDistance() ) {
                    __entityDragInProcess = true;
                };
            
            };
        
            // If we are already in drag process, signal about it.
            // Если уже тащим, сигнал об этом.
        
            if ( __entityDragInProcess ) {
                QPoint targetPos = mapToScene( event->pos() ).toPoint(); 
                targetPos -= __entityDragged->mousePressedPos();
                // qDebug() << "Event pos=" << event->pos() << ", mouse pressed=" << __mousePressedPos << ", element pos=" << __entityDragged->mousePressedPos();
                emit signalItemDragging( __entityDragged, targetPos );
            };
            
        };
    };
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                              Context menu event handler                                          *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                         Обработчик событий контекстного меню.                                    *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::SchemaView::contextMenuEvent ( QContextMenuEvent* event ) {
    
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
// *               Convert QDropEvent (QDragMoveEvent, QDragEnterEvent) to my AbstractEntity object.                  *
// * ---------------------------------------------------------------------------------------------------------------- *
// *             Преобразовать QDropEvent( QDragMoveEvent, QDragEnterEvent) в мой AbstractEntity объект.              *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::AbstractStorageableEntity * tengu::SchemaView::__event_to_entity ( QDropEvent * event ) {
    
    if ( event->mimeData()->hasFormat( "application/json" ) ) {
        
        QJsonDocument doc = QJsonDocument::fromJson( event->mimeData()->data("application/json") );
        if ( ! doc.isEmpty() ) {
            if ( doc.isObject() ) {
                QJsonObject json = doc.object();
                if ( ! json.isEmpty() ) {
                    return ( AgentFactory::createEntity( json ) );
                };
            };
        };        
    };
    
    return nullptr;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                            Enter dragging to this widget                                         *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                      Событие входа таскаемого объекта на виджит.                                 *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::SchemaView::dragEnterEvent ( QDragEnterEvent * event ) {
    QGraphicsView::dragEnterEvent ( event );    
    AbstractStorageableEntity * entity = __event_to_entity( event );
    if ( entity ) {
        delete( entity );
        event->acceptProposedAction();
    };
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                 Drage move event.                                                *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                    Событие перемещения таскаемого объекта по виджиту.                            *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::SchemaView::dragMoveEvent ( QDragMoveEvent * event ) {
    QGraphicsView::dragMoveEvent ( event );
    AbstractStorageableEntity * entity = __event_to_entity ( event );
    if ( entity ) {
        delete( entity );
        event->acceptProposedAction();
    };
}


// ********************************************************************************************************************
// *                                                                                                                  *
// *                                            Dragging leave this widget.                                           *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                          Таскание покинуло этот виджит.                                          *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::SchemaView::dragLeaveEvent ( QDragLeaveEvent * event ) {
    QGraphicsView::dragLeaveEvent ( event );    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                    The drop event.                                               *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                Событие бросания объекта.                                         *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::SchemaView::dropEvent ( QDropEvent* event ) {
    
    QGraphicsView::dropEvent ( event );
    AbstractStorageableEntity * entity = __event_to_entity( event ); 
    
    if ( entity ) {
        emit signalWasDropped( entity );
    };    
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                    The destructor.                                               *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                       Деструктор                                                 *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::SchemaView::~SchemaView() {
}
