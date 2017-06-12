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
    __properties = QList<QPair<QString, QVariant>>();
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
// *                                            Make flags for this index.                                            *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                            Флаги для данного индекса.                                            *
// *                                                                                                                  *
// ********************************************************************************************************************

Qt::ItemFlags tengu::AgentPropertyModel::flags(const QModelIndex & index) const {
        
    Qt::ItemFlags defaultFlags = Qt::ItemIsEnabled | Qt::ItemNeverHasChildren;
    
    if ( !index.isValid() ) return defaultFlags;
    
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
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                        Return data for this model index.                                         *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                         Вернуть данные для этого индекса.                                        *
// *                                                                                                                  *
// ********************************************************************************************************************

QVariant tengu::AgentPropertyModel::data( const QModelIndex & index, int role ) const {
        
    QVariant answer;
    
    if ( ! index.isValid() ) return answer;
    
    if ( ( role == Qt::DisplayRole ) || ( role == Qt::EditRole ) ) {
    
        // Want to display. Return contents of properties list.
        // Хотим показываать. Возвращаем содержание списка свойств.                
        
        if ( index.row() < __properties.size() ) {
            if ( index.column() == 0 ) answer = QVariant( __properties.at( index.row() ).first );
            else answer = __properties.at( index.row() ).second;
        };
        
        return answer;
    };
    
    if ( role == Qt::BackgroundColorRole ) {
        
        // Using temporary invisible widget to grab system background color.
        // Используем временный невидимый виджит, чтобы стащить системный цвет фона.
        
        QWidget w;
        QColor widgetBackground = w.palette().color( QPalette::Window );
        
        // Background for UUID contents which is disable for editing.
        // Цвет фона для содержимого UUID'а, которое нельзя редактировать.
        
        QColor disabledBackground = w.palette().color( QPalette::Button );
        
        if ( index.column() == 0 ) return QVariant( widgetBackground );
        else if ( index.row() == 0 ) return QVariant( disabledBackground );
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
        QFont font("Tahoma", 9 );
        return QVariant( font );
    };
        
    // qDebug() << "get data, but not for display. Role=" << role;
    
    return answer;
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                          Edit data for specified index                                           *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                  Редактирование данных для указанного индекса.                                   *
// *                                                                                                                  *
// ********************************************************************************************************************

bool tengu::AgentPropertyModel::setData(const QModelIndex & index, const QVariant & value, int role) {
    
    qDebug() << "set data row=" << index.row() << ", value=" << value << ", role=" << role;
    
    if (index.isValid() && role == Qt::EditRole) {
        /*
        stringList.replace(index.row(), value.toString());
        emit dataChanged(index, index);
        */
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


