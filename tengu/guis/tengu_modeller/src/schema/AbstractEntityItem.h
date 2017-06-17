// ********************************************************************************************************************
// *                                                                                                                  *
// *                             Abstract graphics object, representation of AbstractEntity.                          *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                           Абстрактный графический объект, представление AbstractEntity.                          *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 08 jun 2017 at 08:57 *
// ********************************************************************************************************************

#pragma once

#include <QGraphicsObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QPen>
#include <QPixmap>
#include <QColor>
#include <QRectF>
#include <QDebug>
#include <QLinearGradient>
#include <QList>
#include <QPair>
#include <QDateTime>
#include <QUuid>
#include <QGraphicsSceneMouseEvent>
#include <QMouseEvent>

#include "AbstractStorageableEntity.h"
#include "AbstractEntity.h"
#include "AbstractAgent.h"
#include "Sprout.h"

// Colors for process diagram.
// Цвета для диаграммы процессов.

#define ORDINAR_BORDER_COLOR            QColor(56,37,9)
#define ORDINAR_SELECTED_BORDER_COLOR   QColor(87,64,30)
#define ORDINAR_FILL_COLOR              QColor(143,107,50) 
#define ORDINAR_SELECTED_FILL_COLOR     QColor(179,146,93)

#define ACTIVE_BORDER_COLOR             QColor(77,38,0) 
#define ACTIVE_SELECTED_BORDER_COLOR    QColor(89,0,0)
#define ACTIVE_FILL_COLOR               QColor(255,126,0)
#define ACTIVE_SELECTED_FILL_COLOR      QColor(242,155,104)


#define FOCUSED_BORDER_COLOR            QColor(117,81,26)
#define FOCUSED_SELECTED_BORDER_COLOR   QColor(143,107,50)
#define FOCUSED_FILL_COLOR              QColor(179,146,93)
#define FOCUSED_SELECTED_FILL_COLOR     QColor(222,188,133)

namespace tengu {

    class AbstractEntityItem : public QGraphicsObject, public AbstractStorageableEntity {
        
        Q_OBJECT
        
        public:
            
            AbstractEntityItem( AbstractEntity * entity = Q_NULLPTR, QGraphicsItem * parent = Q_NULLPTR );
            virtual ~AbstractEntityItem();
        
            void paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = Q_NULLPTR);
            QRectF boundingRect() const;
            
            // AbstractEntity * entity();
            
            // Select this agent (to visual distinguish)
            // Выбрать агента (для визуального отличия)
            
            void setSelected( bool selection );
            
            bool isSelected();
            
            // Properties list. The pair of string. First is name of element, second is value of element.
            // Список свойств. Пара строк. Первая - имя элемента, вторая - его значение.
            
            virtual QList<QPair<QString, QVariant>> properties();
            
            // Is this entity the subject of decomposition?
            // Является ли данная сущность декомпозируемой?
            
            bool isDecomposite();
            
            // ------------------------------------------------------------------------------------
            //                    AbstractStorageableEntity interface realization.
            // ------------------------------------------------------------------------------------
            //                    Реализация интерфейса AbstractStorageableEntity. 
            // ------------------------------------------------------------------------------------
            
            virtual QString getUUID();
            virtual QString getName();
            virtual void setName( QString name );
            virtual QString getComment();
            virtual void setComment( QString comment );
            virtual execution_mode_t getExecutionMode();
            virtual void setExecutionMode( execution_mode_t mode );
            virtual bool hasChanged();
            virtual QDateTime lastModified();
            virtual QJsonObject toJSON();
            virtual bool fromJSON( QJsonObject json );
            
            QPoint mousePressedPos();
            
        protected:
            
            AbstractEntity * _entity;
            QRectF _boundingRect;
            
            int _brighter( int color );
            
            bool _selected;
            
            // Get border's color for process diagram.
            // Получить цвет границ для диаграммы процессов.
            
            QColor _processDiagram_borderColor();
            QColor _processDiagram_shadowColor();
            QColor _processDiagram_darkFillColor();
            QColor _processDiagram_brightFillColor();
            
            QPen _processDiagram_borderPen();
            
            void mousePressEvent( QGraphicsSceneMouseEvent * event );
            // void mouseReleaseEvent( QGraphicsSceneMouseEvent * event );
            // void mouseMoveEvent(QGraphicsSceneMouseEvent * event);

            // Can this entity to be decomposed forcibly?
            // Может ли данная "сущность" быть декомпозирована принудительно?
            
            bool _decomposite;
            
            // Has been this object changed?
            // Был ли данный объект изменен?
            
            bool _changed;
            
        private:
            
            bool __mousePressed;
            QPoint __mousePressedPoint;
            
            QString __uuid;
            
            QDateTime __lastModified;
            
    };
};

