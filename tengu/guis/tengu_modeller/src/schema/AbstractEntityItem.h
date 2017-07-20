// ********************************************************************************************************************
// *                                                                                                                  *
// *                             Abstract graphics object, representation of AbstractEntity.                          *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                           Абстрактный графический объект, представление AbstractEntity.                          *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 08 jun 2017 at 08:57 *
// ********************************************************************************************************************

#pragma once

#include <typeinfo>

#include <QGraphicsObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QPen>
#include <QBrush>
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
#include <QFont>
#include <QFontMetrics>
#include <QComboBox>

#include "AbstractEntity.h"
#include "Sprout.h"
#include "AgentFactory.h"
#include "../AgentPropertyElement.h"

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

    class AbstractEntityItem : public QGraphicsObject, public AbstractEntity {
        
        friend class AgentPropertyModel;
        
        Q_OBJECT
        
        Q_PROPERTY( QString system_name READ getSystemName WRITE setSystemName );
        Q_PROPERTY( QString human_name READ getHumanName WRITE setHumanName );
        Q_PROPERTY( QString comment READ getComment WRITE setComment );
        Q_PROPERTY( QString uuid READ getUUID );   
        Q_PROPERTY( AbstractEntity::execution_mode_t execution_mode READ getExecutionMode WRITE setExecutionMode );
        
        signals:
            void signalSomethingChanged();
            
        public:
            
            AbstractEntityItem( AbstractEntity * entity = Q_NULLPTR, QGraphicsItem * parent = Q_NULLPTR );
            virtual ~AbstractEntityItem();
        
            virtual void paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = Q_NULLPTR);
            virtual QRectF boundingRect() const;
            
            // AbstractEntity * entity();
            
            // Select this agent (to visual distinguish)
            // Выбрать агента (для визуального отличия)
            
            void setSelected( bool selection );
            
            bool isSelected();
            
            // The properties list. 
            // Список свойств. 
            
            virtual QList< QList<AgentPropertyElement> > properties();
            
            // Is this entity the subject of decomposition?
            // Является ли данная сущность декомпозируемой?
            
            bool isDecomposite();
            
            // ------------------------------------------------------------------------------------
            //                        AbstractAgent interface overriding.
            // ------------------------------------------------------------------------------------
            //                       Перекрытие интерфейса AbstractAgent. 
            // ------------------------------------------------------------------------------------
            
            virtual QString getUUID();
            virtual QString getSystemName();
            virtual void setSystemName( QString name );
            virtual QString getHumanName();
            virtual void setHumanName( QString name );
            virtual QString getComment();
            virtual void setComment( QString comment );
            virtual AbstractEntity::execution_mode_t getExecutionMode();
            virtual void setExecutionMode( AbstractEntity::execution_mode_t mode );
            virtual bool hasChanged();
            virtual QDateTime lastModified();
            virtual QJsonObject toJSON();
            virtual bool fromJSON( QJsonObject json );
            
            QPoint mousePressedPos();
            
            virtual void checkEntity();
            AbstractEntity * entity();
                        
            virtual void recalculate();
            
            // int getX();
            void setX( int x );
            // int getY();
            void setY( int y );
                        
        protected:
            
            AbstractEntity * _entity;
            QRectF _boundingRect;
            
            bool _canChangeExecuteMode;
                        
            int _brighter( int color );
            int _darker( int color );
            
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
            
            void _storePainterSettings( QPainter * painter );
            void _restorePainterSettings( QPainter * painter );
            void _drawBorderRect( QPainter * painter );
            void _drawTaskRectangle( QPainter * painter );
            
            QPixmap _executionModePixmap( bool forSize32 = true );
            
            void _somethingChanged();                        
                        
        private:
                        
            bool __mousePressed;
            QPoint __mousePressedPoint;
            
            QString __uuid;
            
            QDateTime __lastModified;
            
            // Stored settings for painter
            // Сохраненнные установки painter'а
            
            QPen    __oldPainterPen;
            QBrush  __oldPainterBrush;
            QFont   __oldPainterFont;
            QPainter::CompositionMode __oldPainterCompositionMode;
            
    };
};

