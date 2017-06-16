// ********************************************************************************************************************
// *                                                                                                                  *
// *                            UML-like "schema", the graphics representation of model.                              *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                             UML-подобная "схема", графическое представление модели.                              *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 28 may 2017 at 15:48 *
// ********************************************************************************************************************

#pragma once

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QWheelEvent>
#include <QDebug>
#include <QMouseEvent>
// #include <QGLFormat>
#include <QPoint>
#include <QMenu>
#include <QAction>
#include <QPixmap>
#include <QMimeData>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDragLeaveEvent>
#include <QDropEvent>
#include <QJsonObject>
#include <QJsonDocument>

#include "AbstractEntityItem.h"
#include "AbstractEntity.h"
#include "AgentFactory.h"

namespace tengu {

    class SchemaView : public QGraphicsView {
        
        Q_OBJECT
        
        signals:
            
            void signalItemPressed( AbstractEntityItem * item, bool controlPressed );
            void signalItemDoubleClicked( AbstractEntityItem * item, bool controlPressed );
            void signalWasDropped( AbstractStorageableEntity * entity );
        
        public:
            
            SchemaView( QGraphicsScene * scene );
            virtual ~SchemaView();

            // mouse wheel event
            // событие колесика мышки
                        
        private:
            
            void mousePressEvent( QMouseEvent * event );
            void mouseReleaseEvent( QMouseEvent * event );
            void mouseMoveEvent( QMouseEvent * event );   
            void mouseDoubleClickEvent(QMouseEvent * event);
            void contextMenuEvent( QContextMenuEvent * event );
            
            void dragEnterEvent(QDragEnterEvent * event);
            void dragMoveEvent(QDragMoveEvent * event);
            void dragLeaveEvent(QDragLeaveEvent * event);
            void dropEvent( QDropEvent * event );
            
            AbstractStorageableEntity * __event_to_entity( QDropEvent * event );
            
#ifndef QT_NO_WHEELEVENT
            void wheelEvent( QWheelEvent * event );
#endif
            
            bool __leftMouseButtonPressed;
            
            QMenu * __contextMenu;
            QMenu * __contextMenu__create;
            QAction * __actionCreateTask;
            
            void __createMenus();
            
            QPoint __scaleCenter;
            
            QPoint __mouseAtSchemaPos;
            
            // float __scaleFactor;
            
        private slots:
            
            void __on_action_create_task();            
                        
    };
};

