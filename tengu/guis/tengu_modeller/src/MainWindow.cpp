// ********************************************************************************************************************
// *                                                                                                                  *
// *                                       The main window of the Tengu Modeller.                                     *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                           Главное окно Tengu-моделлера.                                          *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 27 may 2017 at 08:27 *
// ********************************************************************************************************************

#include "MainWindow.h"

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                  The constructor.                                                *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                     Конструктор.                                                 *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::MainWindow::MainWindow(QWidget *parent) 
    : QMainWindow(parent)
{

    __do_not_handle_events = true;
    
    setWindowTitle( tr("Tengu modeller") );
    setWindowIcon( QIcon( QPixmap(":tengu_32.png") ) );
    
    __createMongoStorage();
    
    // Workspace must be created before widgets based on it
    // Рабочее пространство должно быть создано до виджитов, которые на нем базируются.
    
    __createWorkspace();    
    
    __agentPropertyModel = new AgentPropertyModel();
    __agentPropertyDelegate = new AgentPropertyDelegate( __agentPropertyModel );
    
    __createSchemaScene();
    __createSchemaView();

    
    QVBoxLayout * lay = new QVBoxLayout();
    lay->setMargin( 0 );
    lay->setSpacing( 0 );
    
    __centerWidget = new QWidget();
    __centerWidget->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding ) );
    __centerWidget->setMinimumSize( QSize(800, 600 ) );
    __centerWidget->setLayout( lay );
    
    setCentralWidget( __centerWidget );
    
    __hSplitter = new QSplitter();
    __hSplitter->setHandleWidth( SPLITTERS_HANDLE_WIDTH );
    __hSplitter->setLineWidth( SPLITTERS_LINE_WIDTH );
    
    lay->addWidget( __hSplitter );
    
    // Left part of the application
    // Левая часть приложения.
    
    __left = new MainWindowLeft( __workSpace );    
    // QObject::connect( __left->treeStructure, SIGNAL( signalAgentCreated( AbstractAgent * ) ), this, SLOT( __on__tree_structure__agent_was_created( AbstractAgent * ) ) );
    
    QObject::connect( __left->treeStructure, SIGNAL( signalWantCreateAgent( AbstractAgent *,AbstractEntity::entity_types_t)),
        this, SLOT( __on__want__create_agent( AbstractAgent *, AbstractEntity::entity_types_t ) ) );
    
    QObject::connect( __left->treeStructure, SIGNAL( signalAgentSelected( AbstractAgent * ) ), this, SLOT( __on__tree_structure__agent_was_selected( AbstractAgent * ) ) );
    // QObject::connect( __left->treeStructure, SIGNAL( signalAgentDeleted( QString ) ), this, SLOT( __on__tree_structure__agent_was_deleted( QString ) ) );
    QObject::connect( __left->treeStructure, SIGNAL( signalClearAgent( AbstractAgent * ) ), this, SLOT( __on__clear_agent( AbstractAgent * ) ) );
    QObject::connect( __left->treeStructure, SIGNAL(signalError(tengu::error_level_t,QString,QString)), this, SLOT( __on__error( tengu::error_level_t, QString, QString ) ) );
    
    // Right part of the application
    // Правая часть приложения.
    
    __right = new MainWindowRight();    
    __right->propertyView->setModel( __agentPropertyModel );
    __right->propertyView->setItemDelegateForColumn( 1, __agentPropertyDelegate );
    
    __hSplitter->addWidget( __left );
    __hSplitter->addWidget( __schemaView );
    __hSplitter->addWidget( __right );        
        
    __createActions();
    __createLibraryTab();
    __createMainMenu();
    __createToolBar();
    __createStatusBar();    
    __createDialogs();                
    
    __not_saved_operations = QList<NotSavedOperation>();
    __running_agent = nullptr;

    __do_not_handle_events = false;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                           Check start element in process. Create one if does not exists.                         *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                    Проверка элемента "начало" в процессе. Создание, если его еще не существует.                  *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::MainWindow::__check_start_element_in_process ( tengu::AbstractAgent * agent ) {
    
    Process * process = dynamic_cast< Process * > ( agent );
    if ( process ) {
    
        // Have this process start element?
        // Есть в этом процессе элемент "начало"?
        
        bool have = false;
        QList<AbstractAgent *> hisChildren = agent->children();
        for ( int i=0; i<hisChildren.count(); i++ ) {
            if ( hisChildren.at(i)->entityType() == AbstractEntity::ET_ProcessStart ) {
                have = true;
                break;
            };
        };
        
        // Create process start if we have not one
        // Создание начала процесса, если у нас его еще нет.
        
        if ( ! have ) {
            
            ProcessStart * start = new ProcessStart();
            start->setSystemName( process->getSystemName() );
            start->setHumanName( process->getHumanName() );
            start->setComment( process->getComment() );
            agent->addChild( start );
            
            if ( ( __schemaScene->rootEntity() ) && ( __schemaScene->rootEntity()->getUUID() == agent->getUUID() ) ) {
                ProcessStartItem * si = new ProcessStartItem( start );
                __schemaScene->addItem( si );
            };
        }
    }
    
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                  Action's constructor.                                           *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                              Конструктор действий программы.                                     *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::MainWindow::__createActions() {
    
    __actionQuit = new QAction( tr( "Quit" ), this );
    __actionQuit->setIcon( QIcon( QPixmap(":door_open_16.png") ) );
    QObject::connect( __actionQuit, SIGNAL(triggered()), this, SLOT( __on_quit_request() ) );
    
    // a "File" actions
    // Действия с "файлами".
    
    __action__create_schema = new QAction( QIcon( QPixmap(":note_add_16.png") ), tr("Create..."), this );
    // QObject::connect();
    
    __action__create_schema__process = new QAction( QIcon(":package_add_16.png"), tr("Create the process"), this );
    // QObject::connect( __action__create_schema__process, SIGNAL( triggered() ), this, SLOT( __on__create__process() ) );
    
    // Open (vehicle, process, task e.t.c.)
    // Открыть ("самоходку", процесс, задачу и др)
    
    __action__open_schema = new QAction( QIcon( QPixmap(":folder_16.png") ), tr("Open..."), this );
    QObject::connect( __action__open_schema, SIGNAL( triggered() ), this, SLOT( __on__want__open() ) );
    
    __action__open_schema__process = new QAction( QIcon( QPixmap(":folder_brick_16.png") ), tr("Open the process"), this );
    
    __action__save_schema = new QAction( QIcon(QPixmap(":diskette_16.png")), tr("Save"), this );
    QObject::connect( __action__save_schema, SIGNAL( triggered() ), this, SLOT( __on__save() ) );
    __action__save_schema->setEnabled( false );
    // QObject::connect(
    
    // Execution mode actions.
    // Действия изменения режима выполнения.
    
    QActionGroup * groupExecutionMode = new QActionGroup( this );
    
    __action__execution_mode_xplane = new QAction( QIcon( QPixmap(":xplane10_16.png") ), tr("Set execution mode to X-Plane") , groupExecutionMode );
    QObject::connect( __action__execution_mode_xplane, SIGNAL( triggered() ), this, SLOT( __on_set_execution_mode_xplane() ) );
    __action__execution_mode_xplane->setCheckable( true );
    
    __action__execution_mode_real = new QAction( QIcon( QPixmap(":cog_16.png") ), tr("Set execution mode to Real"), groupExecutionMode );
    QObject::connect( __action__execution_mode_real, SIGNAL( triggered() ), this, SLOT( __on_set_execution_mode_real() ) );
    __action__execution_mode_real->setCheckable( true );
    
    __action__execution_mode_xplane->setChecked( true );
    __execution_mode = AbstractEntity::EM_XPLANE;
    
    // Action for simulation process.
    // Действия для процесса симуляции.
    
    __action__simulation_begin = new QAction( QIcon( QPixmap(":control_start_blue_16.png") ), tr("Jump to simulation begin"), this );
    QObject::connect( __action__simulation_begin, SIGNAL( triggered() ), this, SLOT( __on__simulation_begin() ) );
    __action__simulation_begin->setEnabled( false );
    
    __action__simulation_start = new QAction( QIcon( QPixmap(":control_play_blue_16.png") ), tr("Start a simulation process"), this );
    QObject::connect( __action__simulation_start, SIGNAL( triggered() ), this, SLOT( __on__simulation_start() ) );
    __action__simulation_start->setEnabled( false );
    
    __action__simulation_stop = new QAction( QIcon( QPixmap(":control_stop_blue_16.png") ), tr("Stop a simulation process"), this );
    QObject::connect( __action__simulation_stop, SIGNAL( triggered() ), this, SLOT( __on__simulation_stop() ) );
    __action__simulation_stop->setEnabled( false );
    
    __action__simulation_pause = new QAction( QIcon( QPixmap(":control_play_pause_blue_16.png") ), tr("Pause a simulation process"), this );
    QObject::connect( __action__simulation_pause, SIGNAL( triggered() ), this, SLOT( __on__simulation_pause() ) );
    __action__simulation_pause->setEnabled( false );
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                   Create dialogs.                                                *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                 Создание диалогов.                                               *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::MainWindow::__createDialogs() {
    
    __dialogPropertiesSprout = new DialogPropertiesSprout( __workSpace );
    QObject::connect( __dialogPropertiesSprout, SIGNAL(signalError(tengu::error_level_t,QString,QString)), this, SLOT( __on__error( tengu::error_level_t, QString, QString ) ) );
    
    __dialogPropertiesTask = new DialogPropertiesTask( __workSpace );
    QObject::connect( __dialogPropertiesTask, SIGNAL(signalError(tengu::error_level_t,QString,QString)), this, SLOT( __on__error( tengu::error_level_t, QString, QString ) ) );
    
    __dialogOpenSaveModel = new DialogOpenSaveModel( __mongo );
    QObject::connect( __dialogOpenSaveModel, SIGNAL(signalError(tengu::error_level_t,QString,QString)), this, SLOT( __on__error( tengu::error_level_t, QString, QString ) ) );
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                               Create component's library tabulator for bottom toolbar.                           *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                            Создание табулятора библиотеки элементов для нижнего тулбара.                         *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::MainWindow::__createLibraryTab() {
    __library_tab = new LibraryTab();
    __library_tab->tab__processes->setEnabled( false );
    
    // Signals from schema to tabulator
    // Сигналы от схемы к табулятору.
    
    QObject::connect( __schemaScene, SIGNAL( signalInsideProcess() ), __library_tab, SLOT( on__inside_process() ) );
    QObject::connect( __schemaScene, SIGNAL( signalInsideProcess() ), __library_tab->tab__processes, SLOT( on__inside_process() ) );
    
    // QObject::connect( __schemaScene, SIGNAL( signalProcessStartCreated() ), __library_tab->tab__processes, SLOT( on__process_start_created() ) );
    QObject::connect( __schemaScene, SIGNAL( signalProcessItemWithLinksCreated() ), __library_tab->tab__processes, SLOT( on__process_item_with_links_created() ) );
    QObject::connect( __schemaScene, SIGNAL( signalProcessExplicitTaskCreated() ), __library_tab->tab__processes, SLOT( on__process_explicit_task_created() ) );            
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                               The main menu constructor.                                         *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                               Конструктор главного меню.                                         *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::MainWindow::__createMainMenu() {
    
    QMenuBar * mb = menuBar();
    
    QMenu * menuMain = new QMenu("File");
    menuMain->addAction( __action__open_schema );
    
    menuMain->addAction( __actionQuit );    
    
    mb->addMenu( menuMain );
    
}


// ********************************************************************************************************************
// *                                                                                                                  *
// *                                            Create local mongo storage.                                           *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                        Создание локального хранилища монго.                                      *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::MainWindow::__createMongoStorage() {
    
    __mongo = new MongoStorage();
    
    QObject::connect( __mongo, SIGNAL( signalError( tengu::error_level_t, QString, QString ) ), this, SLOT( __on__error( tengu::error_level_t, QString, QString ) ) );        
    
    // Check actuality of existing indexes for the classes (ORM-like behaviour).    
    // Проверка актуальности существующих индексов для классов (ORM-подобное поведение).
    
    Task t; __mongo->checkIndexes( & t );
    Process p; __mongo->checkIndexes( & p );
    Vehicle v; __mongo->checkIndexes( & v );

}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                        The constructor of model's schema scene with predefined components.                       *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                         Конструктор "схемы" модели с предопределенными компонентами на ней.                      *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::MainWindow::__createSchemaScene() {
    
    // The scene itself.
    // Сама сцена.
    
    __schemaScene = new SchemaScene();
    
    QObject::connect( __schemaScene, SIGNAL( signalSomethingChanged() ), this, SLOT( __on__something_changed() ) );                

}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                           The constructor of model's schama (graphical view of the model).                       *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                               Конструктор схемы (графического представления модели).                             *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::MainWindow::__createSchemaView() {
    __schemaView = new SchemaView( __schemaScene );
    QObject::connect( __schemaView, SIGNAL( signalItemPressed( AbstractEntityItem *, bool )), this, SLOT( __on_schema_item_pressed ( AbstractEntityItem *, bool ) ) );
    QObject::connect( __schemaView, SIGNAL( signalItemDoubleClicked( AbstractEntityItem *, bool )), this, SLOT( __on_schema_item_double_clicked( AbstractEntityItem *, bool ) ) );
    QObject::connect( __schemaView, SIGNAL( signalItemMoved( AbstractEntityItem * , QPoint )), this, SLOT( __on_schama_item_moved( AbstractEntityItem * , QPoint ) ) );
    QObject::connect( __schemaView, SIGNAL( signalWasDropped( AbstractEntity *, QPoint)), this, SLOT( __on_schema_item_was_dropped( AbstractEntity *, QPoint ) ) );
    QObject::connect( __schemaView, SIGNAL( signalWantDelete( AbstractEntity * ) ), this, SLOT( __on__want__delete( AbstractEntity * ) ) );
    QObject::connect( __schemaView, SIGNAL(signalWantCreateAgent(AbstractAgent*,AbstractEntity::entity_types_t)),
                        this, SLOT( __on__want__create_agent( AbstractAgent *, AbstractEntity::entity_types_t ) ) );
        
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                              The status bar constructor.                                         *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                              Конструктор строки статуса.                                         *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::MainWindow::__createStatusBar() {
    QStatusBar * sb = statusBar();
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                The toolbar constructor.                                          *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                           Конструктор инструментальной панели.                                   *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::MainWindow::__createToolBar() {
    
    // --------------------------------------------------------------------------------------------
    //                                        Toolbar "File"
    //                                         тулбар "Файл"
    // --------------------------------------------------------------------------------------------
    
    __toolbar_file = new QToolBar();
    __toolbar_file->setIconSize( QSize(16,16) );
    
    // Create button
    // Кнопка создания
    
    __toolbar_file->addAction( __action__create_schema );
    QToolButton * createSchemaButton = dynamic_cast<QToolButton*>( __toolbar_file->widgetForAction( __action__create_schema ));
    createSchemaButton->setPopupMode( QToolButton::InstantPopup );
    createSchemaButton->removeAction( __action__create_schema );
    // createSchemaButton->addAction( __action__create_schema__process );
    
    // Open button
    // Кнопка открытия
    
    __toolbar_file->addAction( __action__open_schema );
    
    
    //QToolButton * openSchemaButton = dynamic_cast< QToolButton * > ( __toolbar_file->widgetForAction( __action__open_schema ) );
    //openSchemaButton->setPopupMode( QToolButton::InstantPopup );
    //openSchemaButton->removeAction( __action__open_schema );
    //openSchemaButton->addAction( __action__open_schema__process );
    
    __toolbar_file->addAction( __action__save_schema );
        
    addToolBar( Qt::TopToolBarArea, __toolbar_file );
    
    // Model execution mode toolbar
    // Тулбар режима выполнения модели.
    
    __toolbar_execution_mode = new QToolBar();
    __toolbar_execution_mode->setIconSize( QSize(16,16) );
    __toolbar_execution_mode->addAction( __action__execution_mode_xplane );
    __toolbar_execution_mode->addAction( __action__execution_mode_real );
    
    __toolbar_execution_mode->addSeparator();
    
    __toolbar_execution_mode->addAction( __action__simulation_begin );
    __toolbar_execution_mode->addAction( __action__simulation_start );
    __toolbar_execution_mode->addAction( __action__simulation_pause );
    __toolbar_execution_mode->addAction( __action__simulation_stop );
    
    addToolBar( Qt::TopToolBarArea, __toolbar_execution_mode );
    
    // __toolbutton_em_xplane = new QToolButton();
    // __toolbutton_em_xplane->setFixedSize( QSize( 22, 22 ) );
    // groupExecutionMode->addButton( __toolbutton_em_xplane );       
        
    addToolBarBreak();
    
    __toolbar_elements_library = new QToolBar();
    __toolbar_elements_library->addWidget( __library_tab );
    addToolBar( Qt::TopToolBarArea, __toolbar_elements_library );
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                               Create workspace object.                                           *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                         Создание объекта рабочего пространства.                                  *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::MainWindow::__createWorkspace() {
    
    __workSpace = new WorkSpace();
    
    XPlaneSimulator * xplane = new XPlaneSimulator();
    __workSpace->addChild( xplane );
    
    // Connection goes after add an xplane as a child.
    // Соединение происходит после добавления xplane в качестве ребенка.
    
    QObject::connect( __workSpace, SIGNAL( signalSomethingChanged() ), this, SLOT( __on__something_changed() ) );    
    
    // XPlaneProcess * xpProcsss = new XPlaneProcess();
    // __workSpace->addChild( xpProcsss );
    
    // In the workspace we always have an x-plane process.
    // Внутри рабочего пространства у нас всегда есть процесс X-Plane.
    
    // We always have invisible X-Plane schema item for modelling.
    // У нас всегда есть невидимый компонент схемы - X-Plane. Он для моделирования.
    
    /*
    XPlaneAgent * xplane = new XPlaneAgent();
    XPlaneAgentItem * xpItem = new XPlaneAgentItem( xplane );
    xpItem->setX( 0 );
    xpItem->setY( 0 );
    __schemaScene->addItem( xpItem );
    */
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                  Open the model.                                                 *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                  Открытие модели.                                                *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::MainWindow::__on__want__open() {
    __dialogOpenSaveModel->setCurrentMode( DialogOpenSaveModel::DM_OPEN );
    if ( __dialogOpenSaveModel->exec() == QDialog::Accepted ) {
        AbstractAgent * result = __dialogOpenSaveModel->result_agent;
        if ( result ) {
            __on__agent__opened( result );
        };
    }
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                 Create new process.                                              *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                               Создание нового процесса.                                          *
// *                                                                                                                  *
// ********************************************************************************************************************

/*
void tengu::MainWindow::__on__create__process() {
    
    
    Process * process = new Process();
    ProcessItem * processItem = new ProcessItem( process );
    
    // Create Process Start block inside new process.
    // Создание блока начала процесса внутри нового процесса.
    
    ProcessStart * start = new ProcessStart();
    start->setSystemName("NewProcess");
    start->setHumanName("New Process");
    ProcessStartItem * startItem = new ProcessStartItem( start );
    process->addChild( start );
    
    __schemaView->hide();
    __schemaScene->setRootItem( processItem );   
    __schemaScene->addItem( startItem );
    __schemaView->show();
    
    __library_tab->tab__processes->setEnabled( true );
    __library_tab->setCurrentWidget( __library_tab->tab__processes );
    // __library_tab->tab__processes->on__process_created();
  
    
}
*/

// ********************************************************************************************************************
// *                                                                                                                  *
// *                              Some element was (once or single) clicked on the schema.                            *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                         На схеме был нажат какой-то элемент.                                     *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::MainWindow::__on_schema_item_pressed ( tengu::AbstractEntityItem * item, bool controlPressed ) {
    
    __schemaView->hide();
    
    if ( ! controlPressed ) __schemaScene->unselectAll();
    
    // Selecting of the agent.
    // Выбор агента.
    
    item->setSelected( true );
    
    __schemaView->show();
    
    // Synchronizе properties of the selected agent
    // Синхронизация свойств выбранного агента.
    
    __agentPropertyModel->setEntityItem( item );
    __right->propertyView->adjustColumnWidth();
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                     Some element was double clicked on the schema.                               *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                       На элементе схемы был двойной щелчок мыши.                                 *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::MainWindow::__on_schema_item_double_clicked ( tengu::AbstractEntityItem * item, bool controlPressed ) {
    DialogProperties * dialog = nullptr;
    SproutItem * sproutItem = dynamic_cast< SproutItem * > ( item );
    TaskItem * taskItem = dynamic_cast< TaskItem * > ( item );
    
    if ( sproutItem ) dialog = __dialogPropertiesSprout;        
    if ( taskItem ) dialog = __dialogPropertiesTask;
    
    if ( dialog ) {
        dialog->fillFrom( item );
        dialog->show();
    };
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                        Entity element was moved at the schema.                                   *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                           "Сущность" перемещают по схеме.                                        *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::MainWindow::__on_schama_item_moved ( tengu::AbstractEntityItem * entity, QPoint pos ) {
    
    if ( entity ) {
        
        __schemaView->hide();
        
        // Move the entity itself on the screen.
        // Движение самой "сущности" на экране.
        
        entity->setX( pos.x() );
        entity->setY( pos.y() );  
        
        entity->recalculate();        
        entity->update();        
        
        ItemWithLinks * itemWithLinks = dynamic_cast< ItemWithLinks * >( entity );
        
        if ( itemWithLinks ) {
        
        // we need to repaint his links.
        // нужно перерисовать ее связи.
                
            QList<LinkItem * > links = itemWithLinks->hisLinks();
            for ( int i=0; i<links.size(); i ++ ) {
                links.at(i)->recalculate();
                links.at(i)->update();                
            };
        
        }
        
        __schemaScene->update();
        __schemaView->update();
        __schemaView->show();
                
    };
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                    Some item was dropped onto the schema view.                                   *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                            Некий элемент был "сброшен" на схему (на ее представление).                           *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::MainWindow::__on_schema_item_was_dropped ( tengu::AbstractEntity * entity, QPoint pos ) {
        
    if ( entity ) {
                
        // Common item.
        // общий элемент.
        
        AbstractEntityItem * item = dynamic_cast<AbstractEntityItem * >( entity );   
        
        if ( item ) {
            item->checkEntity();
        }
                
        // Specified items, which has been created.
        // Специфические элементы, которые были созданы.
        
        // ProcessStartItem * start = dynamic_cast<ProcessStartItem * >(entity);
        // TaskItem * task = dynamic_cast<TaskItem *>(entity);
        // ORerItem * orer = dynamic_cast<ORerItem *>(entity);
        // ANDorItem * andor = dynamic_cast<ANDorItem *>(entity);
        LinkItem * link = dynamic_cast<LinkItem *>(entity);
        // SproutItem * sprout = dynamic_cast<SproutItem * >( entity );
                        
        if ( ( item ) && ( ! link ) ) {
            
            // It was graphical representation somewhat from agents.
            // Это было графическое представление кого-то из агентов.
            item->setX( pos.x() );
            item->setY( pos.y() );
                        
        };
        
        if ( link ) {
            
            // This is a link, not an agent.
            // Это - связь, а не агент.
            
            ItemWithLinks * existing = dynamic_cast<ItemWithLinks * > ( __schemaScene->itemAt( pos, QTransform() ) );
            if ( ( existing ) && ( existing->acceptOutgoingLink() ) ) {
                existing->setSelected( true );
                if ( link->isEmpty() ) existing->addOutgoingLink( link );
                __schemaView->semiCreatedLink = link;
            };
            
        };
        
        __schemaView->hide();
        __schemaScene->addItem( item );
        __schemaView->show();
        
        AbstractAgent * agent = dynamic_cast< AbstractAgent * > ( item->entity() );
        if ( agent ) {
            __left->treeStructure->addAgent( agent , false );        
        };                
    };    
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                        Something has been changed on the schema.                                 *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                             На схеме что-то было изменено.                                       *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::MainWindow::__on__something_changed() {
    
    __action__save_schema->setEnabled( true );

}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                An agent was selected in tree-like structure view.                                *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                        В древовидной структуре был изменен выбранный (текущий ) агент                            *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::MainWindow::__on__tree_structure__agent_was_selected ( tengu::AbstractAgent * agent ) {
        
    if ( __do_not_handle_events ) return;
    
    // An agent which was selected on the tree appears on the schema.
    // Появление выбранного в дереве агента - на схеме.
    
    AbstractEntityItem * item = AgentItemFactory::createItem( agent );
    
    if ( item ) {
        
        __agentPropertyModel->setEntityItem( item );
        __schemaView->hide();
        switch ( agent->entityType() ) {
            
            case AbstractEntity::ET_Task:
            case AbstractEntity::ET_ProcessStart:
            case AbstractEntity::ET_ProcessStop:
            {
                // If we have process'es element clicked on the tree-like structure, we need set as root - parent process, not an element itself.
                // Если в дереве щелкнули на элемент процесса, корневым на схеме будет - процесс, а не сам элемент.
                
                delete( item );
                
                AbstractAgent * parent = agent->parent();
                
                if ( parent ) {
                    AbstractEntityItem * parentItem = AgentItemFactory::createItem( parent );
                    if ( parentItem ) {
                        __schemaScene->setRootItem( parentItem );
                        AbstractEntityItem * agentItem = __schemaScene->itemFor( agent );
                        if ( agentItem ) {
                            agentItem->setSelected( true );
                        }
                    };
                };
                
            }; break;
                
            default: __schemaScene->setRootItem( item );   
        }
        
        __schemaView->show();            
    };
    
    // Actions enabling depended on kind of selected agent.
    // Разрешение действий в зависимости от типа выбранного агента.
    
    __action__simulation_start->setEnabled( false );
    
    AbstractEntity::entity_types_t et = agent->entityType();
    if ( ( et == AbstractEntity::ET_Vehicle ) || ( et == AbstractAgent::ET_Process ) ) {
        __action__simulation_start->setEnabled( true );
    }
            
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                              Set execution mode to real.                                         *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                         Установить режим выполнения в "реальный".                                *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::MainWindow::__on_set_execution_mode_real() {
    
    __execution_mode = AbstractEntity::EM_REAL;
    __schemaView->hide();
    __schemaScene->setExecutionMode( AbstractEntity::EM_REAL );
    __schemaView->show();
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                            Set execution mode to X-Plane.                                        *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                       Установить режим выполнения в "X-Plane"                                    *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::MainWindow::__on_set_execution_mode_xplane() {
    
    __execution_mode = AbstractEntity::EM_XPLANE;
    __schemaView->hide();
    __schemaScene->setExecutionMode( AbstractEntity::EM_XPLANE );
    __schemaView->show();
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                        User want stop a simulation process.                                      *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                  Пользователь хочет остановить процесс симуляции.                                *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::MainWindow::__on__simulation_stop() {

}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                  An agent was created in tree-like structure view.                               *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                       В древовидной структуре был создан агент.                                  *
// *                                                                                                                  *
// ********************************************************************************************************************
/*
void tengu::MainWindow::__on__tree_structure__agent_was_created ( tengu::AbstractAgent * agent ) {

}
*/
// ********************************************************************************************************************
// *                                                                                                                  *
// *                                  Agent was deleted in tree-like structure view.                                  *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                   В древовидной структуре был удален агент.                                      *
// *                                                                                                                  *
// ********************************************************************************************************************
/*
void tengu::MainWindow::__on__tree_structure__agent_was_deleted( QString uuid ) {
}
*/

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                         Request to quit (exit) from program                                      *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                            Требование выхода из программы.                                       *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::MainWindow::__on_quit_request() {
    qApp->quit();
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                         Restore the settings of main window.                                     *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                        Восстановить установки главного окна.                                     *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::MainWindow::__restoreSettings() {
            
    QSettings s ( TENGU_ORGANIZATION, TENGU_MODELLER_APPLICATION );
    s.beginGroup("MainWindow");    
    
    // The size of main window.
    // Размеры главного окна.

    QRect mainWindowGeometry = s.value("MainWindowGeometry", QRect(0,0,0,0) ).toRect();
    if ( 
        ( mainWindowGeometry.height() > 100 ) 
        && ( mainWindowGeometry.width() > 100 ) 
        && ( mainWindowGeometry.x() >= 0 )
        && ( mainWindowGeometry.y() >= 0 )
    ) {
        this->setGeometry( mainWindowGeometry );
    };
    
    // The sizes of splitter ( the width of left and right pieces of screen ).
    // Размеры сплиттера (ширина левой и правой частей экрана).
    
    QList<int> splitterSizes;    
    int lw = s.value("LeftWidth", -1 ).toInt();
    int rw = s.value("RightWidth", -1 ).toInt();
    if ( ( lw < 0 ) || ( lw > size().width() ) ) lw = size().width() / 5;
    if ( ( rw < 0 ) || ( rw > size().width() ) ) rw = size().width() / 5;
    
    splitterSizes.append( lw );
    splitterSizes.append( size().width() - lw - rw );
    splitterSizes.append( rw );
    __hSplitter->setSizes( splitterSizes );
    
    // The heights of right part of the screen (where agent's properties is).
    // Высоты правой части экрана (где свойства агента).
    
    QList<int> rs;
    int rTop  = s.value( "AgentsPropertiesHeight", -1 ).toInt();
    if ( ( rTop <= 0 ) || (rTop > size().height() ) ) rTop = 3 * size().height() / 4;
    rs.append( rTop );
    
    int rBottom = s.value( "RightBottomHeight", -1 ).toInt();
    if ( ( rBottom <= 0 ) || (rBottom > size().height() ) ) rBottom = size().height() / 4;
    rs.append( rBottom );
    
    __right->setComponentsHeight( rs );    
        
    s.endGroup();
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                 Binding the signals of the process before launch.                                *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                 Привязать сигналы процесса перед его запуском.                                   *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::MainWindow::__execution_bind ( tengu::AbstractAgent * agent ) {
    QObject::connect( agent, SIGNAL( signalActivated( bool ) ), this, SLOT( __on__agent__activated( bool ) ) );
    QObject::connect( agent, SIGNAL( signalFocused( bool ) ), this, SLOT( __on__agent__focused( bool ) ) );
    QObject::connect( agent, SIGNAL( signalFinished() ), this, SLOT( __on__agent__finished() ) );
    QObject::connect( agent, SIGNAL( signalFailed( QString ) ), this, SLOT( __on__agent__failed( QString ) ) );
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                        Recursive binding the signals for this agent and for all his children.                    *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                           Рекурсивная привязка сингалов данного агента и всех его детей.                         *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::MainWindow::__execution_bind_recursive ( tengu::AbstractAgent * agent ) {
    
    qDebug() << "MainWindow::bind()";
    QList<AbstractAgent * > elements;
    agent->childrenRecursive( elements );
    elements.append( agent );
        
    for ( int i=0; i<elements.count(); i++ ) {
        __execution_bind( elements.at( i ) );
    };
        
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                               Unbind the signals of the process after stop his running.                          *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                               Отвязать сигналы процесса после окончания его выполнения.                          *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::MainWindow::__execution_unbind ( tengu::AbstractAgent * agent ) {
    QObject::disconnect( agent, SIGNAL( signalActivated( bool ) ), this, SLOT( __on__agent__activated( bool ) ) );
    QObject::disconnect( agent, SIGNAL( signalFocused( bool ) ), this, SLOT( __on__agent__focused( bool ) ) );
    QObject::disconnect( agent, SIGNAL( signalFinished() ), this, SLOT( __on__agent__finished() ) );
    QObject::disconnect( agent, SIGNAL( signalFailed( QString ) ), this, SLOT( __on__agent__failed( QString ) ) );
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                      Recursivelly unbinding the sighals of this agent and all his children                       *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                             Рекурсивно отвязать сигналы этого агента и всех его детей.                           *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::MainWindow::__execution_unbind_recursive ( tengu::AbstractAgent * agent ) {
    
    qDebug() << "MainWindow::unbind()";
    
    QList<AbstractAgent * > elements;
    agent->childrenRecursive( elements );
    elements.append( agent );
    
    for ( int i=0; i<elements.count(); i++ ) {
        __execution_unbind( elements.at( i ) );
    };
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                     The element was opened (taked from database).                                *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                       Элемент был открыт (взят из базы данных).                                  *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::MainWindow::__on__agent__opened ( tengu::AbstractAgent * agent ) {
    
    QObject::connect( agent, SIGNAL( signalSomethingChanged()), this, SLOT( __on__something_changed() ) );
    QObject::connect( agent, SIGNAL( signalSomethingChanged()), __left->treeStructure, SLOT( on__something_changed() ) );
        
    // Insert it into workspace
    // Добавляем его в рабочее пространство.
    
    AbstractAgent * parent = __workSpace;
    AbstractAgent * selectedTreeAgent = __left->treeStructure->selectedAgent();
    
    if ( selectedTreeAgent ) parent = selectedTreeAgent;
    parent->addChild( agent );
    
    __left->treeStructure->addAgent( agent, true );
    __action__save_schema->setEnabled( false );
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                      The focus of the agent has been changed.                                    *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                Фокус агента изменился.                                           *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::MainWindow::__on__agent__focused ( bool focus ) {
    AbstractAgent * agent = dynamic_cast< AbstractAgent * > ( sender() );
    if ( agent ) {
        
        AbstractEntityItem * item = __schemaScene->itemFor( agent );
        if ( item ) item->update();
        
        // qDebug() << "MainWindow::__on_agent_focused, " << agent->getHumanName() << ", " << focus;
    };
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                       The activity of agent has been changed.                                    *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                              Изменилась активность агента.                                       *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::MainWindow::__on__agent__activated ( bool activity ) {
    
    AbstractAgent * agent = dynamic_cast< AbstractAgent * > ( sender() );
    if ( agent ) {
        
        AbstractEntityItem * item = __schemaScene->itemFor( agent );
        if ( item ) item->update();
        
        // qDebug() << "MainWindow::on_agent_activated, " << agent->getHumanName() << ", " << activity ;
    };
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                        The running of the agent has been ended.                                  *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                Агент завершил свою работу.                                       *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::MainWindow::__on__agent__finished() {
    
    AbstractAgent * agent = dynamic_cast< AbstractAgent * > ( sender() );
    if ( agent ) {
        
        AbstractEntityItem * item = __schemaScene->itemFor( agent );
        if ( item ) item->update();        
        
        // qDebug() << "MainWindow::__on_agent_finished(), " << agent->getHumanName();        
        __execution_unbind( agent );        
        __check__simulation_finished( agent );        
        
    };
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                      The running of the agent has been failed.                                   *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                       Выполнение агента закончилось неудачей.                                    *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::MainWindow::__on__agent__failed ( QString errorMessage ) {
    
    __on__error( EL_CRITICAL, "MainWindow::on_agent_failed()", errorMessage );    
    __schemaView->update();
    
    // __on__error( EL_WARNING, "MainWindow::on_agent_failed()", errorMessage );
    AbstractAgent * agent = dynamic_cast< AbstractAgent * > ( sender() );
    if ( agent ) {
        
        
                
        __execution_unbind( agent );
        __check__simulation_finished( agent );
        
    };    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                     Check the simulation has been finished                                       *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                           Проверка того факта, что симуляция как процесс - закончилась                           *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::MainWindow::__check__simulation_finished( AbstractAgent * rAgent ) {
    /*
    if ( rAgent == __running_agent ) {
        qDebug() << "MainWindow::check_simulation_finished, the simulation was ended.";
        __action__simulation_start->setEnabled( true );
        __running_agent = nullptr;
    };
    */
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                     Clear agent. Independed of signal sender.                                    *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                  Очистка агента. Независимо от источника сигнала.                                *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::MainWindow::__on__clear_agent( AbstractAgent * agent ) {
    
    __do_not_handle_events = true;
    
    // Cleanup at the schema.
    // Очистка на схеме.
    
    if ( ( __schemaScene->rootEntity() ) && ( __schemaScene->rootEntity()->getUUID() == agent->getUUID() ) ) {
        __schemaView->hide();
        __schemaScene->clear();        
        __schemaView->show();
    };
            
    // Cleanup in the tree-like structure.
    // Очистка в древовидной структуре.
    
    __left->treeStructure->clearAgent( agent );
        
    // Delete children's elements physically
    // Физическое удаление детей.
    
    agent->deleteChildren();
        
    // If we have process as root, we need add ProcessStart element at least
    // Если у нас в качестве корневого - процесс, то нужно как минимум добавить ему элемент начала процесса
    
    __check_start_element_in_process( agent );
    __action__save_schema->setEnabled( false );
    __do_not_handle_events = false;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                              Some errors occured.                                                *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                            Возникли какие-то ошибки.                                             *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::MainWindow::__on__error ( tengu::error_level_t errorLevel, QString procedureName, QString errorMessage ) {
    QMessageBox msg;
    msg.setText( procedureName + "\n" + errorMessage );
    switch ( errorLevel ) {
        case EL_INFO: msg.setIcon( QMessageBox::Information ); break;
        case EL_WARNING: msg.setIcon( QMessageBox::Warning ); break;
        case EL_CRITICAL: msg.setIcon( QMessageBox::Critical ); break;
    };
    msg.exec();
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                Save to database.                                                 *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                             Сохранение в базу данных                                             *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::MainWindow::__on__save() {
    
    // Perform not stored operations
    // Выполнение не-записанных операций.
    
    for ( int i=0; i<__not_saved_operations.count(); i++ ) {
        NotSavedOperation op = __not_saved_operations.at(i);
        switch ( op.operation ) {
            case NotSavedOperation::OT_DELETE: {
                
                if ( __mongo->storageable( op.object ) ) {
                    __mongo->remove( op.object );
                }
                
            }; break;
            default: __on__error( EL_WARNING, "MainWindow::on_save", tr("Unknown not-stored operation: ") + QString::number(op.operation) );
        };
    };    
    __not_saved_operations.clear();
    
    QList< AbstractAgent * > elements = __workSpace->children();
    
    for ( int ei=0; ei<elements.size(); ei++ ) {
        
        AbstractAgent * element = elements.at( ei );
        
        if ( element->hasChanged() )  {
            
            // Not every each existing element must be stored.
            // Не каждый существующий элемент должен быть записан.
            
            if ( __mongo->storageable( element ) ) {
                __mongo->store( element );
                element->unmodify();
            };
        };
    };
    
    // The store action become to disabled status after store process.
    // После записи действие записи становится недоступным.
    
    __action__save_schema->setEnabled( false );
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                       User want jump to begin of simulation.                                     *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                   Пользователь хочет перейти на начало симуляции.                                *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::MainWindow::__on__simulation_begin() {

}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                         User want pause simulation process.                                      *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                Пользователь хочет поставить процесс симуляции на паузу.                          *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::MainWindow::__on__simulation_pause() {

}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                         User want start a simulation process.                                    *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                      Пользователь хочет начать процесс симуляции.                                *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::MainWindow::__on__simulation_start() {
    
    AbstractAgent * treeAgent = __left->treeStructure->selectedAgent();
    if ( treeAgent ) {
        
        Process * process = dynamic_cast< Process * > ( treeAgent );
        Vehicle * vehicle = dynamic_cast< Vehicle * > ( treeAgent );
        
        if ( ( process ) || ( vehicle ) ) {
            
            __execution_bind_recursive( treeAgent );
            treeAgent->start();
            
        };
    };
    
    /*
    if ( __schemaScene->rootAsProcess() ) {
        
        Process * schema_root_process = dynamic_cast< Process * > ( __schemaScene->rootEntity() );
        if ( schema_root_process ) {
        
            // We have an process as root element of the schema.
            // В качестве корневого элемента на схеме - процесс.
        
            __running_agent = schema_root_process;
            __action__simulation_start->setEnabled( false );
        
            __execution_bind_recursive( schema_root_process );
        
            schema_root_process->start();
        
        };    
    };
    */
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                           Someone want create an agent.                                          *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                            Кто-то хочет создать агента.                                          *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::MainWindow::__on__want__create_agent( AbstractAgent * parent, AbstractEntity::entity_types_t type ) {

    QJsonObject json;
    
    switch ( type ) {
        case AbstractEntity::ET_Vehicle: json[ JSON_CLASS_NAME_ELEMENT ] = "Vehicle"; break;
        case AbstractEntity::ET_Process: json[ JSON_CLASS_NAME_ELEMENT ] = "Process"; break;
        case AbstractEntity::ET_Task: json[ JSON_CLASS_NAME_ELEMENT ] = "Task"; break;
        default: __on__error( EL_WARNING, "MainWindow::on__want__create_agent()", "Unknown agent type" + QString::number( type ) );
    };
    
    AbstractAgent * agent = dynamic_cast<AbstractAgent * > ( AgentItemFactory::createEntity( json ) );
    
    if ( agent ) {
        
        QObject::connect( agent, SIGNAL(signalSomethingChanged()), this, SLOT( __on__something_changed() ) );
        QObject::connect( agent, SIGNAL( signalSomethingChanged()), __left->treeStructure, SLOT( on__something_changed() ) );
        
        // Coordinates of new created agent
        // Координаты созданного агента.
        
        QObject * s = sender();
        SchemaView * sw = dynamic_cast<SchemaView * >( s );
        if ( sw ) {
            QPoint schMouse =  __schemaView->mouseAtSchema();
            agent->setX( schMouse.x() );
            agent->setY( schMouse.y() );
        };
        
        if ( parent ) parent->addChild( agent );
        else __workSpace->addChild( agent );
        
        // Add new agent to tree-like structure
        // Добавить нового агента в древовидную структуру.
        
        __left->treeStructure->addAgent( agent, true );                
        
        // We need not add element to the schema, because it will be repaint automatically due tree structure focus.
        // На схему добавлять - не нужно. Перерисуется само из-за фокуса в дереве.
        
        __check_start_element_in_process( agent );
        
    } else __on__error( 
        EL_WARNING, 
        "MainWindow::on__want__create_agent()", 
        "Agent was not ben created, type=" + QString::number( type ) + ", class_name=" + json[ JSON_CLASS_NAME_ELEMENT ].toString() 
    );
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                         Someone want to delete something (entity, not an agent only)                             *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                  Кто-то хочет что-то удалить ( не обязательно агента, любую "сущность" )                         *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::MainWindow::__on__want__delete ( tengu::AbstractEntity * entity ) {
    
    qDebug() << "MainWindow::__on_want_delete";
    
    AbstractAgent * agent = dynamic_cast<AbstractAgent * >( entity );
    if ( agent ) {
        
        // find parent in tree-like structure to later selection 
        // Поиск родителя в древовидной структуре для последующего выделения.
        
        QTreeWidgetItem * treeParent = nullptr;
        QTreeWidgetItem * hisTreeItem = __left->treeStructure->itemFor( agent );        
        if ( hisTreeItem ) treeParent = hisTreeItem->parent();
        
        __left->treeStructure->deleteAgent( agent );
        
        // Select parent of deleted agent in tree structure.
        // Выделение родителя удаленного агента в дереве.
        
        if ( treeParent ) {
            __left->treeStructure->setCurrentItem( treeParent );
        };
        
        // Delete an agent from the schema.
        // Удаление агента из схемы.
        
        AbstractEntityItem * schemaItem = __schemaScene->itemFor( entity );
        if ( schemaItem ) {
            __schemaView->hide();
            __schemaScene->removeItem( schemaItem );
            __schemaView->show();
        };
        
        // Delete agent and his children in the workspace
        // Удаление агента и его детей из рабочего пространства.
    
        QList<AbstractAgent * > rch;
        agent->childrenRecursive( rch );
        
        // To delete in one pass
        // Для удаления в один проход.
        
        rch.append( agent );
        
        for ( int i=0; i<rch.count(); i++ ) {
            AbstractAgent * one = rch.at(i);
            QObject::disconnect( one, SIGNAL( signalSomethingChanged() ), this, SLOT( __on__something_changed() ) );
            QObject::disconnect( one, SIGNAL( signalSomethingChanged() ), __left->treeStructure, SLOT( on__something_changed() ) );
            NotSavedOperation oper ( NotSavedOperation::OT_DELETE, one->toJSON() );
            __not_saved_operations.append( oper );
            
            delete( one );
        };
        
        
    };
            
    
    /*
    QTreeWidgetItem * treeItem = __left->treeStructure->itemFor( entity );
    if ( treeItem ) {
        qDebug() << "В дереве - нашли, его - удаляем. ";        
        tree->dedelete ( treeItem );
    };
    */
    
    /*
    hide();
        ItemWithLinks * itemWithLinks = dynamic_cast< ItemWithLinks * > ( __contextMenuItem );
        if ( itemWithLinks ) {
            
            // Links of this item. Will be removed simultaneously with him.
            // Связи данного элемента - будут удалены совместно с ним.
             
            QList< LinkItem *> links = itemWithLinks->hisLinks();
            for ( int i=0; i<links.size(); i++ ) {                
                scene()->removeItem( links.at(i) );
                delete( links.at(i) );                
            };
        
        }
        
        scene()->removeItem( __contextMenuItem );
        delete( __contextMenuItem );
        __contextMenuItem = nullptr;        
        show();
    */    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                               Show main window event.                                            *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                           Событие появления окна на экране.                                      *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::MainWindow::showEvent( QShowEvent * event ) {
    
    QMainWindow::showEvent( event );    
    
    __restoreSettings();   
                
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                               Close main window event.                                           *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                            Событие закрытия главного окна.                                       *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::MainWindow::closeEvent ( QCloseEvent* event ) {
    
    QWidget::closeEvent ( event );
    
    
    QSettings s ( TENGU_ORGANIZATION, TENGU_MODELLER_APPLICATION );
    
    s.beginGroup("MainWindow");
    
    s.setValue("MainWindowGeometry", geometry() );
    
    s.setValue( "LeftWidth", __left->size().width() );
    s.setValue( "RightWidth", __right->size().width() );
    
    QPair<int,int> right = __right->getComponentsHeights();
    s.setValue( "AgentsPropertiesHeight", right.first );
    s.setValue( "RightBottomHeight", right.second );
    
    s.endGroup();
    
    s.sync();
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                   Key press event.                                               *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                               Событие нажатия клавиши.                                           *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::MainWindow::keyPressEvent ( QKeyEvent* event ) {
        
    QWidget::keyPressEvent ( event );
    
    if ( event->key() == Qt::Key_Escape ) {
    
        // It was an Escape key.
        // Это была кнопка Escape.
        
        // Delete links that were not completely created.
        // Удаление связей, которые были созданы не до конца.
        
        __schemaScene->removeSemiCreatedLinks();
        __schemaView->semiCreatedLink = nullptr;
        __schemaView->update();
                
    };
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                   The destructor.                                                *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                      Деструктор.                                                 *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::MainWindow::~MainWindow() {
}
