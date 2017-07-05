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

#include "AbstractAgentKernel.h"
#include "AgentFactory.h"

namespace tengu {

    class WorkSpace : public AbstractAgentKernel {
    
        Q_OBJECT
        public:
            
            // Graphics item types. To avoid checking every time through dynamic_cast only
            
            enum graphics_item_types_t {
                GI_Sprout,
                GI_Task,
                GI_Process,
                GI_Vehicle,
                GI_WorkSpace                
            };                        
            
            WorkSpace();
            virtual ~WorkSpace();
            
            void addChild( AbstractAgentKernel * child );
                        
        private:
                        

    }; // class WorkSpace
    
}; // namespace tengu

