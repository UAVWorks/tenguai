// ********************************************************************************************************************
// *                                                                                                                  *
// *                                             Dialog with the task's properties.                                   *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                 Диалог со свойствами задачи.                                     *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 23 jun 2017 at 14:59 *
// ********************************************************************************************************************

#include "DialogPropertiesTask.h"

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                    The constructor.                                              *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                      Конструктор.                                                *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::DialogPropertiesTask::DialogPropertiesTask ( tengu::WorkSpace* workSpace ) 
    : DialogProperties ( workSpace )
{
    setMinimumSize( 800, 600 );
    
    _centerWidget->setFrameStyle( QFrame::NoFrame );
    
    QHBoxLayout * lay = new QHBoxLayout();
    lay->setMargin( 0 );
    lay->setSpacing( 0 );
    lay->setAlignment( Qt::AlignCenter );
    _centerWidget->setLayout( lay );
    
    __tab = new QTabWidget();
    __tab->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
    
    lay->addWidget( __tab );
    
    __tab__start_condition = new DialogPropertiesTask_TabStartCondition();
    __tab->addTab( __tab__start_condition, tr("Start condition") );
    
    __tab__algorythm = new DialogPropertiesTask_TabAlgorythm();
    __tab->addTab( __tab__algorythm, tr("Algorythm") );
    
    __tab__stop_condition = new DialogPropertiesTask_TabStopCondition();
    __tab->addTab( __tab__stop_condition, tr("Stop condition") );
    
    __tab_qml = new DialogPropertiesTask_TabQML();
    __tab->addTab( __tab_qml, "QML" );
    
    __task = nullptr;
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                              Fill dialog from the item                                           *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                            Заполнение диалога из элемента.                                       *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::DialogPropertiesTask::fillFrom(tengu::AbstractEntityItem * item) {
    TaskItem * taskItem = dynamic_cast< TaskItem * > ( item );
    if ( taskItem ) {
        __task = taskItem->task();
        if ( __task ) {
            __tab__start_condition->start_condition->setPlainText( __task->startCondition() );
            __tab__algorythm->algorythm->setPlainText( __task->algorythm() );
            __tab__stop_condition->stop_condition->setPlainText( __task->stopCondition() );
        };
    };
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                        The cancel button has been pressed.                                       *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                             Была нажата кнопка отмены.                                           *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::DialogPropertiesTask::_on__cancel() {
};

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                          The "Ok" button has been pressed.                                       *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                Была нажата кнопка "Ok".                                          *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::DialogPropertiesTask::_on__ok() {

    if ( __task ) {
        __task->setStartCondition( __tab__start_condition->start_condition->toPlainText() );
        __task->setAlgorythm( __tab__algorythm->algorythm->toPlainText() );
        __task->setStopCondition( __tab__stop_condition->stop_condition->toPlainText() );
    };
    
};

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                   The destructor.                                                *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                      Деструктор.                                                 *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::DialogPropertiesTask::~DialogPropertiesTask() {

}

