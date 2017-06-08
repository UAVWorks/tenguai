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
    setViewport(new QGLWidget( QGLFormat(QGL::SampleBuffers)) );
    
    // Scale center is center of view by default, will be changed by first mouse move event.
    // Центр масштабирования - это по умолчанию центр виджита. Будет изменен первым же движением мыши.
    
    __scaleCenter = QPoint( width() / 2, height() / 2 );
    
    // widget receives mouse move events even if no buttons are pressed.
    // Виджит принимает события от мыши даже если ни одна из кнопок не была нажата.
    
    setMouseTracking( true );
    
    // __scaleFactor = 1.0;
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
    
    qDebug() << "Wheel at " << __scaleCenter;
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
// *                                              Mouse move event handler.                                           *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                         Обработчик события перемещения мыши.                                     *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::MainWindowSchema::mouseMoveEvent ( QMouseEvent * event ) {
    __scaleCenter = event->pos();
    QGraphicsView::mouseMoveEvent ( event );
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
