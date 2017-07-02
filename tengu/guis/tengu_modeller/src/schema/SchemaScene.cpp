// ********************************************************************************************************************
// *                                                                                                                  *
// *                          QGraphicsScene adopted for AbstractAgent and his descedants.                            *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                      QGraphicsScene, адаптированная для абстрактного агента и его потомков.                      *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 10 jun 2017 at 11:11 *
// ********************************************************************************************************************

#include "SchemaScene.h"

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                   The constructor.                                               *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                     Конструктор.                                                 *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::SchemaScene::SchemaScene ( QObject* parent ) 
    : QGraphicsScene ( parent )
{
    __rootItem = nullptr;
    __changed = false;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                           Unselect all of existing elements.                                     *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                  Отменить выбранность у всех существующих элементов.                             *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::SchemaScene::unselectAll() {
    
    QList < QGraphicsItem * > allItems = items();
    for ( int i=0; i<allItems.size(); i++ ) {
        AbstractEntityItem * agentItem = ( AbstractEntityItem * ) allItems.at(i);
        if ( agentItem ) {
            if ( agentItem->isSelected() ) {
                agentItem->setSelected( false );
            }
        };
    };
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                           Overrided slot for scene clear.                                        *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                         Перекрытый слот для очистки сцены.                                       *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::SchemaScene::clear() {
    
    /*
    bool removed = true;
    while (removed) {
        
        removed = false;
        QList<QGraphicsItem *> children = items();
        for ( int i=0; i<children.count(); i++ ) {
            LinkItem * link = dynamic_cast< LinkItem * > ( children.at(i) );
            if ( link ) {
                delete( link );
                removed = true;
                break;
            };            
        };
    };
    */
    
    QGraphicsScene::clear();
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                            Set root entity item for scene.                                       *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                         Установить корневой элемент для сцены.                                   *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::SchemaScene::setRootItem ( tengu::AbstractEntityItem * rootItem ) {
    
    if ( __rootItem ) {
        delete( __rootItem );
        __rootItem = nullptr;
    };
    
    __rootItem = rootItem;      
    
    clear();
    
    /*
    // Add children of this root item to schema.
    // Добавление на схему детей этого корневого элемента.
    
    AbstractAgent * agent = dynamic_cast< AbstractAgent * > ( rootItem->entity() );
    if ( agent ) {
        for ( int i=0; i<agent->children().count(); i++ ) {
            AbstractEntityItem * 
        };
    };
    */
    
    if ( __rootIsProcess() ) emit signalInsideProcess();
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                   Return the root item as the ProcessItem instance                               *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                  Вернуть корневой элемент как инстанцию ProcessItem                              *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::ProcessItem* tengu::SchemaScene::__rootIsProcess() {
    return dynamic_cast< ProcessItem * > ( __rootItem );
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                            Has something been changed?                                           *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                              Менялось ли что-нибудь?                                             *
// *                                                                                                                  *
// ********************************************************************************************************************

bool tengu::SchemaScene::changed() {
    return __changed;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                              Overrided add item method.                                          *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                         Перекрытый метод добавления элемента.                                    *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::SchemaScene::addItem ( tengu::AbstractEntityItem * item ) {
    
    // This is in any case.
    // Это - в любом случае.
        
    QGraphicsScene::addItem( item );
    QObject::connect( item, SIGNAL( signalSomethingChanged() ), this, SLOT( __on__something_changed() ) );
    
    // Add an X-Plane agent does not come to changes.
    // Добавление X-Plane агента - не приводит к изменениям.
    
    XPlaneAgentItem * xp = dynamic_cast<XPlaneAgentItem * >(item);
    if ( ! xp ) __on__something_changed();
    
    // ProcessItem * processItem = dynamic_cast< ProcessItem * > ( item );
    
    ProcessStartItem * processStartItem = dynamic_cast<ProcessStartItem *>( item );
    ItemWithLinks * itemWithLinks = dynamic_cast<ItemWithLinks *>(item);
    TaskItem * taskItem = dynamic_cast<TaskItem *>(item);
    
    if ( __rootIsProcess() ) {
        
        // We have opened "a process" as root of the scheme.
        // У нас открыт "процесс" в качестве корня схемы.
        
        // if ( processStartItem ) emit signalProcessStartCreated();
        if ( ( itemWithLinks ) && ( ! processStartItem ) ) emit signalProcessItemWithLinksCreated();
        if ( taskItem ) emit signalProcessExplicitTaskCreated();
    };
    
    // If this is a link, it can be in "semi-added" state.
    // Если это связь, то она может находиться в "полу-добавленном" состоянии.
    
    // LinkItem * link = dynamic_cast<LinkItem *>( item );
    // if ( link ) {
    //     
    // };    
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                     Remove links that were not completely created.                               *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                 Удаление связей, которые были созданы не полностью.                              *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::SchemaScene::removeSemiCreatedLinks() {
    
    QList<QGraphicsItem * > items = this->items();
    bool deleted = true;
    while ( deleted ) {
        
        deleted = false;
        for ( int i=0; i<items.count(); i++ ) {
            
            LinkItem * link = dynamic_cast<LinkItem *>( items.at(i) );
            if ( ( link ) && ( ( link->semiCreated() ) || ( link->isEmpty() ) ) ) {
                
                deleted = true;
                delete( link );
                link = nullptr;
                items.removeAt( i );
                break;
                
            };
        };
        
    };
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                              Something has been changed.                                         *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                Что-то было изменено.                                             *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::SchemaScene::__on__something_changed() {
    
    __changed = true;
    emit signalSomethingChanged();
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                   The destructor.                                                *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                      Деструктор.                                                 *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::SchemaScene::~SchemaScene() {
    
    if ( __rootItem ) {
        delete( __rootItem );
        __rootItem = nullptr;
    };
    
}

