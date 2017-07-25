// ********************************************************************************************************************
// *                                                                                                                  *
// *                                 The tab with condition for starting of the task.                                 *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                   Вкладка табулятора с условием старта задачи.                                   *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 25 jul 2017 at 12:10 *
// ********************************************************************************************************************

#pragma once

#include <QFrame>
#include <QLayout>
#include <QTextEdit>

namespace tengu {

    class DialogPropertiesTask_TabStartCondition : public QFrame {
        
        Q_OBJECT
        
        public:
            
            DialogPropertiesTask_TabStartCondition();
            virtual ~DialogPropertiesTask_TabStartCondition();
            QTextEdit * start_condition;
            
    };
};

