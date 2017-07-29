// ********************************************************************************************************************
// *                                                                                                                  *
// *                      An item with a links which can change his size depended on linked agents.                   *
// * ---------------------------------------------------------------------------------------------------------------- *
// *           Элемент со связями, который может менять свои размеры в зависимости от связанных с ним элементов.      *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 20 jul 2017 at 17:35 *
// ********************************************************************************************************************

#pragma once

#include "AbstractEntity.h"
#include "ItemWithLinks.h"

namespace tengu {
  
    class ResizeableItemWithLinks : public ItemWithLinks {
        
        Q_OBJECT
        
        public:
            
            ResizeableItemWithLinks( AbstractEntity * entity = nullptr, QGraphicsItem * parent = nullptr ); 
            virtual ~ResizeableItemWithLinks();
            virtual QRectF boundingRect() const;
            
            // void addOutgoingLink( LinkItem * link );
            // void addIncommingLink( LinkItem * link );
            
            // void update( const QRectF & rect = QRectF() );
            // void recalculate();
            
            virtual bool acceptIncommingLink( LinkItem * link );
            virtual bool acceptOutgoingLink( LinkItem * link );
            

            
        private:
            
            void __arrange_vertically( AbstractEntityItem * item, int & minY, int & maxY );
            bool __vertical_correction();
    };

};