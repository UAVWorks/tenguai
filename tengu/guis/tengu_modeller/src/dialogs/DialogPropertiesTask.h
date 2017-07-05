// ********************************************************************************************************************
// *                                                                                                                  *
// *                                             Dialog with the task's properties.                                   *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                 Диалог со свойствами задачи.                                     *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 23 jun 2017 at 14:57 *
// ********************************************************************************************************************

#pragma once

#include "DialogProperties.h"

namespace tengu {
    
    class DialogPropertiesTask : public DialogProperties {
        
        Q_OBJECT
        
        public:
            
            DialogPropertiesTask ( WorkSpace* workSpace );
            virtual ~DialogPropertiesTask();
            
        protected:
        private:
    };
    
};

