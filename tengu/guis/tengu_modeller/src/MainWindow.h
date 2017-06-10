// ********************************************************************************************************************
// *                                                                                                                  *
// *                                       The main window of the Tengu Modeller.                                     *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                           Главное окно Tengu-моделлера.                                          *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 27 may 2017 at 08:26 *
// ********************************************************************************************************************

#pragma once

#include <QApplication>
#include <QMainWindow>
#include <QSplitter>
#include <QLayout>
#include <QGraphicsScene>
#include <QStatusBar>
#include <QToolBar>
#include <QMenuBar>
#include <QMenu>
#include <QIcon>
#include <QPixmap>
#include <QDebug>
#include <QSettings>
#include <QRect>

#include "AbstractAgent.h"
#include "WorkSpace.h"
#include "Task.h"
#include "AgentPropertyModel.h"
#include "MainWindowLeft.h"
#include "SchemaView.h"
#include "SchemaScene.h"
#include "MainWindowRight.h"
#include "Constants.h"
#include "../guis.h"

namespace tengu {
    
    class MainWindow : public QMainWindow {

        Q_OBJECT

        public:
            explicit MainWindow(QWidget *parent = 0);
            ~MainWindow();
            
        private:
        
            // Horizontal splitter for main window's elements.
            // Горизонтальный сплиттер для элементов главного окна.
            QSplitter * __hSplitter;
            
            QWidget * __centerWidget;
            MainWindowLeft * __left;
            SchemaView * __schemaView;
            MainWindowRight * __right;
            SchemaScene * __schemaScene;
            
            QToolBar * __toolbar_file;
            QToolBar * __toolbar_elements_library;
            
            void __createMainMenu();
            void __createSchemaView();
            void __createActions();
            void __createToolBar();
            void __createStatusBar();
            
            QAction * __actionQuit;
            
            void __restoreSettings();
            
            // The top model (a set of an agents)
            // Самая верхняя модель (как множество агентов)
            
            WorkSpace * __workSpace;
            
            // MVC-model for editing property of current (selected) agent or state.
            // MVC-модель редактирования свойств текущего (выбранного) агента или состояния.
            
            AgentPropertyModel * __agentPropertyModel;
            
            // Overrided widget's functions
            // Перекрытые функции виджита.
            
            void closeEvent( QCloseEvent * event );
            void showEvent( QShowEvent * event );
            
        private slots:
            
            void __on_quit_request();  
            
            void __on_schema_item_pressed( AbstractAgentItem * item, bool controlPressed );
            void __on_schema_item_double_clicked( AbstractAgentItem * item, bool controlPressed );
                        
            
    };
    
};

