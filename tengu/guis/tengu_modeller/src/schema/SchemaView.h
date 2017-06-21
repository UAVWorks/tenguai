// ********************************************************************************************************************
// *                                                                                                                  *
// *                            UML-like "schema", the graphics representation of model.                              *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                             UML-подобная "схема", графическое представление модели.                              *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 28 may 2017 at 15:48 *
// ********************************************************************************************************************

#pragma once

#include <QApplication>
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
#include "AgentItemFactory.h"
#include "SchemaScene.h"

namespace tengu {

    class SchemaView : public QGraphicsView {
        
        Q_OBJECT
        
        signals:
            
            void signalItemPressed( AbstractEntityItem * item, bool controlPressed );
            void signalItemDoubleClicked( AbstractEntityItem * item, bool controlPressed );
            void signalItemMoved( AbstractEntityItem * item, QPoint pos );
            void signalWasDropped( AbstractEntity * entity, QPoint pos );
        
        public:
            
            SchemaView( QGraphicsScene * scene );
            virtual ~SchemaView();
            
            LinkItem * semiCreatedLink;
                                    
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
                        
            AbstractEntity * __event_to_entity( QDropEvent * event );            
            
#ifndef QT_NO_WHEELEVENT
            // mouse wheel event
            // событие колесика мышки
            void wheelEvent( QWheelEvent * event );
#endif
            
            bool __leftMouseButtonPressed;
            
            // QMenu * __contextMenu;
            QMenu * __contextMenu__create;
            
            QAction * __action__create_task;
            QAction * __action__delete_item;
            
            void __createMenus();
            
            QPoint __scaleCenter;
            
            QPoint __mouseAtSchemaPos;
            QPoint __mousePressedPos;
            AbstractEntityItem * __contextMenuItem;
            
            AbstractEntityItem * __entityDragged;
            bool __entityDragInProcess;                        
            
            // float __scaleFactor;
            
        private slots:
            
            void __on__action__create_task();
            void __on__action__delete_item();
                        
    };
};

