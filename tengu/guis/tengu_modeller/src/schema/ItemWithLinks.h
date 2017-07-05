// ********************************************************************************************************************
// *                                                                                                                  *
// *                                     An item which can be linked with another items.                              *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                Элемент, который может иметь связь с другими элементами.                          *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 22 jun 2017 at 10:31 *
// ********************************************************************************************************************

#pragma once

#include <QString>
#include <QList>
#include <QMap>

#include "EMChangableItem.h"
#include "LinkItem.h"

namespace tengu {
    
    class ItemWithLinks : public EMChangableItem {
        
        Q_OBJECT
        
        public slots:
            
            void removeLink( QString uuid );
        
        public:
            
            ItemWithLinks ( AbstractEntity * entity = nullptr, QGraphicsItem* parent = nullptr );
            virtual ~ItemWithLinks();
            
            void addOutgoingLink( LinkItem * link );
            void addIncommingLink( LinkItem * link );
            
            /**
             * @short Remove link.
             * 
             * If we gonna go to delete, this is without a difference, whether it is incoming or outgoing, 
             * we will delete it from somewhere.
             * 
             * При удалении нам без разницы, входящий он или исходящий, удаляем его хоть откуда.
             */
            
            void removeLink( LinkItem * link );
            
            QList< LinkItem * > hisLinks();
            
        protected:
            
            bool _iAmSprout;
            
        private:
            
            QMap< QString, LinkItem * > __linksOutgoingFromThis;
            QMap< QString, LinkItem * > __linksIncommingToThis;
                        
    };
    
};

