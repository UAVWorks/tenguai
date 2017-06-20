// ********************************************************************************************************************
// *                                                                                                                  *
// *                                          Table MVC-model for agent's properties.                                 *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                          Табличная MVC-модель для свойств агента.                                *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 01 jun 2017 at 11:35 *
// ********************************************************************************************************************

#include "AgentPropertyModel.h"

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                   The constructor.                                               *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                      Конструктор.                                                *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::AgentPropertyModel::AgentPropertyModel()
    : QAbstractItemModel()
{
    __item = nullptr;
    __properties = QList< QList< AgentPropertyElement >>();
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                      Return the column count for this MVC-model.                                 *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                     Вернуть число столбцов для данной MVC-модели.                                *
// *                                                                                                                  *
// ********************************************************************************************************************

int tengu::AgentPropertyModel::columnCount(const QModelIndex & parent ) const {
    return 2;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                      Return the row count for this model element.                                *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                    Вернуть число строк для данного элемента модели.                              *
// *                                                                                                                  *
// ********************************************************************************************************************

int tengu::AgentPropertyModel::rowCount( const QModelIndex & parent ) const {
    return __properties.count();
};

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                    Return model index for specivied element                                      *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                 Вернуть индекс в модели для указанного элемента.                                 *
// *                                                                                                                  *
// ********************************************************************************************************************

QModelIndex tengu::AgentPropertyModel::index(int row, int column, const QModelIndex & parent ) const {

    QModelIndex res = createIndex( row, column );
    return res;
};

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                          Get a parent for the child.                                             *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                      Вернуть родителя для данного дитяти.                                        *
// *                                                                                                                  *
// ********************************************************************************************************************

QModelIndex tengu::AgentPropertyModel::parent( const QModelIndex & child ) const {

    QModelIndex res;
    return res;
};

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                    Get one property element for this index.                                      *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                  Вернуть один элемент свойств агента по индексу.                                 *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::AgentPropertyElement tengu::AgentPropertyModel::propertyElement ( const QModelIndex & index, bool * ok ) {
    
    ( * ok ) = false;
    
    if ( index.row() >= __properties.size() ) return AgentPropertyElement();        
    QList < AgentPropertyElement > oneRow = __properties.at( index.row() );    
    if ( index.column() > oneRow.size() ) return AgentPropertyElement();    
    AgentPropertyElement oneElement = oneRow.at( index.column() );
    
    ( * ok ) = true;    
    return oneElement;
}


// ********************************************************************************************************************
// *                                                                                                                  *
// *                                            Make flags for this index.                                            *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                            Флаги для данного индекса.                                            *
// *                                                                                                                  *
// ********************************************************************************************************************

Qt::ItemFlags tengu::AgentPropertyModel::flags( const QModelIndex & index) const {
        
    Qt::ItemFlags defaultFlags = Qt::ItemIsEnabled | Qt::ItemNeverHasChildren;
    
    if ( !index.isValid() ) return defaultFlags;
    
    bool ok = false;
    tengu::AgentPropertyElement oneElement = propertyElement( index, & ok );
    if ( ! ok ) return defaultFlags;
            
    defaultFlags = defaultFlags | QAbstractItemModel::flags( index );
    
    // Is this property editable?
    // Редактируемо ли данное свойство?
    
    if ( ( ! oneElement.readOnly ) && ( ! oneElement.propertyName.isEmpty() ) ) return defaultFlags | Qt::ItemIsEditable;
    
    return defaultFlags;

    /*
    // At least the index is valid.
    // Как минимум, индекс - вменяемый.
    
    if ( index.row() == 0 ) {
        
        // First row never editable.
        // Первый столбец не редактируется/
        
        return defaultFlags;
        
    };
    
    // UUID field is never editable.
    
    if ( index.column() == 0 ) return defaultFlags;
    
    // All another fields is editable.
    // Все остальные поля можно редактировать.
    
    return QAbstractItemModel::flags( index ) | Qt::ItemIsEditable | defaultFlags;
    */
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                        Return data for this model index.                                         *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                         Вернуть данные для этого индекса.                                        *
// *                                                                                                                  *
// ********************************************************************************************************************

QVariant tengu::AgentPropertyModel::data( const QModelIndex & index, int role ) const {
            
    if ( ! index.isValid() ) return QVariant();
    
    // Get exactly needed element of properties.
    // Получаем конкретный элемент свойств.
    
    bool ok;
    AgentPropertyElement oneElement = propertyElement( index, & ok );
    if ( ! ok ) return QVariant();
        
    if ( ( role == Qt::DisplayRole ) || ( role == Qt::EditRole ) ) {
    
        // Want to display. Return the value of specified element.
        // Хотим показываать. Возвращаем значение указанного элемента.
        
        switch ( oneElement.type ) {
            
            case AgentPropertyElement::APE_String: {
                return oneElement.value ;
            }; break;
            
            case AgentPropertyElement::APE_ExecutionModeSelector: {
                switch ( ( AbstractAgent::execution_mode_t ) oneElement.value.toInt() ) {
                    case AbstractAgent::EM_ALWAYS:  return QVariant( tr("Always") );
                    case AbstractAgent::EM_REAL:    return QVariant( tr("Real") );
                    case AbstractAgent::EM_XPLANE:  return QVariant( tr("X-Plane simulation") );
                };                                
            };
        };
                                        
    };
    
    if ( role == Qt::BackgroundColorRole ) {        
        return oneElement.backgroundColor;
    };        
    
    // Does not work... :-(
    // Не работает... :-(
    //if ( role == Qt::BackgroundRole ) {
    //    
    //    // The background role. Will return QBrush object.
    //    // Роль - "фон". Будем возвращать объект QBrush.
    //    
    //    QBrush b;
    //    b.setColor( QColor(255,0,0) );
    //    return QVariant( b );
    //    
    //};
    
    if ( role == Qt::FontRole ) {
        
        //QFont font("Tahoma", 9 );
        return QVariant( oneElement.font );
    };
        
    // qDebug() << "get data, but not for display. Role=" << role;
    
    return QVariant();
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                          Edit data for specified index                                           *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                  Редактирование данных для указанного индекса.                                   *
// *                                                                                                                  *
// ********************************************************************************************************************

bool tengu::AgentPropertyModel::setData( const QModelIndex & index, const QVariant & value, int role) {
    
    
    if ( ! index.isValid() ) return false;
    
    bool ok;
    AgentPropertyElement oneElement = propertyElement( index, & ok );
    if ( ! ok ) return false;
    
    if ( ( role == Qt::EditRole ) && ( ! oneElement.readOnly ) && ( ! oneElement.propertyName.isEmpty() ) ) {
        
        switch ( oneElement.type ) {
            
            case AgentPropertyElement::APE_String: {
                __item->setProperty( oneElement.propertyName.toLatin1().data(), value );
            }; break;
            
            case AgentPropertyElement::APE_ExecutionModeSelector : {
                __item->setExecutionMode( (AbstractAgent::execution_mode_t) value.toInt() );
            }; break;
        };
                
        __properties = __item->properties();        
        emit dataChanged( index, index );
        
        return true;        
    }
    
    return false;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                          Set appropriate agent for data model.                                   *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                  Установка соответствующего агента для модели данных.                            *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AgentPropertyModel::setEntityItem ( tengu::AbstractEntityItem * item ) {
    __item = item;
    __properties = item->properties();    
    emit layoutChanged();
    
}


// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                    The destructor.                                               *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                       Деструктор.                                                *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::AgentPropertyModel::~AgentPropertyModel() {
}


