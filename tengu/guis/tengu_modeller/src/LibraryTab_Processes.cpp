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
    // QHBoxLayout * lay = new QHBoxLayout();
    QGridLayout * lay = new QGridLayout();
    lay->setAlignment( Qt::AlignLeft );
    // lay->setDirection( Qt::LeftToRight );
    lay->setMargin( 2 );
    lay->setSpacing( 1 );
    setLayout( lay );
        
    button__process_begin = new LibraryTab_Button( QIcon( QPixmap(":process_begin_16.png") ), tr("Begin of process")  );
    button__process_begin->setDrag( ProcessStartItem().toJSON() );
    lay->addWidget( button__process_begin, 0, 0 );
    
    button__process_end = new LibraryTab_Button( QIcon( QPixmap(":process_end_16.png")), tr("End of process")  );
    button__process_end->setDrag( ProcessStopItem().toJSON() );
    lay->addWidget( button__process_end, 1, 0 );
    
    button__focus_arrow = new LibraryTab_Button( QIcon( QPixmap(":arrow_right_16.png") ), tr("Binding arrow") );
    button__focus_arrow->setDrag( LinkItem().toJSON() );
    lay->addWidget( button__focus_arrow, 0, 1 );
    
    button__focus_andor = new LibraryTab_Button( QIcon( QPixmap(":arrow_switch_and_16.png") ), tr("Branching logical AND block") );
    button__focus_andor->setDrag( ANDorItem().toJSON() );
    lay->addWidget( button__focus_andor, 1, 1 );
    
    button__focus_orer = new LibraryTab_Button( QIcon( QPixmap(":arrow_switch_or_16.png") ), tr("Branching logical OR block") );
    button__focus_orer->setDrag( ORerItem().toJSON() );
    lay->addWidget( button__focus_orer, 0, 2 );
    
    button__task = new LibraryTab_Button( QIcon( QPixmap(":box_16.png") ), tr("The Task") );
    button__task->setDrag( TaskItem().toJSON() );
    lay->addWidget( button__task, 1, 2 );
    
    button__sprout = new LibraryTab_Button( QIcon( QPixmap(":button_navigation_16.png")), tr("The Sprout") );
    lay->addWidget( button__sprout, 0, 3 );
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
// *                                     Disable all elements except process start.                                   *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                      Запретить все, кроме "начала процесса".                                     *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::LibraryTab__processes::on__process_created() {
    button__process_begin->setEnabled( true );
    button__process_end->setEnabled( false );
    button__focus_arrow->setEnabled( false );
    button__focus_andor->setEnabled( false );
    button__focus_orer->setEnabled( false );
    button__task->setEnabled( false );
    button__sprout->setEnabled( false );
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                         The begin element has been created.                                      *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                         Был создан элемент "начало процесса".                                    *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::LibraryTab__processes::on__process_begin_created() {
    
    button__process_begin->setEnabled( false );
    button__process_end->setEnabled( true );
    button__focus_arrow->setEnabled( false );
    button__focus_andor->setEnabled( true );
    button__focus_orer->setEnabled( true );
    button__task->setEnabled( true );
    button__sprout->setEnabled( false );
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                               Some kind of task has been created, we can use links.                              *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                           Был создан какой-то тип задачи - мы можем использовать связи.                          *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::LibraryTab__processes::on__process_some_task_created() {
    button__focus_arrow->setEnabled( true );
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                       There was created exactly a task element, we can use Sprouts.                              *
// * ---------------------------------------------------------------------------------------------------------------- *
// *               Был создан в точности элемент "задача", поэтому мы можем использовать "ростки".                    *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::LibraryTab__processes::on__process_explicit_task_created() {
    button__sprout->setEnabled( true );
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                   The destructor.                                                *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                     Деструктор.                                                  *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::LibraryTab__processes::~LibraryTab__processes() {

}


