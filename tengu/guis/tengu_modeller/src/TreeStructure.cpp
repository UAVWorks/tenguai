// ********************************************************************************************************************
// *                                                                                                                  *
// *                              Tree-like representation of the structure of the workspace                          *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                              Древовидное представление структуры рабочего пространства.                          *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 24 jun 2017 at 14:55 *
// ********************************************************************************************************************

#include "TreeStructure.h"

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                  The constructor.                                                *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                    Конструктор.                                                  *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::TreeStructure::TreeStructure ( tengu::WorkSpace* ws ) 
    : QTreeWidget()
{
    header()->close();
    setColumnCount(1);
    
    __workSpace = ws;
    __selectedItem = nullptr;
    __selectedAgent = nullptr;
    __do_not_handle_events = false;    
    
    // Create root tree item.
    // Создание "корневого узла" дерева.
    
    __rootItem = new QTreeWidgetItem( WorkSpace::GI_WorkSpace );
    __rootItem->setText(0, tr("The Workspace") );
    __rootItem->setIcon(0, QIcon( QPixmap(":chart_organisation_16.png") ) );
    __rootItem->setData( 0, Qt::UserRole, QVariant::fromValue<AbstractAgentKernel * >( ws ) );
    
    addTopLevelItem( __rootItem );
    setCurrentItem( __rootItem );
    
    __create_actions();
    
    QObject::connect( selectionModel(), SIGNAL(selectionChanged( const QItemSelection & , const QItemSelection & )), this, SLOT( __on__tree_item_selected( const QItemSelection & , const QItemSelection & ) ) );
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                              Action's constructor.                                               *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                Создание действий.                                                *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::TreeStructure::__create_actions() {
    
    __action__create__vehicle = new QAction( QIcon(QPixmap(":robot_16.png")) , tr("Vehicle"), this);
    QObject::connect( __action__create__vehicle, SIGNAL( triggered() ), this, SLOT( __on__create__vehicle() ) );
    
    __action__create__process = new QAction( QIcon( QPixmap(":package_add_16.png") ), tr("Process"), this );
    QObject::connect( __action__create__process, SIGNAL( triggered() ), this, SLOT( __on__create__process() ) );
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                     Overrided context menu event handler.                                        *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                Перекрытый обработчик события контекстного меню.                                  *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::TreeStructure::contextMenuEvent ( QContextMenuEvent * event ) {
    
    QMenu contextMenu;
    QMenu createMenu;
    QTreeWidgetItem * curItem = itemAt( event->pos() );
    
    if ( curItem ) {
        
        switch ( curItem->type() ) {
            
            case WorkSpace::GI_Vehicle: {
                createMenu.addAction( __action__create__process );
            }; break;
            
            case WorkSpace::GI_WorkSpace: {
                createMenu.addAction( __action__create__vehicle );
            }; break;
                        
        };
        
    };
    
    if ( ! createMenu.isEmpty() ) {
        QAction * aCreate = contextMenu.addMenu( & createMenu );
        aCreate->setText( tr("Create") );
        aCreate->setIcon( QIcon( QPixmap( ":chart_organisation_add_16.png" ) ) );
    };
    
    if ( ! contextMenu.isEmpty() ) contextMenu.exec( event->globalPos() );
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                              Forcibly selection of item with expanding a parent item                             *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                            Принудительный выбор элемента с раскрытием родительского узла.                        *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::TreeStructure::__select_forcibly_with_expanding ( QTreeWidgetItem * parent, QTreeWidgetItem * selected ) {
    
    __do_not_handle_events = true;
    
    if ( ! parent->isExpanded() ) parent->setExpanded( true );    
    setCurrentItem( selected );    
    __selectedItem = selected;
    AbstractAgentKernel * agent = qvariant_cast< AbstractAgentKernel * > ( selected->data( 0, Qt::UserRole ) );
    __selectedAgent = agent ;
    if ( agent ) emit signalAgentSelected( agent );
        
    __do_not_handle_events = false;
    

}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                   We want create a vehicle (a mobile robot)                                      *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                       Мы хотим создать мобильного робота                                         *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::TreeStructure::__on__create__vehicle() {
    
    Vehicle * vehicle = new Vehicle();
    __workSpace->addChild( vehicle );
    
    QTreeWidgetItem * item = new QTreeWidgetItem( WorkSpace::GI_Vehicle );
    item->setText( 0, vehicle->getHumanName() );
    item->setIcon( 0, QIcon( QPixmap( ":robot_16.png" )));
    item->setData( 0, Qt::UserRole, QVariant::fromValue< AbstractAgentKernel * >( vehicle ) );
    
    __rootItem->addChild( item );
    __select_forcibly_with_expanding( __rootItem, item );
    
    emit signalAgentCreated( vehicle );
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                               Create the process                                                 *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                Создание процесса.                                                *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::TreeStructure::__on__create__process() {
        
    if ( ( __selectedItem ) && ( __selectedAgent ) ) {
        
        Process * process = new Process();
        __selectedAgent->addChild( process );
        
        // Add a ProcessStart item to new process.
        // Добавление ProcessStart элемента в процесс.
        
        ProcessStart * processStart = new ProcessStart();
        processStart->setSystemName( process->getSystemName() );
        processStart->setHumanName( process->getHumanName() );
        process->addChild( processStart );
                
        QTreeWidgetItem * item = new QTreeWidgetItem( WorkSpace::GI_Process );
        item->setText( 0, process->getHumanName() );
        item->setIcon( 0, QIcon( QPixmap(":package_16.png") ) );
        item->setData( 0, Qt::UserRole, QVariant::fromValue< AbstractAgentKernel * > ( process ) );
        __selectedItem->addChild( item );
        __select_forcibly_with_expanding( __selectedItem, item );
        emit signalAgentCreated( process );
        
    };
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                         Change selection in the tree.                                            *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                      Изменение выбранного элемента в дереве.                                     *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::TreeStructure::__on__tree_item_selected ( const QItemSelection & current, const QItemSelection & previous ) {
    
    Q_UNUSED( current );
    Q_UNUSED( previous );
    
    if ( __do_not_handle_events ) return;
    
    __selectedItem = nullptr;
    __selectedAgent = nullptr;
    
    QList<QTreeWidgetItem * > sList = selectedItems();
    if ( sList.count() > 0 ) {
        __selectedItem = sList.at( 0 );
        if ( __selectedItem ) {
            __selectedAgent = qvariant_cast< AbstractAgentKernel * >( __selectedItem->data( 0, Qt::UserRole ) );
            if ( __selectedAgent ) emit signalAgentSelected( __selectedAgent );
        };
    };        
}


// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                   The destructor.                                                *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                     Деструктор.                                                  *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::TreeStructure::~TreeStructure() {

}

