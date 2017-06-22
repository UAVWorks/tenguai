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
            
            void signalSomethingChanged();
        
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
                        
        private slots:
            
            void __on__something_changed();
            
    };
    
};

