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
// *                                                   The destructor.                                                *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                      Деструктор.                                                 *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::Process::~Process() {
    
}
