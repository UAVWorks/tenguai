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
    __rootItem = new QTreeWidgetItem( WorkSpace::GI_WorkSpace );
    __rootItem->setText(0, tr("The Workspace") );
    __rootItem->setIcon(0, QIcon( QPixmap(":chart_organization_16.png") ) );
    __rootItem->setData( 0, Qt::UserRole, QVariant( ws->getUUID() ) );
    
    addTopLevelItem( __rootItem );
    setCurrentItem( __rootItem );
    
    __create_actions();
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                              Action's constructor.                                               *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                Создание действий.                                                *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::TreeStructure::__create_actions() {
    __action__create__vehicle = new QAction( QIcon(QPixmap(":robot_16.png")) , tr("Mobile robot"), this);
    QObject::connect( __action__create__vehicle, SIGNAL( triggered() ), this, SLOT( __on__create__vehicle() ) );
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                     Overrided context menu event handler.                                        *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                Перекрытый обработчик события контекстного меню.                                  *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::TreeStructure::contextMenuEvent ( QContextMenuEvent * event ) {
    // QAbstractScrollArea::contextMenuEvent ( event );
    QMenu contextMenu;
    QMenu createMenu;
    QTreeWidgetItem * curItem = itemAt( event->pos() );
    if ( curItem ) {
        switch ( curItem->type() ) {
            case WorkSpace::GI_WorkSpace: {
                createMenu.addAction( __action__create__vehicle );
            }; break;
        };
    };
    if ( ! createMenu.isEmpty() ) {
        QAction * aCreate = contextMenu.addMenu( & createMenu );
        aCreate->setText( tr("Create") );
    };
    if ( ! contextMenu.isEmpty() ) contextMenu.exec( event->globalPos() );
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                   We want create a vehicle (a mobile robot)                                      *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                       Мы хотим создать мобильного робота                                         *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::TreeStructure::__on__create__vehicle() {
    
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

