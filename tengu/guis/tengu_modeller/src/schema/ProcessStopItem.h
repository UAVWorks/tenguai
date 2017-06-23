// ********************************************************************************************************************
// *                                                                                                                  *
// *                                        Process stop graphics representation.                                     *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                   Графическое представление окончания процесса.                                  *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 16 jun 2017 at 12:56 *
// ********************************************************************************************************************

#pragma once

#include <QPen>

#include "ItemWithLinks.h"
#include "ProcessStop.h"

namespace tengu {
    
    class ProcessStopItem : public ItemWithLinks {
        
        Q_OBJECT
        public:
            
            ProcessStopItem ( ProcessStop * entity = Q_NULLPTR, QGraphicsItem * parent = Q_NULLPTR );
            virtual ~ProcessStopItem();
            
            // QJsonObject toJSON();
            // bool fromJSON( QJsonObject json );
            
            void checkEntity();
            
        protected:
            
            void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = Q_NULLPTR);
            
        private:
            
            int __radius;
            
    };
};

