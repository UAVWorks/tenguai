// ********************************************************************************************************************
// *                                                                                                                  *
// *      The top level for modelling of a system: a set of agents and it's states for graphics representation.       *
// * ---------------------------------------------------------------------------------------------------------------- *
// *    Верхний уровень моделирования системы: множество агентов и их состояний для графического представления.       *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 02 jun 2017 at 16:47 *
// ********************************************************************************************************************

#pragma once

#include <QList>

#include "AbstractAgent.h"
#include "AgentFactory.h"

namespace tengu {

    class WorkSpace : public AbstractAgent {
    
        Q_OBJECT
        public:
            
            // Graphics item types. To avoid checking every time through dynamic_cast only
            
            enum graphics_item_types_t {
                GI_WorkSpace
            };                        
            
            WorkSpace();
            virtual ~WorkSpace();
                        
        private:
                        

    }; // class WorkSpace
    
}; // namespace tengu

