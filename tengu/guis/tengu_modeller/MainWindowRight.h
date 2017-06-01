// ********************************************************************************************************************
// *                                                                                                                  *
// *                                     The right part of main window (properties area).                             *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                        Правая часть главного окна (зона свойств).                                *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 01 jun 2017 at 09:11 *
// ********************************************************************************************************************

#pragma once

#include <QWidget>
#include <QSplitter>
#include <QLayout>
#include <QPair>
#include <QList>

#include "AgentPropertyView.h"
#include "../guis.h"

namespace tengu {

    class MainWindowRight : public QWidget {
        Q_OBJECT
        public:
            MainWindowRight ( QWidget * parent = 0 );
            ~MainWindowRight();
            
            // Get heights of top and bottom components for store in the settings.
            // Получить высОты верхнего и нижнего компонента для записи в установках.
            
            QPair<int, int> getComponentsHeights();
            
            // Set components height after restore window's settings.
            // Установить высоты компонентов после восстановления установок окна.
            
            void setComponentsHeight( QList<int> h);
            
        private:
            QSplitter * __splitter;
            AgentPropertyView * __agentsView;
            QWidget * __down;
    };
    
};

