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
        
        case AgentPropertyElement::String: 
        case AgentPropertyElement::Float: {
            
            return QStyledItemDelegate::createEditor( parent, option, index );
            
        }; break;
        
        case AgentPropertyElement::ExecutionModeSelector: {
            
            QComboBox * editor = new QComboBox( parent );
            editor->setFont( oneElement.font );
            editor->addItem( tr("Always"), QVariant( AbstractAgent::EM_ALWAYS ) );
            editor->addItem( tr("Real"), QVariant( AbstractAgent::EM_REAL ) );
            editor->addItem( tr("X-Plane simulation"), QVariant( AbstractAgent::EM_XPLANE ) );
            return editor;
            
        }; break;
        
        case AgentPropertyElement::SproutTypeSelector : {
            QComboBox * editor = new QComboBox( parent );
            editor->setFont( oneElement.font );
            editor->addItem( tr("In-process input"), QVariant( Sprout::SPT__IN_PROCESS_INPUT ) );
            editor->addItem( tr("External input"), QVariant( Sprout::SPT__EXTERNAL_INPUT ) );
            editor->addItem( tr("In-process output"), QVariant( Sprout::SPT__IN_PROCESS_OUTPUT ) );
            editor->addItem( tr("External output"), QVariant( Sprout::SPT__EXTERNAL_OUTPUT ) );
            return editor;
        }; break;
        
        case AgentPropertyElement::SproutAngleSelector : {
            QComboBox * editor = new QComboBox( parent );
            editor->setFont( oneElement.font );
            editor->addItem( tr("0 degrees"), SproutItem::SPO_0 );
            editor->addItem( tr("45 degrees"), SproutItem::SPO_45 );
            editor->addItem( tr("90 degrees"), SproutItem::SPO_90 );
            editor->addItem( tr("135 degrees"), SproutItem::SPO_135 );
            editor->addItem( tr("180 degrees"), SproutItem::SPO_180 );
            editor->addItem( tr("225 degrees"), SproutItem::SPO_225 );
            editor->addItem( tr("270 degrees"), SproutItem::SPO_270 );
            editor->addItem( tr("315 degrees"), SproutItem::SPO_315 );
            return editor;
        };
        
    };
    
    return QStyledItemDelegate::createEditor( parent, option, index );
        
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                            Set editor data for combo-box variant of editor's widget.                             *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                    Установить данные для варианта, когда редактором является выпадающий список.                  *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AgentPropertyDelegate::__setComboBoxValue ( QWidget* editor, const QModelIndex& index ) {
    
    int value = index.model()->data( index, Qt::EditRole ).toInt();
    QComboBox * cb = static_cast<QComboBox*>( editor );

    int idx = cb->findData( value );
    if ( idx < 0 ) return;
    cb->setCurrentIndex( idx );
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
        
        case AgentPropertyElement::String: 
        case AgentPropertyElement::Float:
        {
            QStyledItemDelegate::setEditorData ( editor, index );
            
        }; break;
        
        case AgentPropertyElement::ExecutionModeSelector : 
        case AgentPropertyElement::SproutTypeSelector :
        case AgentPropertyElement::SproutAngleSelector :
        {
            /*
            int value = index.model()->data( index, Qt::EditRole ).toInt();
            QComboBox * cb = static_cast<QComboBox*>( editor );

            int idx = cb->findData( value );
            if ( idx < 0 ) return;
            cb->setCurrentIndex( idx );
            */
            __setComboBoxValue( editor, index );
            
        }; break;
        
        // case AgentPropertyElement::SproutTypeSelector : {
            /*
            int value = index.model()->data( index, Qt::EditRole ).toInt();
            QComboBox * cb = static_cast< QComboBox * >( editor );
            int idx = cb->findData( value );
            */
        // };
    };
        
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                      Set model data from combo-box editor.                                       *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                 Установить данные модели из выпадающего списка.                                  *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AgentPropertyDelegate::__setModelValueFromComboBox ( QWidget* editor, QAbstractItemModel* model, const QModelIndex& index ) {
    QComboBox * cb = static_cast<QComboBox*>( editor );
    int idx = cb->currentIndex();
    if ( idx < 0 ) return;
    int value = cb->itemData( idx ).toInt();
    model->setData( index, value, Qt::EditRole );
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
        
        case AgentPropertyElement::String: 
        case AgentPropertyElement::Float:
        {
            QStyledItemDelegate::setModelData ( editor, model, index );
        }; break;
        
        case AgentPropertyElement::ExecutionModeSelector :
        case AgentPropertyElement::SproutTypeSelector :
        case AgentPropertyElement::SproutAngleSelector :
        {
            
            __setModelValueFromComboBox( editor, model, index );
            /*
            QComboBox * cb = static_cast<QComboBox*>( editor );
            int idx = cb->currentIndex();
            if ( idx < 0 ) return;
            int value = cb->itemData( idx ).toInt();
            model->setData( index, value, Qt::EditRole );
            */
        }; break;
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


