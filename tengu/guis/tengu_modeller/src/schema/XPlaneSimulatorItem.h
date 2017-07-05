// ********************************************************************************************************************
// *                                                                                                                  *
// *                                      The graphics representation of X-Plane simulator.                           *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                        Графическое представление симулятора X-Plane.                             *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 05 jul 2017 at 15:34 *
// ********************************************************************************************************************

#pragma once
#include "AbstractEntityItem.h"
#include "XPlaneSimulator.h"

namespace tengu {
    
    class XPlaneSimulatorItem : public AbstractEntityItem {
        
        Q_OBJECT
        
        public:
            
            XPlaneSimulatorItem ( XPlaneSimulator * entity = nullptr, QGraphicsItem * parent = nullptr );
            virtual ~XPlaneSimulatorItem();
            
            void paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = Q_NULLPTR);
    };
    
};
