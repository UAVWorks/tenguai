// ********************************************************************************************************************
// *                                                                                                                  *
// *                                       Process start graphics representation.                                     *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                    Графическое представление начала процесса.                                    *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 16 jun 2017 at 12:51 *
// ********************************************************************************************************************

#pragma once

#include <QRadialGradient>

#include "AbstractEntityItem.h"
#include "ProcessStart.h"

namespace tengu {
    
    class ProcessStartItem : public AbstractEntityItem {
        
        Q_OBJECT
        
        public:
            
            ProcessStartItem ( ProcessStart * entity = Q_NULLPTR, QGraphicsItem * parent = Q_NULLPTR );
            virtual ~ProcessStartItem();
            
            QJsonObject toJSON();
            bool fromJSON( QJsonObject json );
            
            void checkEntity();
            
        protected:
            
            void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = Q_NULLPTR);
    };
};

