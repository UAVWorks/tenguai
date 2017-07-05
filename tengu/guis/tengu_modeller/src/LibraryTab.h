// ********************************************************************************************************************
// *                                                                                                                  *
// *                                     Tabulator widget for component library toolbar.                              *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                  Виджит табулятора для тулбара библиотеки компонентов.                           *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 12 jun 2017 at 14:56 *
// ********************************************************************************************************************

#pragma once

#include <QTabWidget>
#include <QFont>

#include "LibraryTab_Processes.h"

namespace tengu {
    
    class LibraryTab : public QTabWidget {
        
        Q_OBJECT
        
        public:
            
            explicit LibraryTab ( QWidget* parent = 0 );
            virtual ~LibraryTab();
        
            LibraryTab__processes * tab__processes;
        
        public slots:
            
            void on__inside_process();
            
        protected:
        private:
            
            
            
    };
};
