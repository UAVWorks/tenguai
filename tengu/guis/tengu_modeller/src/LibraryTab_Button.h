// ********************************************************************************************************************
// *                                                                                                                  *
// *                         One button at component library toolbar, it can emit drag event.                         *
// * ---------------------------------------------------------------------------------------------------------------- *
// *             Кнопка на тулбаре библиотеки компонентов, которая может генерировать сигнал перетаскивания.          *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 12 jun 2017 at 16:08 *
// ********************************************************************************************************************

#pragma once

#include <QPushButton>
#include <QIcon>
#include <QString>
#include <QSize>
#include <QEvent>
#include <QMouseEvent>
#include <QPoint>
#include <QDebug>
#include <QDrag>
#include <QMimeData>

namespace tengu {
    
    class LibraryTab_Button : public QPushButton {
        
        Q_OBJECT
        
        public:
            
            LibraryTab_Button ( QIcon icon, QString hint );
            virtual ~LibraryTab_Button();
            
        protected:
            
            void mousePressEvent( QMouseEvent * event );
            void mouseReleaseEvent( QMouseEvent * event );
            void mouseMoveEvent( QMouseEvent * event );
            void leaveEvent( QEvent * event );
            
        private:
            
            bool __mouse_left_button_pressed;
            bool __dragging;
            QPoint __mouse_pressed_position;
            
    };
    
};

