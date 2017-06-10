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

#include "AbstractAgentItem.h"
#include "VehicleItem.h"
#include "TaskItem.h"
#include "SproutItem.h"

namespace tengu {
    
    class SchemaScene : public QGraphicsScene {
        
        Q_OBJECT
        
        public:
            
            SchemaScene ( QObject* parent = 0 );
            virtual ~SchemaScene();
            
            void unselectAll();
            
    };
    
};

