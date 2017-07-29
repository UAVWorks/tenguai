// ********************************************************************************************************************
// *                                                                                                                  *
// *                          QGraphicsScene adopted for AbstractAgent and his descedants.                            *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                      QGraphicsScene, адаптированная для абстрактного агента и его потомков.                      *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 10 jun 2017 at 11:11 *
// ********************************************************************************************************************

#pragma once

#include <QGraphicsScene>
#include <QList>
#include <QGraphicsItem>

#include "AgentItemFactory.h"

namespace tengu {
    
    class SchemaScene : public QGraphicsScene {
        
        Q_OBJECT
        
        signals:
            
            /**
             * @short We have some changes at the schema.
             * 
             * Сигнал вырабатывается в том случае, если на схеме были хоть какие-то изменения.
             */
            
            void signalSomethingChanged();
            void signalInsideProcess();
            // void signalProcessStartCreated();
            void signalProcessItemWithLinksCreated();
            
            /**
             * @short The task was created inside the process.
             * 
             * The task (precisely the task, not any block) was created within the process.
             * Задача (именно и в точности задача, а не любой блок) была создана внутри процесса.
             */
            
            void signalProcessExplicitTaskCreated();
        
        public slots:
            
            void clear();
            
        public:
            
            SchemaScene ( QObject* parent = 0 );
            virtual ~SchemaScene();
            
            void unselectAll();
            void setRootItem( AbstractEntityItem * rootItem );
            
            void addItem( QGraphicsItem * gItem );
            void removeItem ( QGraphicsItem * gItem );
            AbstractEntity * rootEntity();
            ProcessItem * rootAsProcess();
            AbstractEntityItem * itemFor( AbstractEntity * entity );
            
            bool changed();
            
            void removeSemiCreatedLinks();
            
            // Set element's visibility according to selected execution mode
            // Установить видимость элементов согласно выбранному режиму выполнения.
            
            void setExecutionMode( AbstractEntity::execution_mode_t execution_mode );
            
            void addLink( AbstractEntity * fromAgent, AbstractEntity * toAgent );
            bool haveLink( AbstractEntity * fromAgent, AbstractEntity * toAgent );
            
        private:
            
            AbstractEntityItem * __rootItem;
            AbstractEntity * __rootEntity;
            bool __changed;
            
            // QMap< QString, TaskItem * > __taskItems;
            // QMap< QString, SproutItem * > __sproutItems;
                        
        private slots:
            
            void __on__something_changed();
            
    };
    
};

