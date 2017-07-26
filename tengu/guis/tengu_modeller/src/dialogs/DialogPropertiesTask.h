// ********************************************************************************************************************
// *                                                                                                                  *
// *                                             Dialog with the task's properties.                                   *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                 Диалог со свойствами задачи.                                     *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 23 jun 2017 at 14:57 *
// ********************************************************************************************************************

#pragma once

#include <QTabWidget>

#include "Task.h"
#include "TaskItem.h"
#include "DialogProperties.h"
#include "DialogPropertiesTask_TabAlgorythm.h"
#include "DialogPropertiesTask_TabStartCondition.h"
#include "DialogPropertiesTask_TabStopCondition.h"
#include "DialogPropertiesTask_TabQML.h"

namespace tengu {
    
    class DialogPropertiesTask : public DialogProperties {
        
        Q_OBJECT
        
        public:
            
            DialogPropertiesTask ( WorkSpace* workSpace );
            virtual ~DialogPropertiesTask();
            
            virtual void fillFrom( tengu::AbstractEntityItem * item ) override;
            
        protected:
            
            virtual void _on__cancel() override;
            virtual void _on__ok() override;
            
        private:
            
            QTabWidget * __tab;
            DialogPropertiesTask_TabAlgorythm * __tab__algorythm;
            DialogPropertiesTask_TabStartCondition * __tab__start_condition;
            DialogPropertiesTask_TabStopCondition * __tab__stop_condition;
            DialogPropertiesTask_TabQML * __tab_qml;
            
            Task * __task;
            
    };
    
};

