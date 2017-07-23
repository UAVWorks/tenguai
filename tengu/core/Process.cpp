// ********************************************************************************************************************
// *                                                                                                                  *
// *          The process. A container for tasks. It have a start point and stop('s) point(-s), it execute            *
// *                                                 by focus transition.                                             *
// * ---------------------------------------------------------------------------------------------------------------- *
// *         Процесс (контейнер для задач). Имеет точку старта и одну или несколько точек останова. Выполняется       *
// *                                                путем перехода фокуса.                                            *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 09 jun 2017 at 10:33 *
// ********************************************************************************************************************

#include "Process.h"

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                  The constructor.                                                *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                     Конструктор.                                                 *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::Process::Process () 
    : AbstractAgent ()
{
    _class_name = "Process";
    _entity_type = AbstractEntity::ET_Process;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                              The copying constructor.                                            *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                               Конструктор копирования.                                           *
// *                                                                                                                  *
// ********************************************************************************************************************
/*
tengu::Process::Process ( const tengu::Process & o ) 
    : Process()
{

}
*/

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                            Convert object to JSON form.                                          *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                         Преобразовать объект в JSON-форму.                                       *
// *                                                                                                                  *
// ********************************************************************************************************************

QJsonObject tengu::Process::toJSON() {
    
    QJsonObject o = AbstractAgent::toJSON();
    o[ JSON_COLLECTION_ELEMENT ] = "processes";
    // if ( _parent ) o["parent_uuid"] = _parent->getUUID();
    return o;
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                    Overrided function for add a child to this process.                           *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                    Перекрытая функция добавления дитя в данный процесс.                          *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::Process::addChild ( tengu::AbstractAgent * child ) {
    
    tengu::AbstractAgent::addChild ( child );
    
    // Automatically names
    // Автоматические имена.
    
    Task * task = dynamic_cast<Task*> ( child );
    ProcessStop * stop = dynamic_cast< ProcessStop * > ( child );
    ANDor * andor = dynamic_cast< ANDor * > ( child );
    ORer * orer = dynamic_cast < ORer * > ( child );
    
    bool systemNameEmpty = child->getSystemName().isEmpty();
    bool humanNameEmpty = child->getHumanName().isEmpty();
    
    if ( systemNameEmpty | humanNameEmpty ) {
        
        if( task ) {        
            int taskCount = childrenCountOf< Task * >() + 1;
            if ( systemNameEmpty ) child->setSystemName( tr("New_Task_") + QString::number( taskCount ) );
            if ( humanNameEmpty ) child->setHumanName( tr("New Task ") + QString::number( taskCount ) );
        };
    
        if ( stop ) {
            int stopCount = childrenCountOf<ProcessStop * >() + 1;
            if ( systemNameEmpty ) child->setSystemName( tr("ProcessStop_") + QString::number( stopCount ) );
            if ( humanNameEmpty ) child->setHumanName( tr("Process Stop ") + QString::number( stopCount ) );
        };
        
        if ( andor ) {
            int andorCount = childrenCountOf<ANDor *>() + 1;
            if ( systemNameEmpty ) child->setSystemName( tr("Block_AND_") + QString::number( andorCount ) );
            if ( humanNameEmpty ) child->setHumanName( tr("Block AND ") + QString::number( andorCount ) );
        };
        
        if ( orer ) {
            int orerCount = childrenCountOf< ORer *>() + 1;
            if ( systemNameEmpty ) child->setSystemName( tr("Block_OR_") + QString::number( orerCount ) );
            if ( humanNameEmpty ) child->setHumanName( tr("Block OR ") + QString::number( orerCount ) );
        };
    }
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                           Start of process execution.                                            *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                            Старт выполнения процесса.                                            *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::Process::start() {

    qDebug() << "Process::start()";
    // Finding ProcessStart element in children.
    // Поиск элемента ProcessStart в детях.
    
    ProcessStart * pstart = nullptr;
    
    QList< AbstractAgent * > chi = children();
    
    for ( int i=0; i<chi.count(); i++ ) {
        ProcessStart * cur = dynamic_cast< ProcessStart * > ( chi.at(i) );
        if ( cur ) {
            pstart = cur;
            break;
        };
    };
    
    if ( pstart ) {
        
        // Start execution of process.
        // Начало выполнения процесса.
        
        for ( int i=0; i<chi.count(); i++ ) {
            AbstractAgent * c = chi.at(i);
            QObject::connect( c, SIGNAL( signalFocused( bool ) ), this, SLOT( __on__agent__focused( bool ) ) );
            QObject::connect( c, SIGNAL( signalActivated( bool ) ), this, SLOT( __on__agent__activated( bool ) ) );
            QObject::connect( c, SIGNAL( signalFinished() ), this, SLOT( __on__agent__finished() ) );
            QObject::connect( c, SIGNAL( signalFailed( QString ) ), this, SLOT( __on__agent__failed( QString ) ) );
        };
        
        // Instead of assigning a focus to the start element, we immediately assign focus to his children.
        // Вместо назначения фокуса на узел старта, сразу назначаем на его детей.
        // --- pstart->setFocus( true );
        
        QList<AbstractAgent * > to_focus = pstart->nextByFocus();
        if ( to_focus.count() > 0 ) {
            for ( int i=0; i< to_focus.count(); i++ ) {
                to_focus.at(i)->setFocus( true );
            };
        } else {
            __stopExecution();
            emit signalFailed( tr("ProcessStart element does not have the following by focus") );
        };
        
    } else {
        
        // We did'nt start block.
        // Не нашли стартового блока.
        
        __stopExecution();
        emit signalFailed( tr("There is not ProcessStart element.") );
    }
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                         Stop execution of this process.                                          *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                      Остановка выполнения данного процесса.                                      *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::Process::__stopExecution() {
    
    QList<AbstractAgent * > chi = children();
    for ( int i=0; i<chi.count(); i++ ) {
        AbstractAgent * c = chi.at(i);
        QObject::disconnect( c, SIGNAL( signalFocused( bool ) ), this, SLOT( __on__agent__focused( bool ) ) );
        QObject::disconnect( c, SIGNAL( signalActivated( bool ) ), this, SLOT( __on__agent__activated( bool ) ) );
        QObject::disconnect( c, SIGNAL( signalFinished() ), this, SLOT( __on__agent__finished() ) );
        QObject::disconnect( c, SIGNAL( signalFailed( QString ) ), this, SLOT( __on__agent__failed( QString ) ) );
    };
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                              An agent has been focused.                                          *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                          У агента случилось событие фокуса.                                      *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::Process::__on__agent__focused ( bool focus ) {
    AbstractAgent * agent = dynamic_cast< AbstractAgent * > ( sender() );
    if ( agent ) {
        qDebug() << "Process::on_agent_focused, name=" << agent->getHumanName() << ", focus=" << focus;
    };
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                              Agent has been activated.                                           *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                Агент был активирован.                                            *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::Process::__on__agent__activated( bool activity ) {
    AbstractAgent * agent = dynamic_cast< AbstractAgent * >( sender() );
    if ( agent ) {
        qDebug() << "Process::__on_agent_activated( " << activity << "), " << agent->getHumanName();
    };
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                    The execution of the agent has been finished.                                 *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                              Выполнение агента закончилось.                                      *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::Process::__on__agent__finished() {
    AbstractAgent * agent = dynamic_cast< AbstractAgent * > ( sender() );
    if ( agent ) {
        qDebug() << "Process::on agent finished, name=" << agent->getHumanName();
    };
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                       The execution of the agent has been failed.                                *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                           Выполнение агента было провалено.                                      *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::Process::__on__agent__failed ( QString errorMessage ) {
    AbstractAgent * agent = dynamic_cast< AbstractAgent * > ( sender() );
    if ( agent ) {
        qDebug() << "Process::on_agent_failed: " << agent->getHumanName() << ", msg=" << errorMessage;
    };
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                   The destructor.                                                *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                      Деструктор.                                                 *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::Process::~Process() {
    
}
