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
    
    QVBoxLayout * lay = new QVBoxLayout();
    lay->setMargin( 0 );
    lay->setSpacing( 0 );
    
    __centerWidget = new QWidget();
    __centerWidget->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding ) );
    __centerWidget->setMinimumSize( QSize(800, 600 ) );
    __centerWidget->setLayout( lay );
    
    setCentralWidget( __centerWidget );
    
    __left_schema_splitter = new QSplitter();
    __left_schema_splitter->setHandleWidth( 3 );
    __left_schema_splitter->setLineWidth( 2 );
    
    lay->addWidget( __left_schema_splitter );
    
    __canvas = new QGraphicsScene();
    __left = new MainWindowLeft();
    __schema = new MainWindowSchema( __canvas );
    
    __left_schema_splitter->addWidget( __left );
    __left_schema_splitter->addWidget( __schema );
            
    __createActions();
    __createMainMenu();
    __createToolBar();
    __createStatusBar();
    
    __restoreSettings();    
    
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
// *                                                The toolbar constructor.                                          *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                           Конструктор инструментальной панели.                                   *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::MainWindow::__createToolBar() {
    
    __toolbar_buttons = new QToolBar();
    addToolBar( Qt::TopToolBarArea, __toolbar_buttons );
    
    addToolBarBreak();
    
    __toolbar_elements_library = new QToolBar();
    addToolBar( Qt::TopToolBarArea, __toolbar_elements_library );
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
    
    // The sizes of splitter ( left piece width of screen ).
    // Размеры сплиттера (ширина левой части экрана).
    
    int lw = s.value("LeftWidth", -1 ).toInt();
    if ( ( lw > 0 ) && ( lw < size().width() ) ) {
        
        // Somewhat exists in the settings. And sizes seems correctly.
        // Что-то в установках было. И оно было вменябельным по размерам.
        
        QList<int> sizes;
        sizes.append( lw );
        sizes.append( size().width() - lw );
        __left_schema_splitter->setSizes( sizes );
        
    } else {
        
        // There was nothing about splitter in the settings.
        
        // Не было ничего в установках по поводу сплиттера.
        // Отношения размеров по умолчанию.
    
        __left_schema_splitter->setStretchFactor( 0, 1 );
        __left_schema_splitter->setStretchFactor( 1, 5 );

    }
    
    s.endGroup();
    
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
    
    
    // QList<int> sizes = __left_schema_splitter->sizes();
    
    QSettings s ( TENGU_ORGANIZATION, TENGU_MODELLER_APPLICATION );
    
    s.beginGroup("MainWindow");
    s.setValue("MainWindowGeometry", geometry() );
    
    s.setValue("LeftWidth", __left->size().width() );
    
//    s.beginWriteArray("LeftSchemaSplitter");    
//    for ( int i=0; i<sizes.length(); i++ ) {
//        s.setArrayIndex( i );
//        s.setValue("pos", sizes.at(i) );
//    };  
//    s.endArray();
    
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
