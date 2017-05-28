// ********************************************************************************************************************
// *                                                                                                                  *
// *                                       The main window of the Tengu Modeller.                                     *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                           Главное окно Tengu-моделлера.                                          *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 27 may 2017 at 08:26 *
// ********************************************************************************************************************

#pragma once

#include <QMainWindow>
#include <QSplitter>
#include <QLayout>

#include "MainWindowLeft.h"
#include "MainWindowSchema.h"

namespace tengu {
    
    class MainWindow : public QMainWindow {

        Q_OBJECT

        public:
            explicit MainWindow(QWidget *parent = 0);
            ~MainWindow();
            
        private:
        
            QSplitter * __splitter;
            QWidget * __centerWidget;
            MainWindowLeft * __left;
            MainWindowSchema * __schema;
    };
    
};

