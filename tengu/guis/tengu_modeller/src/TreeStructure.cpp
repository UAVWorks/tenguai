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
    
    __rootItem = new QTreeWidgetItem( AbstractEntity::ET_WorkspaceTreeItem );
    __rootItem->setText(0, tr("The Workspace") );
    __rootItem->setIcon(0, QIcon( QPixmap(":chart_organisation_16.png") ) );
    __rootItem->setData( 0, Qt::UserRole, QVariant::fromValue<AbstractAgent * >( ws ) );
    
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
    
    __action__clear = new QAction( QIcon(QPixmap(":Eraser_16.png")), tr("Clear"), this );
    QObject::connect( __action__clear, SIGNAL( triggered() ), this, SLOT( __on__clear() ) );
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
            
            case AbstractEntity::ET_VehicleTreeItem : {
                createMenu.addAction( __action__create__process );
            }; break;
            
            case AbstractEntity::ET_WorkspaceTreeItem: {
                createMenu.addAction( __action__create__vehicle );
            }; break;
                        
        };
        
    };
    
    if ( ! createMenu.isEmpty() ) {
        QAction * aCreate = contextMenu.addMenu( & createMenu );
        aCreate->setText( tr("Create") );
        aCreate->setIcon( QIcon( QPixmap( ":chart_organisation_add_16.png" ) ) );
    };
    
    contextMenu.addAction( __action__clear );
    
    if ( ! contextMenu.isEmpty() ) contextMenu.exec( event->globalPos() );
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                               Add an agent to tree                                               *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                              Добавить агента в дерево.                                           *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::TreeStructure::addAgent( AbstractAgent * agent, bool focusToHim ) {
    
    if ( ! agent ) return;
    
    // First we try to find his parent in the tree.
    // Сначала пытаемся найти его родителя в дереве.
    
    QTreeWidgetItem * parentItem = __rootItem;
    
    if ( agent->parent() ) {
        QList<QTreeWidgetItem * > treeItems = getAllItems();
        for ( int i=0; i<treeItems.count(); i++ ) {
            QTreeWidgetItem * item = treeItems.at( i );
            AbstractAgent * treeAgent = qvariant_cast< AbstractAgent * >( item->data( 0, Qt::UserRole ) );
            if ( ( treeAgent ) && ( treeAgent->getUUID() == agent->parent()->getUUID() ) ) {
                parentItem = item;
                break;
            }
        }
    };
    
    // The constructor itself goes later because there will be an element type.
    // Сам конструктор идет потом, потому что там указывается тип элемента.
    
    QTreeWidgetItem * item = nullptr; 
    
    switch ( agent->entityType() ) {
        
        case AbstractEntity::ET_Vehicle: {
            item = new QTreeWidgetItem( AbstractEntity::ET_VehicleTreeItem );
            item->setIcon( 0, QIcon( QPixmap( ":robot_16.png") ) );
        }; break;
        
        case AbstractEntity::ET_Process: {
            item = new QTreeWidgetItem( AbstractEntity::ET_ProcessTreeItem );
            item->setIcon( 0, QIcon( QPixmap( ":package_add_16" ) ) );
        }; break;
        
        case AbstractEntity::ET_ProcessStart: {
            item = new QTreeWidgetItem( AbstractEntity::ET_ProcessStartTreeItem );
            item->setIcon( 0, QIcon( QPixmap(":process_begin_16.png") ));
        }; break;
        
        case AbstractEntity::ET_ProcessStop: {
            item = new QTreeWidgetItem( AbstractEntity::ET_ProcessStopTreeItem );
            item->setIcon( 0, QIcon( QPixmap(":process_end_16.png") ));
        }; break;
        
        case AbstractEntity::ET_Task: {
            item = new QTreeWidgetItem( AbstractEntity::ET_TaskTreeItem );
            item->setIcon( 0, QIcon( QPixmap( ":box_16.png" ) ) );
        }; break;                
        
        default: qDebug() << "TreeStructure::addAgent, unhandled type " << (int) agent->entityType();
    };
    
    // Set the text and add element to the parent if element has been created. 
    // Если элемент был порожден - устанавливаем ему текст и добавляем в родителя.
    
    if ( item ) {
        
        item->setData( 0, Qt::UserRole, QVariant::fromValue<AbstractAgent * >( agent ) );
        item->setText( 0, agent->getHumanName() );
        
        parentItem->addChild( item );
        
        if ( focusToHim ) {
            __select_forcibly_with_expanding( parentItem, item );
        };
    };  
    
    if ( agent->hasChildren() ) {
        QList< AbstractAgent * > hisChildren = agent->children();
        for ( int i=0; i<hisChildren.count(); i++ ) {
            addAgent( hisChildren.at(i), false );
        };
    };
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                             Recursively delete an agent itself and all his children.                             *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                            Рекурсивное удаление агента как такового и всех его детей.                            *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::TreeStructure::deleteAgent ( tengu::AbstractAgent * agent ) {
    
    // His children firstly.
    // Сначала - всех его детей.
    
    if ( agent->hasChildren() ) {
        QList<AbstractAgent * > his_children = agent->children();
        for ( int i=0; i<his_children.count(); i++ ) {
            AbstractAgent * child = his_children.at( i );
            deleteAgent( child );
        };
    }
    
    // Agent itself.
    // Сам агент.            
    
    QList<QTreeWidgetItem * > all_items = getAllItems();
    
    for ( int i=0; i<all_items.count(); i++ ) {
        QTreeWidgetItem * item = all_items.at(i);
        AbstractAgent * item_agent = qvariant_cast<AbstractAgent * >( item->data(0, Qt::UserRole ) );        
        if ( ( item_agent ) && ( item_agent->getUUID()  == agent->getUUID() ) ) {
            delete( item );
            break;
        };
    };
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                   Clear agent (remove all his children from tree )                               *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                 Очистка агента (удаление всех его детей из дерева)                               *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::TreeStructure::clearAgent ( tengu::AbstractAgent * agent ) {
    QList<AbstractAgent * > his_children = agent->children();
    for ( int i=0; i<his_children.count(); i++ ) {
        deleteAgent( his_children.at(i) );
    };
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                         Get selected agent (not tree item)                                       *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                             Вернуть выбранного в дереве агента (а не элемент дерева)                             *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::AbstractAgent * tengu::TreeStructure::selectedAgent() {
    return __selectedAgent;
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
    AbstractAgent * agent = qvariant_cast< AbstractAgent * > ( selected->data( 0, Qt::UserRole ) );
    __selectedAgent = agent ;
    if ( agent ) emit signalAgentSelected( agent );
        
    __do_not_handle_events = false;
    

}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                Clear selected element                                            *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                               Очистка текущего элемента.                                         *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::TreeStructure::__on__clear() {

    if ( __selectedAgent ) {
        emit signalClearAgent( __selectedAgent );
    };
    
    /*
    if ( __selectedItem ) {
        
        while ( __selectedItem->childCount() > 0 ) {
        
            QTreeWidgetItem * child = __selectedItem->child( 0 );
            __selectedItem->removeChild( child );
        };
        
    };
    
    if ( ( __selectedAgent ) && ( __selectedAgent->hasChildren() ) ) {
        
        QList< AbstractAgent * > children = __selectedAgent->children();
        for ( int i=0; i<children.count(); i++ ) {
            AbstractAgent * child = children.at(i);
            emit signalAgentDeleted( child->getUUID() );
            __selectedAgent->removeChild( child );
            delete( child );
            child = nullptr;
        };
        
    };
    */
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                   We want create a vehicle (a mobile robot)                                      *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                       Мы хотим создать мобильного робота                                         *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::TreeStructure::__on__create__vehicle() {
    /*
    Vehicle * vehicle = new Vehicle();
    __workSpace->addChild( vehicle );
    
    QTreeWidgetItem * item = new QTreeWidgetItem( WorkSpace::GI_Vehicle );
    item->setText( 0, vehicle->getHumanName() );
    item->setIcon( 0, QIcon( QPixmap( ":robot_16.png" )));
    item->setData( 0, Qt::UserRole, QVariant::fromValue< AbstractAgent * >( vehicle ) );
    
    __rootItem->addChild( item );
    __select_forcibly_with_expanding( __rootItem, item );
    
    emit signalAgentCreated( vehicle );
    */
    emit signalWantCreateAgent( __selectedAgent, AbstractEntity::ET_Vehicle );
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                               Create the process                                                 *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                Создание процесса.                                                *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::TreeStructure::__on__create__process() {
    
    /*    
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
        item->setData( 0, Qt::UserRole, QVariant::fromValue< AbstractAgent * > ( process ) );
        __selectedItem->addChild( item );
        __select_forcibly_with_expanding( __selectedItem, item );
        emit signalAgentCreated( process );
        
    };
    */
    
    emit signalWantCreateAgent( __selectedAgent, AbstractEntity::ET_Process );
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
            __selectedAgent = qvariant_cast< AbstractAgent * >( __selectedItem->data( 0, Qt::UserRole ) );
            if ( __selectedAgent ) emit signalAgentSelected( __selectedAgent );
        };
    };        
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                  Append an item and all his children recursive.                                  *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                Добавить элемент и - рекурсивно - всех его детей.                                 *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::TreeStructure::__iterable_append_item( QList<QTreeWidgetItem * > * result, QTreeWidgetItem * item ) {

    result->append( item );
    if ( item->childCount() > 0 ) {
        for ( int i=0; i<item->childCount(); i++ ) {
            QTreeWidgetItem * oci = item->child( i );
            __iterable_append_item( result, oci );
        };
    }
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                       Get all elements from tree structure                                       *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                         Получить все элементы структуры.                                         *
// *                                                                                                                  *
// ********************************************************************************************************************

QList<QTreeWidgetItem * > tengu::TreeStructure::getAllItems() {
    QList<QTreeWidgetItem * > result;
    __iterable_append_item( & result, __rootItem );
    return result;
};

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                   The destructor.                                                *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                     Деструктор.                                                  *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::TreeStructure::~TreeStructure() {

}

