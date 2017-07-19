// ********************************************************************************************************************
// *                                                                                                                  *
// *          The process. A container for tasks. It have a start point and stop('s) point(-s), it execute            *
// *                                                 by focus transition.                                             *
// * ---------------------------------------------------------------------------------------------------------------- *
// *         Процесс (контейнер для задач). Имеет точку старта и одну или несколько точек останова. Выполняется       *
// *                                                путем перехода фокуса.                                            *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 09 jun 2017 at 10:32 *
// ********************************************************************************************************************

#pragma once

#include "AbstractAgent.h"
#include "Task.h"
#include "ProcessStop.h"

namespace tengu {
    
    class Process : public AbstractAgent {
        
        Q_OBJECT
        
        public:
            
            Process ();
            // Process( const Process & o );
            virtual ~Process();
            virtual QJsonObject toJSON();
            
            void addChild( AbstractAgent * child );
            
    };
    
};

// Q_DECLARE_METATYPE( tengu::Process );


