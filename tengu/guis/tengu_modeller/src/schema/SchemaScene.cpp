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
    __rootEntity = nullptr;
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
        
        QGraphicsItem * item = gItems.at(i);
        removeItem( item );
    };
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                         Get the entity for root scene item.                                      *
// * ---------------------------------------------------------------------------------------------------------------- *
// * Получить "сущность" для корневого элемента схемы. *
// * *
// ********************************************************************************************************************

tengu::AbstractEntity * tengu::SchemaScene::rootEntity() {
    return __rootEntity;
};

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                    Cleanup the whole scene, overrided function.                                  *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                      Очистка всей сцены, перекрытая функция.                                     *
// *                                                                                                                  *
// ********************************************************************************************************************
/*
void tengu::SchemaScene::clear() {

    QList<QGraphicsItem * > items = items();
    for ( int i=0; i<items.count(); i++ ) {
        QGraphicsItem * item = items.at(i);
        removeItem( item );
        delete( item );
    };
    
}
*/
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
    __rootEntity = rootItem->entity();
    
    if ( rootAsProcess() ) emit signalInsideProcess();
    
    // Add children of this root item to schema.
    // Добавление на схему детей этого корневого элемента.
    
    AbstractAgent * entity = dynamic_cast< AbstractAgent * > ( rootItem->entity() );
    
    if ( ( entity ) && ( entity->hasChildren() ) ) {
        
        QList<AbstractAgent * > hisChildren = entity->children();
        
        // In 2 pass. First one is agents itself.
        // В 2 прохода. Сначала - агенты как таковые.
        
        for ( int i=0; i<hisChildren.count(); i++ ) {
            
            AbstractEntityItem * item = AgentItemFactory::createItem( hisChildren.at( i ) );
            
            if ( item ) {
                addItem( item );          
            };
        };
        
        // Second one is links of that agents. This doing when all agents was created.
        // Вторая часть - это связи агентов. После того, как все агенты были созданы.
        
        for ( int i=0; i<hisChildren.count(); i++ ) {
            
            AbstractAgent * oneAgent = hisChildren.at(i);
                                                
            QList<AbstractAgent * > prev = oneAgent->previousByFocus();
                        
            for ( int i=0; i<prev.count(); i++ ) {
                AbstractAgent * ol = prev.at(i);
                addLink( ol, oneAgent );
            };
            
            QList<AbstractAgent * > next = oneAgent->nextByFocus();
            
            for ( int i=0; i<next.count(); i++ ) {
                AbstractAgent * nl = next.at(i);
                addLink( oneAgent, nl );
            };        
        };
        
    };            
    
    // Sprouts and his links.
    // Sprout'ы и их связи.
    
    if ( entity ) {
        
        QList<AbstractAgent * > chi = entity->children();
        for ( int i=0; i<chi.count(); i++ ) {
            SproutableAgent * spa = dynamic_cast<SproutableAgent * >( chi.at(i) );
            if ( spa ) {
                QList<Sprout * > his_sprouts = spa->sprouts();
                for ( int spIndex=0; spIndex<his_sprouts.count(); spIndex++ ) {
                    Sprout * os = his_sprouts.at( spIndex );
                    SproutItem * spi = new SproutItem( os );
                    addItem( spi );
                    addLink( spa, os );
                };
            };
        };
    };    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                   Return the root item as the ProcessItem instance                               *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                  Вернуть корневой элемент как инстанцию ProcessItem                              *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::ProcessItem * tengu::SchemaScene::rootAsProcess() {
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
// *                                               Add one link to the scene.                                         *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                              Добавить на сцену одну связь.                                       *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::SchemaScene::addLink ( tengu::AbstractEntity * fromAgent, tengu::AbstractEntity * toAgent ) {
    
    ItemWithLinks * fromItem = dynamic_cast< ItemWithLinks * >( itemFor( fromAgent ) );
    ItemWithLinks * toItem = dynamic_cast< ItemWithLinks * > ( itemFor ( toAgent ) );
    
    if ( ( fromItem ) && ( toItem ) ) {
        
        if ( ! haveLink( fromAgent, toAgent ) ) {
            LinkItem * link = new LinkItem();
            fromItem->addOutgoingLink( link );
            toItem->addIncommingLink( link );
            link->recalculate();
            QGraphicsScene::addItem( link );
        };
        
    } else qDebug() << "SchemaScene::addLink, кто-то из участников пустой:" << fromItem << ", " << toItem ;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                       Does the schema have this link already?                                    *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                          Есть ли уже такая связь на схеме?                                       *
// *                                                                                                                  *
// ********************************************************************************************************************

bool tengu::SchemaScene::haveLink ( tengu::AbstractEntity * fromAgent, tengu::AbstractEntity * toAgent ) {
    
    if ( ( ! fromAgent ) || ( ! toAgent ) ) return false;
    
    QList< QGraphicsItem * > haved = items();
    
    for ( int i=0; i< haved.count(); i++ ) {
        
        LinkItem * link = dynamic_cast< LinkItem * > ( haved.at(i) );
        
        if ( link ) {
            
            AbstractEntityItem * eiFrom = link->getFrom();
            AbstractEntityItem * eiTo = link->getTo();
            
            if ( ( eiFrom ) && ( eiTo ) ) {
                
                AbstractEntity * l_a_from = eiFrom->entity();
                AbstractEntity * l_a_to = eiTo->entity();
                
                if ( ( l_a_from ) && ( l_a_to ) ) {
                    if ( ( l_a_from->getUUID() == fromAgent->getUUID() ) && ( l_a_to->getUUID() == toAgent->getUUID() ) ) {
                        return true;
                    }
                };
            };
        };
    };
    
    return false;
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
        
        tengu::AbstractEntity * cent = item->entity();
        
        if ( cent ) {
            cent->silent( true );
            gItem->setX( cent->getX() );
            gItem->setY( cent->getY() );
            cent->silent( false );
        };
        
        AbstractAgent * rootAgent = dynamic_cast<AbstractAgent * > ( __rootItem->entity() );
        AbstractAgent * curAgent = dynamic_cast<AbstractAgent * > ( item->entity() );
        if ( ( rootAgent ) && ( curAgent ) ) rootAgent->addChild( curAgent );
        
        QObject::connect( item, SIGNAL( signalSomethingChanged() ), this, SLOT( __on__something_changed() ) );
        
        ProcessStartItem * processStartItem = dynamic_cast<ProcessStartItem *>( item );
        ItemWithLinks * itemWithLinks = dynamic_cast<ItemWithLinks *>(item);
        TaskItem * taskItem = dynamic_cast<TaskItem *>(item);
                
        if ( rootAsProcess() ) {
            
            // We have opened "a process" as root of the scheme.
            // У нас открыт "процесс" в качестве корня схемы.
            
            if ( ( itemWithLinks ) && ( ! processStartItem ) ) {
                emit signalProcessItemWithLinksCreated();
            };
        };        
                
        if ( taskItem ) {
            
            // There was a task.
            // Это была задача.
        
            emit signalProcessExplicitTaskCreated();            
        }
                
    }
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                           Remove one item from the scene.                                        *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                          Удаление одного элемента со сцены.                                      *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::SchemaScene::removeItem ( QGraphicsItem * gItem ) {
    
    AbstractEntityItem * item = dynamic_cast < AbstractEntityItem * > ( gItem );
    if ( item ) {
        
        ItemWithLinks * li = dynamic_cast < ItemWithLinks * >( item );
        if ( li ) {
            
            // We have an item with links. Remove his links together.
            // У нас есть элемент со связями. Вместе с ним удаляем и его связи тоже.
            
            QList< LinkItem * > his_links = li->hisLinks();
            for ( int i=0; i<his_links.count(); i++ ) {
                removeItem( his_links.at(i) );
            };
        };
        
        QObject::disconnect( item, SIGNAL( signalSomethingChanged() ), this, SLOT( __on__something_changed() ) );        
        
        // TaskItem * taskItem = dynamic_cast < TaskItem * > ( item );
        // SproutItem * sproutItem = dynamic_cast <SproutItem * > ( item );
        // if ( taskItem )  __taskItems.remove( taskItem->getUUID() );
        // if ( sproutItem ) __sproutItems.remove( sproutItem->getUUID() );
    };
    
    QGraphicsScene * scene = gItem->scene();
    if ( scene == this )  QGraphicsScene::removeItem( gItem );
    
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
// *                                         Find an graphics item for this entity                                    *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                     Поиск графического элемента для данной сущности.                             *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::AbstractEntityItem * tengu::SchemaScene::itemFor ( tengu::AbstractEntity* entity ) {
    
    if ( ( ! entity ) || ( entity->getUUID().isEmpty() ) ) return nullptr;
    
    QList<QGraphicsItem *> ilist = items();
    for ( int i=0; i<ilist.count(); i++ ) {
        AbstractEntityItem * item = dynamic_cast<AbstractEntityItem * > (ilist.at(i));
        if ( item ) {
            AbstractEntity * ie = item->entity();
            if ( ( ie ) && ( ie->getUUID() == entity->getUUID() ) ) return item;
        }
    };
    
    return nullptr;
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

