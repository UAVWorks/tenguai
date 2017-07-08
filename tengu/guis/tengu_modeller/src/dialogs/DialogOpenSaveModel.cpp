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
    __combo_box__type_of_elements->addItem( QIcon( QPixmap(":robot_32.png") ), tr("Vehicle (a car, an aircraft, a helicopter e.t.c.)"), OSM_VEHICLE );
    
    lay->addWidget( __combo_box__type_of_elements );
    
    __table_of_elements = new QTableWidget();
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
    _clearTable( __table_of_elements );

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

