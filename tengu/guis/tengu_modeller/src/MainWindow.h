// ********************************************************************************************************************
// *                                                                                                                  *
// *                                       The main window of the Tengu Modeller.                                     *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                           Главное окно Tengu-моделлера.                                          *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 27 may 2017 at 08:26 *
// ********************************************************************************************************************

#pragma once

#include <typeinfo>

#include <QApplication>
#include <QMainWindow>
#include <QSplitter>
#include <QLayout>
#include <QGraphicsScene>
#include <QStatusBar>
#include <QToolBar>
#include <QToolButton>
#include <QButtonGroup>
#include <QActionGroup>
#include <QMenuBar>
#include <QMenu>
#include <QIcon>
#include <QPixmap>
#include <QDebug>
#include <QSettings>
#include <QRect>
#include <QDrag>
#include <QMessageBox>

#include "MongoStorage.h"
#include "AgentFactory.h"
#include "AgentItemFactory.h"
#include "AgentPropertyModel.h"
#include "AgentPropertyDelegate.h"
#include "MainWindowLeft.h"
#include "LibraryTab.h"
#include "SchemaView.h"
#include "SchemaScene.h"
#include "MainWindowRight.h"
#include "Constants.h"
#include "../guis.h"
#include "DialogPropertiesSprout.h"
#include "DialogPropertiesTask.h"
#include "DialogOpenSaveModel.h"
#include "XPlaneProcess.h"
#include "NotSavedOperation.h"

namespace tengu {
    
    class MainWindow : public QMainWindow {

        Q_OBJECT

        public:
            
            explicit MainWindow(QWidget *parent = 0);
            ~MainWindow();
            
        protected:
            
            // Overrided widget's functions
            // Перекрытые функции виджита.
            
            void closeEvent( QCloseEvent * event );
            void showEvent( QShowEvent * event );
            void keyPressEvent( QKeyEvent * event );
            
            
        private:
        
            bool __do_not_handle_events;
            
            // Horizontal splitter for main window's elements.
            // Горизонтальный сплиттер для элементов главного окна.
            QSplitter * __hSplitter;
            
            QWidget * __centerWidget;
            MainWindowLeft * __left;
            SchemaView * __schemaView;
            MainWindowRight * __right;
            SchemaScene * __schemaScene;
            
            QToolBar * __toolbar_file;
            
            QToolBar * __toolbar_execution_mode;
            
            QAction * __action__create_schema;
            QAction * __action__create_schema__process;
            QAction * __action__open_schema;
            QAction * __action__open_schema__process;
            QAction * __action__save_schema;            
            
            QAction * __action__execution_mode_xplane;
            QAction * __action__execution_mode_real;
            
            QAction * __action__simulation_begin;
            QAction * __action__simulation_start;
            QAction * __action__simulation_pause;
            QAction * __action__simulation_stop;
            
            QToolBar * __toolbar_elements_library;
            
            LibraryTab * __library_tab;
            
            MongoStorage * __mongo;
            
            void __createMongoStorage();
            void __createWorkspace();
            void __createMainMenu();
            void __createLibraryTab();
            void __createSchemaView();
            void __createSchemaScene();
            void __createActions();
            void __createToolBar();
            void __createStatusBar();
            void __createDialogs();
            
            QAction * __actionQuit;
            
            void __restoreSettings();
                        
            WorkSpace * __workSpace;
            
            // MVC-model for editing property of current (selected) agent or state.
            // MVC-модель редактирования свойств текущего (выбранного) агента или состояния.
            
            AgentPropertyModel * __agentPropertyModel;
            AgentPropertyDelegate * __agentPropertyDelegate;
                        
            // The main window's dialogs
            // Диалоги главного окна.
            
            DialogPropertiesSprout * __dialogPropertiesSprout;
            DialogPropertiesTask * __dialogPropertiesTask;
            DialogOpenSaveModel * __dialogOpenSaveModel;
            
            AbstractEntity::execution_mode_t __execution_mode;
            
            void __check_start_element_in_process( AbstractAgent * agent );
            
            QList<NotSavedOperation> __not_saved_operations;
            
        private slots:
            
            void __on_quit_request();  
            
            void __on_schema_item_pressed( AbstractEntityItem * item, bool controlPressed );
            void __on_schema_item_double_clicked( AbstractEntityItem * item, bool controlPressed );
            void __on_schama_item_moved( AbstractEntityItem * entity, QPoint pos );
            void __on_schema_item_was_dropped( AbstractEntity * entity, QPoint pos );
            void __on__something_changed();
            
            void __on__tree_structure__agent_was_selected( AbstractAgent * agent );
            // void __on__tree_structure__agent_was_created( AbstractAgent * agent );
            // void __on__tree_structure__agent_was_deleted( QString uuid );
            
            // Clear agent. Independed of signal sender.
            // Очистка агента. Независимо от источника сигнала.
            void __on__clear_agent( AbstractAgent * agent );
            
            void __on__want__create_agent( AbstractAgent * parent, AbstractEntity::entity_types_t type );
            void __on__want__delete( AbstractEntity * entity );
            
            // void __on__create__process();
            void __on__save();
            void __on__want__open();
            void __on__agent__opened( AbstractAgent * agent );
            void __on__error( tengu::error_level_t errorLevel, QString procedureName, QString errorMessage );
            
            void __on_set_execution_mode_xplane();
            void __on_set_execution_mode_real();
            
            void __on__simulation_begin();
            void __on__simulation_stop();
            void __on__simulation_start();
            void __on__simulation_pause();
                        
            
    };
    
};

