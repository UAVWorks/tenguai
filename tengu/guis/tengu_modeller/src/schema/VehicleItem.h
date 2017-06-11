// ********************************************************************************************************************
// *                                                                                                                  *
// *                                         Vehicle graphics representation.                                         *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                     Графическое представление "самоходки".                                       *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 08 jun 2017 at 13:51 *
// ********************************************************************************************************************

#pragma once

#include <QPixmap>

#include "AbstractEntityItem.h"
#include "Vehicle.h"

namespace tengu {

    class VehicleItem : public AbstractEntityItem {
        
        Q_OBJECT
        public:
            
            VehicleItem ( Vehicle * vehicle, QGraphicsItem* parent = nullptr );
            virtual ~VehicleItem();
            
            void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = Q_NULLPTR);
            QRectF boundingRect() const;
    };
};

