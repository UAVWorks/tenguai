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
    
    setWindowTitle( tr("Tengu modeller") );
    setWindowIcon( QIcon( QPixmap(":tengu_32.png") ) );
    
    __workSpace = new WorkSpace();
    __agentPropertyModel = new AgentPropertyModel();
    
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
    
    __createSchemaScene();
            
    Task * task = new Task();
    task->setName( "The task" );
    TaskItem * i = new TaskItem( task );
    i->setX( 10 );
    i->setY( 10 );
    __schemaScene->addItem( i );
    
    Sprout * s = new Sprout( task );
    SproutItem * si = new SproutItem( s );
    si->setX( 10 );
    si->setY( -80 );
    __schemaScene->addItem( si );
    
    ProcessStart * pStart = new ProcessStart();
    ProcessStartItem * pi = new ProcessStartItem( pStart );
    pi->setX( 0 );
    pi->setY( 300 );
    __schemaScene->addItem( pi );
    
    ProcessStop * pStop = new ProcessStop();
    ProcessStopItem * pStopItem = new ProcessStopItem( pStop );
    pStopItem->setX( 500 );
    pStopItem->setY( 300 );
    __schemaScene->addItem( pStopItem );
        
    /*
    VehicleItem * w = new VehicleItem( new Vehicle( nullptr, "Vehicle") );
    __canvas->addItem( w );
    w->setX( -20 );
    w->setY( 50 );
    AbstractAgentItem * a = new AbstractAgentItem( __topModel );
    __canvas->addItem( a );
    a->setX( 20 );
    a->setY( -50 );
    */

    __left = new MainWindowLeft();
    __createSchemaView();
    
    __right = new MainWindowRight();
    
    __right->propertyView->setModel( __agentPropertyModel );
    
    __hSplitter->addWidget( __left );
    __hSplitter->addWidget( __schemaView );
    __hSplitter->addWidget( __right );        
            
    __createActions();
    __createLibraryTab();
    __createMainMenu();
    __createToolBar();
    __createStatusBar();    
    
    __mongo = new MongoStorage();
    
    // __mongo->store( task );
    
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
// *                                               The main menu constructor.                                         *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                               Конструктор главного меню.                                         *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::MainWindow::__createMainMenu() {
    
    QMenuBar * mb = menuBar();
    
    QMenu * menuMain = new QMenu("File");
    menuMain->addAction( __actionQuit );
    mb->addMenu( menuMain );
    
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
// *                               Create component's library tabulator for bottom toolbar.                           *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                            Создание табулятора библиотеки элементов для нижнего тулбара.                         *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::MainWindow::__createLibraryTab() {
    __library_tab = new LibraryTab();
}


// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                The toolbar constructor.                                          *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                           Конструктор инструментальной панели.                                   *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::MainWindow::__createToolBar() {
    
    __toolbar_file = new QToolBar();
    
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
// *                           The constructor of model's schama (graphical view of the model).                       *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                               Конструктор схемы (графического представления модели).                             *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::MainWindow::__createSchemaView() {
    __schemaView = new SchemaView( __schemaScene );
    QObject::connect( __schemaView, SIGNAL( signalItemPressed( AbstractEntityItem *, bool )), this, SLOT( __on_schema_item_pressed ( AbstractEntityItem *, bool ) ) );
    QObject::connect( __schemaView, SIGNAL( signalItemDoubleClicked( AbstractEntityItem *, bool )), this, SLOT( __on_schema_item_double_clicked( AbstractEntityItem *, bool ) ) );
    QObject::connect( __schemaView, SIGNAL( signalItemDragging( AbstractEntityItem * , QPoint )), this, SLOT( __on_schama_item_dragged( AbstractEntityItem * , QPoint ) ) );
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
    
    // We always have invisible X-Plane schema item for modelling.
    // У нас всегда есть невидимый компонент схемы - X-Plane. Он для моделирования.
    
    XPlaneAgent * xplane = new XPlaneAgent();
    XPlaneAgentItem * xpItem = new XPlaneAgentItem( xplane );
    xpItem->setX( 0 );
    xpItem->setY( 0 );
    __schemaScene->addItem( xpItem );

}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                              Some element was (once or single) clicked on the schema.                            *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                         На схеме был нажат какой-то элемент.                                     *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::MainWindow::__on_schema_item_pressed ( tengu::AbstractEntityItem * item, bool controlPressed ) {
    
    if ( ! controlPressed ) __schemaScene->unselectAll();
    
    // Selecting of the agent.
    // Выбор агента.
    
    item->setSelected( true );
    
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

}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                       Entity element was dragged at the schema.                                  *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                "Сущность" тащат по схеме.                                        *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::MainWindow::__on_schama_item_dragged ( tengu::AbstractEntityItem * entity, QPoint pos ) {
    
    if ( entity ) {
        entity->setX( pos.x() );
        entity->setY( pos.y() );
        entity->update(); 
        // currentPos = __schemaView->mapFromScene( currentPos );
        // qDebug() << "After map to scene: " << currentPos;
    };
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                              Set execution mode to real.                                         *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                         Установить режим выполнения в "реальный".                                *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::MainWindow::__on_set_execution_mode_real() {

}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                            Set execution mode to X-Plane.                                        *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                       Установить режим выполнения в "X-Plane"                                    *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::MainWindow::__on_set_execution_mode_xplane() {

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
// *                                         User want start a simulation process.                                    *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                      Пользователь хочет начать процесс симуляции.                                *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::MainWindow::__on__simulation_start() {

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
// *                                        User want stop a simulation process.                                      *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                  Пользователь хочет остановить процесс симуляции.                                *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::MainWindow::__on__simulation_stop() {

}

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
// *                                               Show main window event.                                            *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                           Событие появления окна на экране.                                      *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::MainWindow::showEvent( QShowEvent * event ) {
    __restoreSettings();
    QMainWindow::showEvent( event );    
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
// *                                                   The destructor.                                                *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                      Деструктор.                                                 *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::MainWindow::~MainWindow() {
}
