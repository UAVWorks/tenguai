// ********************************************************************************************************************
// *                                                                                                                  *
// *                            The delegate for make a custom agent's properties widgets.                            *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                          Делегат для создания произвольных виджитов свойств агента.                              *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 20 jun 2017 at 14:29 *
// ********************************************************************************************************************

#pragma once

#include <QStyledItemDelegate>
#include <QDebug>
#include <QComboBox>
#include <QLayout>

#include "AgentPropertyModel.h"
#include "AgentPropertyElement.h"

namespace tengu {
    
    class AgentPropertyDelegate : public QStyledItemDelegate {
        
        Q_OBJECT
        
        public:
            
            AgentPropertyDelegate ( AgentPropertyModel * model );
            virtual ~AgentPropertyDelegate();
            
            QWidget * createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const;
            void setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const;
            void setEditorData(QWidget * editor, const QModelIndex & index) const;
            // void paint( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const;
            void updateEditorGeometry(QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index) const;
            
        private:
            
            AgentPropertyModel * __model;
            
            void __setComboBoxValue( QWidget * editor, const QModelIndex & index );
            void __setModelValueFromComboBox(  QWidget * editor, QAbstractItemModel * model, const QModelIndex & index );
            
    };
};

