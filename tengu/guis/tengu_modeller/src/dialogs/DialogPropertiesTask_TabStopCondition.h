// ********************************************************************************************************************
// *                                                                                                                  *
// *                                   The tab with condition for stop of the task.                                   *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                              Вкладка табулятора, содержащая условие остановки задачи.                            * 
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 25 jul 2017 at 12:17 *
// ********************************************************************************************************************

#pragma once

#include <QFrame>
#include <QLayout>
#include <QTextEdit>

namespace tengu {

    class DialogPropertiesTask_TabStopCondition : public QFrame {
        
        Q_OBJECT
        
        public:
        
            DialogPropertiesTask_TabStopCondition();
            virtual ~DialogPropertiesTask_TabStopCondition();
            
            QTextEdit * stop_condition;
    };
    
};

