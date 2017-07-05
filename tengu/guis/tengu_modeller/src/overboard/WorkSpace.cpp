// ********************************************************************************************************************
// *                                                                                                                  *
// *      The top level for modelling of a system: a set of agents and it's states for graphics representation.       *
// * ---------------------------------------------------------------------------------------------------------------- *
// *    Верхний уровень моделирования системы: множество агентов и их состояний для графического представления.       *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 02 jun 2017 at 16:47 *
// ********************************************************************************************************************

#include "WorkSpace.h"

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                    The constructor.                                              *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                      Конструктор.                                                *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::WorkSpace::WorkSpace()
    : AbstractAgentKernel()
{
    setSystemName( tr("Workspace") );
    setHumanName( tr("Workspace") );
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                   Overrided function for add a child into workspace                              *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                            Перекрытая функция для добавления ребенка в рабочее пространство.                     *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::WorkSpace::addChild ( tengu::AbstractAgentKernel* child ) {
    
    tengu::AbstractAgentKernel::addChild ( child );
    
    QList<AbstractAgentKernel * > hisChildren = children();
    
    Vehicle * vehicle = dynamic_cast<Vehicle * > ( child );
    if ( vehicle ) {
        
        // Automatic naming for vehicles.
        // Автоматическое именование "самоходок".
        
        int vehiclesCount = 0;
        for ( int i=0; i<hisChildren.count(); i++ ) {
            AbstractAgentKernel * och = hisChildren.at(i);
            Vehicle * vch = dynamic_cast< Vehicle * > ( och );
            if ( vch ) vehiclesCount ++;
        };
        
        if ( child->getSystemName().isEmpty() ) child->setSystemName( tr("Vehicle_") + QString::number( vehiclesCount ) );
        if ( child->getHumanName().isEmpty() ) child->setHumanName( tr("Vehicle ") + QString::number( vehiclesCount ) ) ;
    };
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                    The destructor.                                               *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                       Деструктор.                                                *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::WorkSpace::~WorkSpace() {
}
