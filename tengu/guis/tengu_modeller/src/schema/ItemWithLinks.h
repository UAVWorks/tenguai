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

#include "AbstractEntityItem.h"
#include "LinkItem.h"

namespace tengu {
    
    class ItemWithLinks : public AbstractEntityItem {
        
        Q_OBJECT
        
        public slots:
            
            void removeLink( QString uuid );
        
        public:
            
            ItemWithLinks ( AbstractEntity * entity = nullptr, QGraphicsItem* parent = nullptr );
            virtual ~ItemWithLinks();
            
            virtual void addOutgoingLink( LinkItem * link );
            virtual void addIncommingLink( LinkItem * link );
            virtual bool acceptIncommingLink( LinkItem * link );
            virtual bool acceptOutgoingLink( LinkItem * link );
            
            /**
             * @short Remove link.
             * 
             * If we gonna go to delete, this is without a difference, whether it is incoming or outgoing, 
             * we will delete it from somewhere.
             * 
             * При удалении нам без разницы, входящий он или исходящий, удаляем его хоть откуда.
             */
            
            void removeLink( LinkItem * link );
            
            // virtual void update( const QRectF & rect = QRectF() );
            
            QList< LinkItem * > hisLinks();
                                    
            // virtual void recalculate();
            
            virtual QJsonObject toJSON();
            virtual bool fromJSON( QJsonObject json );
            
        protected:
            
            // bool _iAmSprout;            
            
            QMap< QString, LinkItem * > _linksOutgoingFromThis;
            QMap< QString, LinkItem * > _linksIncommingToThis;
        
        private slots:
            
            // void __on__linked();
                        
    };
    
};

