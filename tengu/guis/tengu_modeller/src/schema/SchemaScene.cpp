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
// *                             Set element's visibility according to selected execution mode.                       *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                         Установить видимость элементов согласно выбранному режиму выполнения.                    *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::SchemaScene::setExecutionMode ( tengu::AbstractEntity::execution_mode_t execution_mode ) {
    
    for ( int i=0; i<items().count(); i++ ) {
        AbstractEntityItem * ei = dynamic_cast< AbstractEntityItem * >( items().at(i) );
        if ( ei ) {
            AbstractEntity::execution_mode_t eiem = ei->getExecutionMode();
            if ( ( eiem == AbstractEntity::EM_ALWAYS ) || ( eiem == execution_mode ) ) ei->setVisible( true );
            else ei->setVisible( false );
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
    
    QList< QGraphicsItem * > gItems = items();
    for ( int i=0; i<gItems.count(); i++ ) {
        AbstractEntityItem * aitem = dynamic_cast< AbstractEntityItem * > ( gItems.at(i) );
        if ( aitem ) {
            QObject::disconnect( aitem, SIGNAL( signalSomethingChanged() ), this, SLOT( __on__something_changed() ) );
        };
    };
    
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

    // __taskItems.clear();
    // __sproutItems.clear();
    
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

    clear();
    
    if ( __rootItem ) {
        delete( __rootItem );
        __rootItem = nullptr;
    };
        
    __rootItem = rootItem;      
        
    
    
    // Add children of this root item to schema.
    // Добавление на схему детей этого корневого элемента.
    
    AbstractAgent * entity = dynamic_cast< AbstractAgent * > ( rootItem->entity() );
    
    if ( ( entity ) && ( entity->hasChildren() ) ) {
        
        for ( int i=0; i<entity->children().count(); i++ ) {
            
            AbstractEntityItem * item = AgentItemFactory::createItem( entity->children().at(i) );
            
            if ( item ) {
                AbstractEntity * cent = item->entity();
                if ( cent ) {
                    QGraphicsItem * qgr = ( QGraphicsItem * ) item;
                    qgr->setX( cent->getX() );
                    qgr->setY( cent->getY() );
                };
                addItem( item );
            };
        };
    };    
    
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

void tengu::SchemaScene::addItem ( QGraphicsItem * gItem ) {
    
    // This is in any case.
    // Это - в любом случае.
        
    QGraphicsScene::addItem( gItem );    
    
    tengu::AbstractEntityItem * item = dynamic_cast< tengu::AbstractEntityItem * > ( gItem );
    
    if ( item ) {
        
        item->checkEntity();
        
        AbstractAgent * rootAgent = dynamic_cast<AbstractAgent * > ( __rootItem->entity() );
        AbstractAgent * curAgent = dynamic_cast<AbstractAgent * > ( item->entity() );
        if ( ( rootAgent ) && ( curAgent ) ) rootAgent->addChild( curAgent );
        
        QObject::connect( item, SIGNAL( signalSomethingChanged() ), this, SLOT( __on__something_changed() ) );
        
        // Add an X-Plane agent does not come to changes.
        // Добавление X-Plane агента - не приводит к изменениям.
        
        XPlaneAgentItem * xp = dynamic_cast<XPlaneAgentItem * >(item);
        if ( ! xp ) __on__something_changed();
        
        // ProcessItem * processItem = dynamic_cast< ProcessItem * > ( item );
        
        ProcessStartItem * processStartItem = dynamic_cast<ProcessStartItem *>( item );
        ItemWithLinks * itemWithLinks = dynamic_cast<ItemWithLinks *>(item);
        TaskItem * taskItem = dynamic_cast<TaskItem *>(item);
        // SproutItem * sproutItem = dynamic_cast<SproutItem * > ( item );
        
        if ( __rootIsProcess() ) {
            
            // We have opened "a process" as root of the scheme.
            // У нас открыт "процесс" в качестве корня схемы.
            
            if ( ( itemWithLinks ) && ( ! processStartItem ) ) emit signalProcessItemWithLinksCreated();
        };
        
        
        // Caching added component. Automatic naming if there are none.
        // Кэширование добавленного компонента. Автоматическое присвоение имен, если их нет.
        
        if ( taskItem ) {
            
            // There was a task.
            // Это была задача.
            
            emit signalProcessExplicitTaskCreated();
            // __taskItems[ taskItem->getUUID() ] = taskItem ;
            // if ( taskItem->getSystemName().isEmpty() ) taskItem->setSystemName( tr("Task_") + QString::number( __taskItems.count() ) );
            // if ( taskItem->getHumanName().isEmpty() ) taskItem->setHumanName( tr("Task ") + QString::number( __taskItems.count() ) );
        }
        
        /*
        if ( sproutItem ) {
            
            // There was a sprout.
            // Это был sprout.
            
            __sproutItems[ sproutItem->getUUID() ] = sproutItem;
            if ( sproutItem->getSystemName().isEmpty() ) sproutItem->setSystemName( tr("Sprout_") + QString::number( __sproutItems.count() ) );
            if ( sproutItem->getHumanName().isEmpty() ) sproutItem->setHumanName( tr("Sprout ") + QString::number( __sproutItems.count() ) );
            
        };
        */        
        
        // If this is a link, it can be in "semi-added" state.
        // Если это связь, то она может находиться в "полу-добавленном" состоянии.
        
        // LinkItem * link = dynamic_cast<LinkItem *>( item );
        // if ( link ) {
        //     
        // };    
    }
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                           Remove one item from the scene.                                        *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                          Удаление одного элемента со сцены.                                      *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::SchemaScene::removeItem ( QGraphicsItem* gItem ) {
    
    AbstractEntityItem * item = dynamic_cast < AbstractEntityItem * > ( gItem );
    if ( item ) {
        QObject::disconnect( item, SIGNAL( signalSomethingChanged() ), this, SLOT( __on__something_changed() ) );
        
        // TaskItem * taskItem = dynamic_cast < TaskItem * > ( item );
        // SproutItem * sproutItem = dynamic_cast <SproutItem * > ( item );
        // if ( taskItem )  __taskItems.remove( taskItem->getUUID() );
        // if ( sproutItem ) __sproutItems.remove( sproutItem->getUUID() );
    };
    
    QGraphicsScene::removeItem( gItem );
    
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

