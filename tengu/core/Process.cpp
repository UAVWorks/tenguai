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
    
    // So far it has not been determined whether the process will be executed or not. Therefore, we do not do initialization in the constructor.
    // Пока что не определено, будет процесс выполняться или нет. Поэтому инициализацию в конструкторе не делаем.
    
    __qmlEngine = nullptr;
    
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
// *                                  Either return or create and return QML Engine.                                  *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                  Вернуть, либо создать и вернуть, QML Engine.                                    *
// *                                                                                                                  *
// ********************************************************************************************************************

QQmlEngine* tengu::Process::qmlEngine() {
    
    if ( __qmlEngine ) return __qmlEngine;
    
    __qmlEngine = new QQmlEngine( this );
    
    // fEngine->globalObject().setProperty("print", print);
    
    return __qmlEngine;
    
}

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
    
    qDebug() << "Process::start(): focused? " << isFocused() << ", active? " << isActive();
    if ( isActive() ) return;
    
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
        
        qDebug() << "Process::start(), we have a ProcessStart block, name is " << pstart->getHumanName();
        
        // Start execution of process.
        // Начало выполнения процесса.
        
        _prepare_for_execution();
        
        // Instead of assigning a focus to the start element, we immediately assign focus to his children.
        // Вместо назначения фокуса на узел старта, сразу назначаем на его детей.
        // --- pstart->setFocus( true );
        
        QList<AbstractAgent * > to_focus = pstart->nextByFocus();
        if ( to_focus.count() > 0 ) {
            for ( int i=0; i< to_focus.count(); i++ ) {
                to_focus.at(i)->setFocus( true, this );
            };
        } else {
            _free_after_execution();
            emit signalFailed( tr("ProcessStart element does not have the following by focus") );
        };
        
    } else {
        
        // We did'nt start block.
        // Не нашли стартового блока.
        
        _free_after_execution();
        emit signalFailed( tr("There is not ProcessStart element.") );
    }
    
    qDebug() << "After process::start(), focus=" << isFocused() << ", activity=" << isActive();
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                               Stop of the whole process.                                         *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                             Остановка процесса полностью.                                        *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::Process::stop() {
    
    qDebug() << "Process::stop()";
    QList<AbstractAgent *> chi;
    childrenRecursive( chi );
    
    for ( int i=0; i<chi.count(); i++ ) {
        chi.at(i)->stop();
    };
    
    // For this element.
    // Для данного элемента.
    
    AbstractAgent::stop();
    
    _free_after_execution();
    
    qDebug() << "Process::stop() end";
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                               Prepare for execution.                                             *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                              Подготовка к выполнению.                                            *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::Process::_prepare_for_execution() {
        
    AbstractAgent::_prepare_for_execution();
    
    QList<AbstractAgent * > chi = children();
    
    for ( int i=0; i<chi.count(); i++ ) {
        AbstractAgent * c = chi.at(i);
        QObject::connect( c, SIGNAL( signalFocused( bool ) ), this, SLOT( __on__agent__focused( bool ) ) );
        QObject::connect( c, SIGNAL( signalActivated( bool ) ), this, SLOT( __on__agent__activated( bool ) ) );
        QObject::connect( c, SIGNAL( signalFinished() ), this, SLOT( __on__agent__finished() ) );
        QObject::connect( c, SIGNAL( signalFailed( QString ) ), this, SLOT( __on__agent__failed( QString ) ) );
    };        
        
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                         Stop execution of this process.                                          *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                      Остановка выполнения данного процесса.                                      *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::Process::_free_after_execution() {
    
    AbstractAgent::_free_after_execution();
    
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
// *          Input focus for this process is empty. I.e. he does not have any previous by focus agents.              *
// * ---------------------------------------------------------------------------------------------------------------- *
// *       Входной фокус данного процесса - пустой. То есть у него нет ни одного агента, предыдущего по фокусу.       *
// *                                                                                                                  *
// ********************************************************************************************************************

bool tengu::Process::emptyInputFocus() {
    return ( _previousByFocus.isEmpty() );
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
        
        // The unsuccessful shutdown of at least one agent results in stopping the process as a whole
        // Неудачное завершение хотя бы одного агента приводит к остановке процесса в целом.
        
        stop();        
        emit signalFailed( errorMessage );
        
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
    
    if ( __qmlEngine ) {
        __qmlEngine->collectGarbage();
        delete( __qmlEngine );
        __qmlEngine = nullptr;
    };
    
}
