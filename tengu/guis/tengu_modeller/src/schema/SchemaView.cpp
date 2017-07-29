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
    
    setUpdatesEnabled( true );
    
    __leftMouseButtonPressed = false;
    __entityDragged = nullptr;
    __entityDragInProcess = false;
    
    __createMenus();
    __mouseAtSchemaPos = QPoint( 0, 0 );
    __contextMenuItem = nullptr;
    
    semiCreatedLink = nullptr;
    
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
    
    // __contextMenu = new QMenu( this );
    
    __contextMenu__create = new QMenu( tr("Create") );
    __contextMenu__create->setIcon( QIcon( QPixmap(":bricks_16.png") ) );
    
    __action__create_task = new QAction( QIcon(QPixmap(":box_16.png")), tr("Task"), this );
    QObject::connect( __action__create_task, SIGNAL( triggered()), this, SLOT( __on__action__create_task() ) );
    __contextMenu__create->addAction( __action__create_task );
    
    __action__delete_item = new QAction( QIcon( QPixmap(":chart_organisation_delete_16.png") ), tr("Delete"), this );
    QObject::connect( __action__delete_item, SIGNAL( triggered() ), this, SLOT( __on__action__delete_item() ) );
    
    // __contextMenu->addMenu( __contextMenu__create );
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                            Delete item from schema.                                              *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                           Удалить элемент со схемы.                                              *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::SchemaView::__on__action__delete_item() {
    
    if ( __contextMenuItem ) {
        
        AbstractEntity * entity = __contextMenuItem->entity();
        if ( entity ) {
            emit signalWantDelete( entity );
        };
        
        /*
        hide();
        ItemWithLinks * itemWithLinks = dynamic_cast< ItemWithLinks * > ( __contextMenuItem );
        if ( itemWithLinks ) {
            
            // Links of this item. Will be removed simultaneously with him.
            // Связи данного элемента - будут удалены совместно с ним.
             
            QList< LinkItem *> links = itemWithLinks->hisLinks();
            for ( int i=0; i<links.size(); i++ ) {                
                scene()->removeItem( links.at(i) );
                delete( links.at(i) );                
            };
        
        }
        
        scene()->removeItem( __contextMenuItem );
        delete( __contextMenuItem );
        __contextMenuItem = nullptr;        
        show();
        */
        
    };
}


// ********************************************************************************************************************
// *                                                                                                                  *
// *               Convert QDropEvent (QDragMoveEvent, QDragEnterEvent) to my AbstractEntity object.                  *
// * ---------------------------------------------------------------------------------------------------------------- *
// *             Преобразовать QDropEvent( QDragMoveEvent, QDragEnterEvent) в мой AbstractEntity объект.              *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::AbstractEntity * tengu::SchemaView::__event_to_entity ( QDropEvent * event ) {
    
    if ( event->mimeData()->hasFormat( "application/json" ) ) {
        
        QJsonDocument doc = QJsonDocument::fromJson( event->mimeData()->data("application/json") );
        if ( ! doc.isEmpty() ) {
            if ( doc.isObject() ) {
                QJsonObject json = doc.object();
                if ( ! json.isEmpty() ) {
                    return ( AgentItemFactory::createEntity( json ) );
                };
            };
        };        
    };
    
    return nullptr;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                             Slot: we want create a task.                                         *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                           Слот: мы хотим создать задачу.                                         *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::SchemaView::__on__action__create_task() {
    SchemaScene * sc = dynamic_cast<SchemaScene * >( scene() );
    if ( sc ) {
        AbstractAgent * rootAgent = dynamic_cast<AbstractAgent * >( sc->rootEntity() );
        if ( rootAgent ) {
            
            emit signalWantCreateAgent( rootAgent, AbstractEntity::ET_Task );
            
        } else qDebug() << "SchemaView::__on_action_create_task, root schema agent is empty";
    } else qDebug() << "SchemaView::__on_action_create_task, schema scene is empty.";
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
        
        QList<QGraphicsItem * > itemsList = items( event->pos() );
        AbstractEntityItem * entity = dynamic_cast<AbstractEntityItem *>( itemsList.at(0) );
        ItemWithLinks * itemWithLinks = dynamic_cast< ItemWithLinks * >( entity );      
        
        // Wi try to find out first not-link element.
        // Ищем первую не-связь.
        
        if ( ! itemWithLinks ) {
            for ( int i=1; i<itemsList.count(); i++ ) {
                itemWithLinks = dynamic_cast< ItemWithLinks * > ( itemsList.at(i) );
                if ( itemWithLinks ) break;
            };
        };
        
        // QGraphicsItem * item = itemAt( event->pos());
        
        bool controlPressed = event->modifiers() & Qt::ControlModifier;
        
        if ( itemsList.count() > 0 ) {
                                    
            if ( ( semiCreatedLink ) && ( itemWithLinks ) && ( itemWithLinks->acceptIncommingLink( semiCreatedLink ) ) ) {
                
                // If we have an semi-created link and mouse was pressed on the agent - we will finish the link creating 
                // process. But we need not a link.
                
                // Если у нас полу-созданная связь и нажата мышь на агенте - завершаем создание связи. Но нам нужна не-связь.
                
                qDebug() << "У нас есть полу-созданная связь";
                                                                            
                semiCreatedLink->hide();                
                itemWithLinks->addIncommingLink( semiCreatedLink );                    
                semiCreatedLink->show();                
                semiCreatedLink = nullptr;                
                
                SchemaScene * sc = dynamic_cast<SchemaScene * >( scene() );
                if ( sc ) sc->unselectAll();
                
                
            } else {
                                
                if ( itemWithLinks ) {
                    
                    // If there is at least one non-link in the array of elements under the mouse -
                    // then it becomes active and dragged.
                    
                    // Если в массиве элементов под мышкой есть хотя бы одна не-связь - 
                    // то активной и перетаскиваемой становится именно она.
                
                    __entityDragged = itemWithLinks;
                    emit signalItemPressed( itemWithLinks, controlPressed );
                    
                } else if ( entity ) {
                    
                    // If there are a links only, then we try to find the closest one to the point of the mouse from them.
                    // Если там только связи, то пытаемся найти из них самую ближнюю к точке мыши.
                    
                    float distance = MAXIMUM_CONSTRAINT;
                    LinkItem * link = nullptr;
                    for ( int i=0; i<itemsList.count(); i++ ) {
                        LinkItem * l = dynamic_cast< LinkItem * > ( itemsList.at(i ) );
                        if ( l ) {
                            float d = l->distanceTo( mapToScene( event->pos() ) );
                            if ( d < distance ) {
                                distance = d;
                                link = l;
                            };
                        };
                    };
                    
                    if ( link ) {
                        
                        // Links are not dragged.
                        // Связи - не таскаются.
                        // __entityDragged = entity;                
                    
                        emit signalItemPressed( link, controlPressed );
                    };
                    
                };                            
            };
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
            
    if ( semiCreatedLink ) {
        
        // Semi-created link we will move by the point where mouse is located now.
        // Полу-созданную связь будем перемещать при помощи точки, в которой сейчас расположена мышь.
        
        semiCreatedLink->hide();
        semiCreatedLink->setTo( __mouseAtSchemaPos );
        semiCreatedLink->update();
        semiCreatedLink->show();
                
    };
    
    if ( event->buttons() & Qt::LeftButton ) {
        
        if ( __entityDragged ) {
            
            LinkItem * link = dynamic_cast<LinkItem * > ( __entityDragged );
            
            if ( ! __entityDragInProcess )  {
            
                // May be we need to switch on the drag process.
                // Может быть, необходимо включить перетаскивание.
            
                QPoint delta = event->pos() - __mousePressedPos;
                
                // Wi will not dragg a link.
                // Связь не будем таскать.curItem
                
                if (( ! link ) && ( delta.manhattanLength() > QApplication::startDragDistance() )) {
                    __entityDragInProcess = true;
                };
            
            };
        
            // If we are already in drag process, signal about it.
            // Если уже тащим, сигнал об этом.
        
            if ( __entityDragInProcess ) {
                
                QPoint targetPos = mapToScene( event->pos() ).toPoint(); 
                targetPos -= __entityDragged->mousePressedPos();
                // qDebug() << "Event pos=" << event->pos() << ", mouse pressed=" << __mousePressedPos << ", element pos=" << __entityDragged->mousePressedPos();
                emit signalItemMoved( __entityDragged, targetPos );
            };
            
        };
    };
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                          Return mouse at schema position                                         *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                           Вернуть позицию мышки на схеме.                                        *
// *                                                                                                                  *
// ********************************************************************************************************************

QPoint tengu::SchemaView::mouseAtSchema() {
    return __mouseAtSchemaPos;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                              Context menu event handler                                          *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                         Обработчик событий контекстного меню.                                    *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::SchemaView::contextMenuEvent ( QContextMenuEvent* event ) {
    
    __contextMenuItem = dynamic_cast<AbstractEntityItem * > ( itemAt( event->pos()) );
    
    QMenu menu;
    
    if ( __contextMenuItem ) {
        
        // We have some item under mouse
        // У нас есть какой-то элемент под мышкой.
        
        // We can not delete the start of process.
        // Мы не можем удалить старт процесса.
        
        ProcessStartItem * processStartItem = dynamic_cast< ProcessStartItem * > ( __contextMenuItem );        
        if ( ! processStartItem ) menu.addAction( __action__delete_item );                
        
    } else {
        
        // The space under mouse is clear.
        // Пространство под мышкой чистое.
        
        menu.addMenu( __contextMenu__create );                
    };
    
    menu.exec( event->globalPos() );
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
    AbstractEntity * entity = __event_to_entity( event );
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
    
    // What is dragged.
    // То, что тащится
    
    AbstractEntity * entity = __event_to_entity ( event );
    LinkItem * link = dynamic_cast<LinkItem * >( entity );
    
    // What is dragged over
    // То, над чем тащится
    
    AbstractEntityItem * curItem = dynamic_cast< AbstractEntityItem * >( itemAt( event->pos() ) );
    ItemWithLinks * iwl = dynamic_cast< ItemWithLinks * > ( itemAt( event->pos() ) );
    
    if ( entity ) {
        
        // This is need in any case
        // Это нужно по-любому.
        
        delete( entity );
        
        // The event will be processed only if the object does not drop onto an existing object.
        // But if this is a link, then it can be thrown only on the existing object.
        
        // Событие будет обработано только в том случае, если объект не бросается на уже существующий объект.
        // Но если это связь, то ее наоборот можно бросать только на существующий объект.
                
        if ( ( link ) && ( iwl ) && ( iwl->acceptOutgoingLink( link ) ) ) event->acceptProposedAction();
        
        if ( ( ! link ) && ( ! curItem ) ) event->acceptProposedAction();
        
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

void tengu::SchemaView::dropEvent ( QDropEvent * event ) {
    
    QGraphicsView::dropEvent ( event );
    
    // What is dropped on
    // То, что было сброшено.
    
    AbstractEntity * entity = __event_to_entity( event ); 
    
    // LinkItem * link = dynamic_cast<LinkItem *>(entity);
    
    // What is dropped over
    // То, над чем было брошено
    
    // AbstractEntityItem * curItem = dynamic_cast< AbstractEntityItem * >( itemAt( event->pos() ) );
    
    
    if ( entity ) {
        
        // Point on the scene where item was dropped on
        // Точка на сцене, куда бросили элемент.
        
        QPoint pos = mapToScene( event->pos() ).toPoint();
        
        // If this is a link, we need set his "from" or "to" entities.
        // Если это связь, то нужно установить ей сущности "откуда" или "куда".
        
        /*
        if ( ( link ) && ( curItem ) ) {
            
            if ( link->isEmpty() ) {
                link->setFrom( curItem );
                __semiCreatedLink = link;
            } else if ( link->semiCreated() ) {
                link->setTo( curItem );
                __semiCreatedLink = nullptr;
            };
            
        };
        */        
        
        emit signalWasDropped( entity, pos );
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
