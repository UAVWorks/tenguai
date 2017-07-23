// ********************************************************************************************************************
// *                                                                                                                  *
// *                                         Dialog to work with model's storage (MongoDB).                           *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                       Диалог для работы с хранилищем моделей (MongoDB)                           *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 07 jul 2017 at 05:56 *
// ********************************************************************************************************************

#include "DialogOpenSaveModel.h"

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                       The constructor.                                           *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                         Конструктор.                                             *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::DialogOpenSaveModel::DialogOpenSaveModel( MongoStorage * mongo ) {
    
    __mongo = mongo;
    __selector[ JSON_DATABASE_ELEMENT ] = TENGU_DATABASE;
    __current_mode = DM_OPEN;
    result_agent = nullptr;
    
    setWindowIcon( QIcon(QPixmap(":folder_16.png")) );
    setWindowTitle( tr("Open...") );
    _buttonOk->setText( tr("Open") );
    _buttonOk->setIcon( QIcon( QPixmap(":folder_16.png") ) );
    setMinimumSize( 800, 500 );
    
    // Horizontal layout for storage selector and element's list
    // Горизонтальный менеджер расположения - для селектора хранилищ и списка элементов.
    
    QHBoxLayout * lay = new QHBoxLayout();
    lay->setMargin( 0 );
    lay->setSpacing( 0 );
    _centerWidget->setLayout( lay );
    
    // Left part - storage selector
    // Левая часть - выбор хранилища.
    
    lay->addWidget( __createStorageSelector() );
    lay->setStretch( 0, 2 );
    
    // Right part, list of elements
    // Правая часть, список элементов.
    
    lay->addWidget( __createElementsList() );
    lay->setStretch( 1, 5 );
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                      Right part of the dialog, the element's list.                               *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                         Правая часть диалога, список элементов.                                  *
// *                                                                                                                  *
// ********************************************************************************************************************

QFrame * tengu::DialogOpenSaveModel::__createElementsList() {
    
    QFrame * frame = new QFrame();
    
    QVBoxLayout * lay = new QVBoxLayout();
    lay->setMargin( 5 );
    lay->setSpacing( 5 );
    lay->setDirection( QBoxLayout::BottomToTop );
    frame->setLayout( lay );
    
    __combo_box__type_of_elements = new QComboBox();
    QObject::connect( __combo_box__type_of_elements, SIGNAL( activated( int ) ), this, SLOT( __on__type_of_elements_changed( int ) ) );
    
    __combo_box__type_of_elements->addItem( QIcon( QPixmap(":robot_32.png") ), tr("Vehicle (a car, an aircraft, a helicopter e.t.c.)"), OSM_VEHICLE );
    __combo_box__type_of_elements->addItem( QIcon( QPixmap( ":package_16.png" ) ) , tr("Process"), OSM_PROCESS );
    __combo_box__type_of_elements->addItem( QIcon( QPixmap( ":box_16.png" ) ) , tr("Task"), OSM_TASK );
    
    lay->addWidget( __combo_box__type_of_elements );
    
    __table_of_elements = new QTableWidget();
    QObject::connect( __table_of_elements->selectionModel(), SIGNAL( selectionChanged( QItemSelection ,QItemSelection )), this, SLOT( __on__table_element_selected( QItemSelection , QItemSelection ) ) );
    __table_of_elements->setColumnCount( 1 );
    __table_of_elements->horizontalHeader()->hide();
    __table_of_elements->verticalHeader()->hide();
    
    QHeaderView * hh = __table_of_elements->horizontalHeader();
    hh->setSectionResizeMode( QHeaderView::Stretch );
    
    QHeaderView * vh = __table_of_elements->verticalHeader();
    vh->setSectionResizeMode(QHeaderView::Fixed);
    vh->setDefaultSectionSize( 16 );    
    
    lay->addWidget( __table_of_elements );
    
    // Top element is searcher.
    // Верхний элемент, поле поиска.
    
    QWidget * searcherWidget = new QWidget();
    QHBoxLayout * searcherLay = new QHBoxLayout();
    searcherLay->setMargin( 0 );
    searcherLay->setSpacing( 5 );
    searcherLay->setDirection( QBoxLayout::RightToLeft );
    searcherWidget->setLayout( searcherLay );
    
    QLabel * searcherLabel = new QLabel();
    searcherLabel->setPixmap( QPixmap( ":magnifier_16.png" ) );
    searcherLay->addWidget( searcherLabel );
    
    __searcher = new QLineEdit();
    searcherLay->addWidget( __searcher );
    
    lay->addWidget( searcherWidget );
    
    
    return frame;
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                         Left part of dialog, storage selector.                                   *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                          Левая часть диалога, выбор хранилища.                                   *
// *                                                                                                                  *
// ********************************************************************************************************************

QFrame* tengu::DialogOpenSaveModel::__createStorageSelector() {
    
    QFrame * storageSelector = new QFrame();
    
    QVBoxLayout * lay = new QVBoxLayout();
    lay->setMargin( 5 );
    lay->setSpacing( 5 );
    lay->setDirection( QBoxLayout::TopToBottom );
    lay->setAlignment( Qt::AlignTop );
    storageSelector->setLayout( lay );
    
    QWidget * widget_local_storage = __createStorageLabel( QPixmap(":iDatabase_20.png"), QString("Local storage") );
    lay->addWidget( widget_local_storage );
    
    return storageSelector;
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                        Create one widget for storage selector.                                   *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                      Создание одного виджита для выбора хранилища.                               *
// *                                                                                                                  *
// ********************************************************************************************************************

QWidget * tengu::DialogOpenSaveModel::__createStorageLabel ( QPixmap pixmap, QString text ) {
    
    QWidget * w = new QWidget();
    
    QHBoxLayout * lay = new QHBoxLayout();
    lay->setMargin( 5 );
    lay->setSpacing( 5 );
    lay->setAlignment( Qt::AlignLeft | Qt::AlignVCenter );
    lay->setDirection( QBoxLayout::LeftToRight );
    w->setLayout( lay );
    
    QLabel * lblPixmap = new QLabel();
    lblPixmap->setPixmap( pixmap );    
    lay->addWidget( lblPixmap );
    
    QLabel * lblText = new QLabel( text );
    QFont font( "Arial", 14 );
    font.setBold( false );
    lblText->setFont( font );
    
    lay->addWidget( lblText ) ;
    
    return w;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                            Appearance the dialog on the screen.                                  *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                 Появление диалога на экране.                                     *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::DialogOpenSaveModel::showEvent ( QShowEvent* event ) {
    
    QDialog::showEvent ( event );
    __fill_table_of_elements();
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                     The desired element type has been changed.                                   *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                          Желаемый тип элемента был изменен.                                      *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::DialogOpenSaveModel::__on__type_of_elements_changed( int type ) {
    
    __fill_table_of_elements();
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                            Fill element's table according selector, filter and element type.                     *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                          Заполнить таблицу элементов согласно селектору, фильтру и типу элемента                 *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::DialogOpenSaveModel::__fill_table_of_elements() {
    
    _clearTable( __table_of_elements );
    _buttonOk->setEnabled( false );
    
    int rows = 0;
    QString collectionName = QString("");
    switch ( __combo_box__type_of_elements->currentData( Qt::UserRole).toInt() ) {
        case OSM_VEHICLE: collectionName = "vehicles"; break;
        case OSM_PROCESS: collectionName = "processes"; break;
        case OSM_TASK: collectionName = "tasks"; break;
        
    };
    
    if ( ! collectionName.isEmpty() ) {
        
        __selector[ JSON_COLLECTION_ELEMENT ] = collectionName;
        __selector.remove( JSON_UUID_ELEMENT );
        
        QList< QJsonObject > elements = __mongo->read( __selector, false );
        if ( elements.count() > 0 ) {
            for ( int i=0; i<elements.count(); i++ ) {
                QJsonObject element = elements.at( i );
                qDebug() << "DialogOpenSaveModel::fill_table_of_elements, open element " << element << "\n";
                
                AbstractEntity * entity = AgentItemFactory::createEntity( element );
                if ( entity ) {
                    
                    __table_of_elements->insertRow( rows );
                    
                    QTableWidgetItem * item = new QTableWidgetItem();
                    item->setFlags( item->flags() & ~Qt::ItemIsEditable );
                    item->setText( entity->getHumanName() );
                    item->setToolTip( entity->getComment() );
                    item->setData( Qt::UserRole, QVariant( entity->getUUID() ) );
                    __table_of_elements->setItem( rows, 0, item );
                    
                    rows ++;
                    
                    // We did it for visibility only, the object itself is not need, the memory must be cleared.
                    // Мы это делали только для того, чтобы показать. Объект сам по себе не нужен, память нужно освободить.
                    
                    delete( entity );
                };
            };
        };
    };
};

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                    Operation which goes after "Ok" button pressed.                               *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                         Операции после нажатия на кнопку "Ок".                                   *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::DialogOpenSaveModel::_on__ok() {
    
    result_agent = nullptr;
    
    QList<QJsonObject> readed = __mongo->read( __selector, true );
    
    if ( readed.isEmpty() ) emit signalError( EL_WARNING, "DialogOpenSaveModel::_on__ok()", tr("Empty set of object was readed") );
    else {
        
        qDebug() << "DialogOpenSaveModel, create for : " << readed.at(0) << "\n";
        
        result_agent = dynamic_cast< AbstractAgent * > ( AgentItemFactory::createEntity( readed.at(0) ) );
        
        if ( ! result_agent ) {
            emit signalError( EL_WARNING, "DialogOpenSaveModel::_on__ok()", tr("Result agent is empty") );
        }
    }
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                      The element was selected in element's table.                                *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                В таблице выбрали элемент                                         *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::DialogOpenSaveModel::__on__table_element_selected ( QItemSelection current, QItemSelection previous ) {
    
    __selector.remove( JSON_UUID_ELEMENT );
    _buttonOk->setEnabled( true );
    QList<QTableWidgetItem * > selectedList = __table_of_elements->selectedItems();
    
    if ( selectedList.count() > 0 ) {
        QString uuid = selectedList.at(0)->data( Qt::UserRole ).toString();
        if ( ! uuid.isEmpty() ) {
            _buttonOk->setEnabled( true );
            __selector[ JSON_UUID_ELEMENT ] = uuid;
        };
    };
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                           Set current mode of this dialog.                                       *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                       Установить текущий режим данного диалога.                                  *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::DialogOpenSaveModel::setCurrentMode ( tengu::DialogOpenSaveModel::dialog_mode_t mode ) {
    __current_mode = mode;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                       The destructor.                                            *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                         Деструктор.                                              *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::DialogOpenSaveModel::~DialogOpenSaveModel() {

}

