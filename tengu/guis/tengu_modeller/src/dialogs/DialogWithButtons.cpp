// ********************************************************************************************************************
// *                                                                                                                  *
// *                                     Common ancestor for dialogs which have a buttons.                            *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                              Общий предок диалогов с кнопками                                    *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 23 jun 2017 at 13:50 *
// ********************************************************************************************************************

#include "DialogWithButtons.h"

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                    The constructor.                                              *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                      Конструктор.                                                *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::DialogWithButtons::DialogWithButtons () 
    : QDialog() 
{
    
    setModal( true );
    setSizeGripEnabled( false );
    
    QVBoxLayout * lay = new QVBoxLayout();
    lay->setDirection( QBoxLayout::TopToBottom );
    this->setLayout( lay );
    
    _centerWidget = new QFrame();
    _centerWidget->setFrameStyle( QFrame::Sunken | QFrame::StyledPanel );
    _centerWidget->setMinimumSize( QSize(300, 100) );
    _centerWidget->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
    lay->addWidget( _centerWidget );
    
    // Down widget, with a buttons
    // Нижний виджит, с кнопками.
    
    QWidget * buttonWidget = new QWidget();
    buttonWidget->setMaximumHeight( 50 );
    QHBoxLayout * buttonLay = new QHBoxLayout();
    
    buttonLay->setAlignment( Qt::AlignRight | Qt::AlignVCenter );
    buttonWidget->setLayout( buttonLay );
    
    _buttonOk = new QPushButton( QIcon(QPixmap(":accept_button_32.png")), tr("Accept") );
    _buttonOk->setFixedSize( 110, 35 );
    QObject::connect( _buttonOk, SIGNAL(pressed()), this, SLOT( __on__ok_pressed()) );
    buttonLay->addWidget( _buttonOk );
    _buttonCancel = new QPushButton( QIcon( QPixmap(":cancel_32.png") ), tr("Cancel") );
    _buttonCancel->setFixedSize( 110, 35 );
    QObject::connect( _buttonCancel, SIGNAL(pressed()), this, SLOT( __on__cancel_pressed() ) );
    buttonLay->addWidget( _buttonCancel );
    
    lay->addWidget( buttonWidget );
    
    // Focus default to cancel button.
    // Фокус по умолчанию - на кнопку отмены.
    
    _buttonCancel->setFocus();
        
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                      Cancel operation, empty in this class.                                      *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                     Операции при отмене, пустые в данном классе.                                 *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::DialogWithButtons::_on__cancel() {
};

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                             The "Ok" button was pressed.                                         *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                Была нажата кнопка "Ok".                                          *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::DialogWithButtons::__on__ok_pressed() {
    emit accept();
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                            The "Cancel" button was pressed.                                      *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                               Была нажата кнопка "Отмена"                                        *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::DialogWithButtons::__on__cancel_pressed() {
    _on__cancel();
    emit reject();
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                 Clear table contents.                                            *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                              Очистка содержимого таблицы.                                        *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::DialogWithButtons::_clearTable ( QTableWidget * table ) {
    table->clearContents();
    table->setRowCount( 0 );
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                     The destructor.                                              *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                       Деструктор.                                                *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::DialogWithButtons::~DialogWithButtons() {

}

