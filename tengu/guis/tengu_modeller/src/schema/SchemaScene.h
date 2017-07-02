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
            
            void addItem( AbstractEntityItem * item );
            bool changed();
            
            void removeSemiCreatedLinks();
            
        private:
            
            AbstractEntityItem * __rootItem;
            bool __changed;
            ProcessItem * __rootIsProcess();
                        
        private slots:
            
            void __on__something_changed();
            
    };
    
};

