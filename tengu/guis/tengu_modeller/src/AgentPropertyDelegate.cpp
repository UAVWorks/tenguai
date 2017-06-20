// ********************************************************************************************************************
// *                                                                                                                  *
// *                            The delegate for make a custom agent's properties widgets.                            *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                          Делегат для создания произвольных виджитов свойств агента.                              *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 20 jun 2017 at 14:33 *
// ********************************************************************************************************************

#include "AgentPropertyDelegate.h"

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                 The constructor.                                                 *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                    Конструктор.                                                  *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::AgentPropertyDelegate::AgentPropertyDelegate( AgentPropertyModel * model ) 
    : QStyledItemDelegate() 
{
    __model = model;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                     Overrided create displayed widget method.                                    *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                  Перекрытый метод создания виджита для отображения.                              *
// *                                                                                                                  *
// ********************************************************************************************************************

QWidget * tengu::AgentPropertyDelegate::createEditor ( QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index ) const {
    
    bool ok = false;
    AgentPropertyElement oneElement = __model->propertyElement( index, & ok );
    if ( ! ok ) return QStyledItemDelegate::createEditor( parent, option, index );
    switch ( oneElement.type ) {
        
        case AgentPropertyElement::APE_String: return QStyledItemDelegate::createEditor( parent, option, index );
        
        case AgentPropertyElement::APE_ExecutionModeSelector: {
            
            QComboBox * editor = new QComboBox( parent );
            editor->setFont( oneElement.font );
            editor->addItem( tr("Always"), QVariant( AbstractAgent::EM_ALWAYS ) );
            editor->addItem( tr("Real"), QVariant( AbstractAgent::EM_REAL ) );
            editor->addItem( tr("X-Plane simulation"), QVariant( AbstractAgent::EM_XPLANE ) );
            return editor;
        };
    };
    
    QStyledItemDelegate::createEditor( parent, option, index );
        
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                Set data for editor.                                              *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                          Установить данные для редактора.                                        *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AgentPropertyDelegate::setEditorData ( QWidget * editor, const QModelIndex & index ) const {
    
    // qDebug() << "AgentPropertyDelegate::setEditorData, index=" << index ;
    // QStyledItemDelegate::setEditorData ( editor, index );
    
    bool ok = false;
    AgentPropertyElement oneElement = __model->propertyElement( index, & ok );
    if ( ! ok ) {
        QStyledItemDelegate::setEditorData ( editor, index );
        return;
    };
    
    switch ( oneElement.type ) {
        
        case AgentPropertyElement::APE_String: {
            QStyledItemDelegate::setEditorData ( editor, index );
        }; break;
        
        case AgentPropertyElement::APE_ExecutionModeSelector: {
            
            int value = index.model()->data( index, Qt::EditRole ).toInt();
            QComboBox * cb = static_cast<QComboBox*>( editor );

            int idx = cb->findData( value );
            if ( idx < 0 ) return;
            cb->setCurrentIndex( idx );
            
        }; break;
    };
        
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                Set data for model.                                               *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                           Установить данные для модели.                                          *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AgentPropertyDelegate::setModelData ( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const {
    
    bool ok = false;
    AgentPropertyElement oneElement = __model->propertyElement( index, & ok );
    if ( ! ok ) {
        QStyledItemDelegate::setModelData ( editor, model, index );
        return;
    };
    
    switch ( oneElement.type ) {
        
        case AgentPropertyElement::APE_String: {
            QStyledItemDelegate::setModelData ( editor, model, index );
        }; break;
        
        case AgentPropertyElement::APE_ExecutionModeSelector: {
            QComboBox * cb = static_cast<QComboBox*>( editor );
            int idx = cb->currentIndex();
            if ( idx < 0 ) return;
            int value = cb->itemData( idx ).toInt();
            model->setData( index, value, Qt::EditRole );
        };        
    };
        
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                             Overrided painting method.                                           *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                            Перекрытый метод рисования.                                           *
// *                                                                                                                  *
// ********************************************************************************************************************
/*
void tengu::AgentPropertyDelegate::paint ( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const {
    QStyledItemDelegate::paint ( painter, option, index );
}
*/
// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                 Update editor geometry.                                          *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                          Изменение геометрии виджита редактора.                                  *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AgentPropertyDelegate::updateEditorGeometry ( QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index ) const {
    // QStyledItemDelegate::updateEditorGeometry ( editor, option, index );
    editor->setGeometry( option.rect );
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                   The destructor.                                                *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                     Деструктор.                                                  *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::AgentPropertyDelegate::~AgentPropertyDelegate() {

}


