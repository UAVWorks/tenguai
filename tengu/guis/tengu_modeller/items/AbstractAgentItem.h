// ********************************************************************************************************************
// *                                                                                                                  *
// *                              Abstract graphics object, representation of AbstractAgent.                          *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                           Абстрактный графический объект, представление AbstractAgent'а.                         *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 08 jun 2017 at 08:57 *
// ********************************************************************************************************************

#pragma once

#include <QGraphicsObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QRectF>

#include "AbstractAgent.h"

namespace tengu {

    class AbstractAgentItem : public QGraphicsObject {
        Q_OBJECT
        public:
            AbstractAgentItem( AbstractAgent * agent, QGraphicsItem * parent = Q_NULLPTR );
            virtual ~AbstractAgentItem();
        
            void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR);
            QRectF boundingRect() const;
            
        protected:
            
            AbstractAgent * _agent;
            
        private:
    };
};

