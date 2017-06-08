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
#include <QGLFormat>
#include <QPoint>

namespace tengu {

    class MainWindowSchema : public QGraphicsView {
        
        Q_OBJECT
        
        public:
            
            MainWindowSchema( QGraphicsScene * scene );
            virtual ~MainWindowSchema();

            // mouse wheel event
            // событие колесика мышки
            
#ifndef QT_NO_WHEELEVENT
            void wheelEvent( QWheelEvent * event );
#endif
            
        private:
            
            void mouseMoveEvent( QMouseEvent * event );
            QPoint __scaleCenter;
            
            // float __scaleFactor;
                        
    };
};

