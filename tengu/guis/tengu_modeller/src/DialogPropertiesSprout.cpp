// ********************************************************************************************************************
// *                                                                                                                  *
// *                                        The dialog with sprout's properties.                                      *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                           Диалог со свойствами "отростка".                                       *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 23 jun 2017 at 13:55 *
// ********************************************************************************************************************

#include "DialogPropertiesSprout.h"

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                   The constructor.                                               *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                      Конструктор.                                                *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::DialogPropertiesSprout::DialogPropertiesSprout( WorkSpace * workSpace ) 
    : DialogProperties( workSpace )
{
    __sprout = nullptr;
    
    setWindowTitle( tr("Sprout's properties") );
    setWindowIcon( QIcon(QPixmap( ":button_navigation_16.png" )) );
    setMinimumSize( 600, 360 );
    
    QVBoxLayout * lay = new QVBoxLayout();
    lay->setMargin( 0 );
    lay->setSpacing( 0 );
    _centerWidget->setLayout( lay );
    
    // Top widget, slider or indicator
    // Верхний виджит, слайдер или индикатор.
    
    QWidget * topWidget = new QWidget();
    topWidget->setFixedHeight( 50 );
    QHBoxLayout * topLayout = new QHBoxLayout();
    topLayout->setMargin( 5 );
    topLayout->setSpacing( 5 );
    topWidget->setLayout( topLayout );
    
    QSlider * slider = new QSlider();
    slider->setOrientation( Qt::Horizontal );
    topLayout->addWidget( slider );
    
    lay->addWidget( topWidget );
    
    // --------------------------------------------------------------------------------------------
    //                                        Properties widget
    //                                       Виджит со свойствами.
    // --------------------------------------------------------------------------------------------
    
    QWidget * propWidget = new QWidget();
    propWidget->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
    lay->addWidget( propWidget );
    
    QVBoxLayout * propLay = new QVBoxLayout();
    propLay->setMargin( 0 );
    propLay->setSpacing( 0 );
    propWidget->setLayout( propLay );
    
    // Settings the sprout's type 
    // Установка типа Sprout'а
    
    QWidget * wType = new QWidget();
    propLay->addWidget( wType );
    QHBoxLayout * layoutType = new QHBoxLayout();
    layoutType->setMargin( 5 );
    layoutType->setSpacing( 5 );
    layoutType->setAlignment( Qt::AlignLeft | Qt::AlignVCenter );    
    wType->setLayout( layoutType );
    QLabel * lblType = new QLabel( tr("Signal type:") );
    lblType->setMargin( 0 );
    layoutType->addWidget( lblType );
    
    
    __combo_box__type = new QComboBox();
    __combo_box__type->addItem( tr("In-process Input"), Sprout::IN_PROCESS_INPUT );
    __combo_box__type->addItem( tr("External Input"), Sprout::EXTERNAL_INPUT );
    __combo_box__type->addItem( tr("In-process Output"), Sprout::IN_PROCESS_OUTPUT );
    __combo_box__type->addItem( tr("External Output"), Sprout::EXTERNAL_OUTPUT );
    layoutType->addWidget( __combo_box__type );
    
    
    // Manual setting signal name
    // Ручная установка имени сигнала.
    
    
    QWidget * widgetSignal = new QWidget();
    propLay->addWidget( widgetSignal );
    QHBoxLayout * layoutSignal = new QHBoxLayout();
    layoutSignal->setMargin( 5 );
    layoutSignal->setSpacing( 5 );
    layoutSignal->setAlignment( Qt::AlignLeft | Qt::AlignVCenter );
    widgetSignal->setLayout( layoutSignal );
    
    __check_box__manual = new QCheckBox( tr("Set signal name manually:") );
    QObject::connect( __check_box__manual, SIGNAL(stateChanged(int)), this, SLOT( __on__manual_signal_selection_state_changed( int ) ) );
    layoutSignal->addWidget( __check_box__manual );
    
    __manual_signal_name = new QLineEdit();
    __manual_signal_name->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );
    layoutSignal->addWidget( __manual_signal_name );
    
    
    // Widget for selection sprout's signal name
    // Виджит для выбора сигнала sprout'а.
    
    QWidget * selectorWidget = new QWidget();    
    selectorWidget->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
    propLay->addWidget( selectorWidget );
    QGridLayout * selectorLayout = new QGridLayout();
    selectorLayout->setMargin( 5 );
    selectorLayout->setSpacing( 5 );
    selectorWidget->setLayout( selectorLayout );
    
    // Labels for processes / tasks / sprout selection
    // Метки для выбора процессов / задач / отростков.
    
    QLabel * labelForProcess = new QLabel( tr("Process:") );
    selectorLayout->addWidget( labelForProcess, 0, 0 );
    QLabel * labelForTask = new QLabel( tr("Task:") );
    selectorLayout->addWidget( labelForTask, 0, 1 );
    QLabel * labelForSprout = new QLabel( tr("Sprout:") );
    selectorLayout->addWidget( labelForSprout, 0, 2 );
    
    // Filters for processes / tasks / sprouts
    // Фильтры для процессов / задач / отростков.
    
    __filter_process = new QLineEdit();
    // __filter_process->setMinimumHeight( 23 );
    selectorLayout->addWidget( __filter_process, 1 , 0 );
    
    __filter_task = new QLineEdit();
    // __filter_task->setMinimumHeight( 23 );
    selectorLayout->addWidget( __filter_task, 1, 1 );
    
    __filter_sprouts = new QLineEdit();
    // __filter_sprouts->setMinimumHeight( 23);
    selectorLayout->addWidget( __filter_sprouts, 1, 2 );
    
    // Tables for processes / tasks / sprouts
    // Таблицы для процессов / задач / отростков.
    
    __table_processes = new QTableWidget();
    __init_table( __table_processes );
    selectorLayout->addWidget( __table_processes, 2, 0 );
    
    __table_tasks = new QTableWidget();
    __init_table( __table_tasks );
    selectorLayout->addWidget( __table_tasks, 2, 1 );
    
    __table_sprouts = new QTableWidget();
    __init_table( __table_sprouts );
    selectorLayout->addWidget( __table_sprouts, 2, 2 );
};

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                              Initialize one table widget                                         *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                       Инициализация одного табличного виджита.                                   *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::DialogPropertiesSprout::__init_table ( QTableWidget* table ) {
    table->setMinimumWidth( 80 );
    table->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                            Fill dialog's fields from sprout.                                     *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                          Заполнение полей диалога из "отростка".                                 *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::DialogPropertiesSprout::fillFrom( tengu::AbstractEntityItem * item ) {
    
    SproutItem * sprout = dynamic_cast< SproutItem * > ( item );
    if ( sprout ) {
        __sprout = sprout;
        __check_box__manual->setChecked( __sprout->manualSignalNameSelection );
        // setManualSelection( sprout->manualSignalNameSelection );
    };
};

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                    Set manual selection of sprout's signal name.                                 *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                               Установка ручного способа выбора имени сигнала sprout'а.                           *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::DialogPropertiesSprout::setManualSelection ( bool manual ) {
    
    __manual_signal_name->setEnabled( manual );
    __filter_process->setEnabled( ! manual );
    __filter_task->setEnabled( ! manual );
    __filter_sprouts    ->setEnabled( ! manual );
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                   Slot: change the state of manual check box selection                           *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                             Слот: изменение нажатости крыжика "ручной выбор имени сигнала"                       *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::DialogPropertiesSprout::__on__manual_signal_selection_state_changed ( int state ) {
    setManualSelection( __check_box__manual->isChecked() );
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                    The destructor.                                               *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                       Деструктор.                                                *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::DialogPropertiesSprout::~DialogPropertiesSprout() {
}

