// ********************************************************************************************************************
// *                                                                                                                  *
// *                                    The tab for items that are relevant to processes                              *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                            Вкладка для элементов, которые имеют отношение к процессам.                           *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 12 jun 2017 at 15:23 *
// ********************************************************************************************************************

#include "LibraryTab_Processes.h"

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                  The constructor.                                                *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                     Конструктор.                                                 *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::LibraryTab__processes::LibraryTab__processes ( QWidget* parent, Qt::WindowFlags f ) 
    : QWidget ( parent, f )
{
    setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
    QHBoxLayout * lay = new QHBoxLayout();
    lay->setAlignment( Qt::AlignLeft );
    // lay->setDirection( Qt::LeftToRight );
    lay->setMargin( 2 );
    lay->setSpacing( 1 );
    setLayout( lay );
        
    button__process_begin = new LibraryTab_Button( QIcon( QPixmap(":process_begin_16.png") ), tr("Begin of process")  );
    button__process_begin->setDrag( ProcessStart().toJSON() );
    lay->addWidget( button__process_begin );
    
    button__process_end = new LibraryTab_Button( QIcon( QPixmap(":process_end_16.png")), tr("End of process")  );
    button__process_end->setDrag( ProcessStop().toJSON() );
    lay->addWidget( button__process_end );
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                  Init each one button to make all buttons equivalent.                            *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                              Инициализация каждой кнопки, чтобы они были одинаковыми.                            *
// *                                                                                                                  *
// ********************************************************************************************************************
/*
void tengu::LibraryTab__processes::__init_button ( QPushButton * button ) {
    
    button->setFlat( true );
    button->setFixedSize( QSize(22, 22 ) );    
    // button->setAutoFillBackground( true );
}
*/
// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                   The destructor.                                                *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                     Деструктор.                                                  *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::LibraryTab__processes::~LibraryTab__processes() {

}


