// ********************************************************************************************************************
// *                                                                                                                  *
// *     Graphics representation of Sprout class (signal/slot mechanism, inter-agent redis-based communications).     *
// * ---------------------------------------------------------------------------------------------------------------- *
// *          Графическое представление класса Sprout ("росток' - механизм сигналов-слотов, межагентное общение,      *
// *                                                  основанное на redis'е.                                          *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 10 jun 2017 at 12:20 *
// ********************************************************************************************************************

#pragma once

#include "AbstractEntityItem.h"
#include "Sprout.h"
#include "AgentPropertyElement.h"

namespace tengu {
    
    class SproutItem : public AbstractEntityItem {
        
        Q_OBJECT
        
        Q_PROPERTY( Sprout::sprout_type_t sprout_type READ getSproutType WRITE setSproutType );
        Q_PROPERTY( SproutItem::sprout_orientation_t orientation READ getOrientation WRITE setOrientation );
        
        public:
            
            // Draft orientation agnle for this sprout item
            // Угол ориентации данного Sprout'а на диаграмме.
            
            enum sprout_orientation_t {
                
                SPO_0 = 0,
                SPO_45 = 45,
                SPO_90 = 90,
                SPO_135 = 135,
                SPO_180 = 180,
                SPO_225 = 225,
                SPO_270 = 270,
                SPO_315 = 315
                
            };
            
            SproutItem ( Sprout * sprout = nullptr, QGraphicsItem * parent = nullptr );
            virtual ~SproutItem();
            
            void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = Q_NULLPTR);
            
            QList< QList<AgentPropertyElement> > properties();
            
            void checkEntity();
            
            void setSproutType( Sprout::sprout_type_t type );
            Sprout::sprout_type_t getSproutType();
            
            Sprout * sprout();
            
            /**
             * @short Get the orientation of this element at the diagram.
             * 
             * The "Sprout" can be rotated on the chart to make it more convenient and readable.
             * 
             * "Росточек" может вращаться на диаграмме, чтобы было удобнее и читабельнее.
             */
            
            sprout_orientation_t getOrientation();
            void setOrientation( sprout_orientation_t orientation );
            
        private:
            
            sprout_orientation_t __orientation;
    };
    
};

