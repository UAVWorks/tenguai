// ********************************************************************************************************************
// *                                                                                                                  *
// *                                       Tab widget containing task algorythm.                                      *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                  Вкладка табулятора, содержащая алгоритм задачи.                                 *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 25 jul 2017 at 12:05 *
// ********************************************************************************************************************

#pragma once

#include <QWidget>
#include <QFrame>
#include <QTextEdit>
#include <QLayout>

namespace tengu {

    class DialogPropertiesTask_TabAlgorythm : public QFrame {
    
        Q_OBJECT
        
        public:
            
            DialogPropertiesTask_TabAlgorythm();
            virtual ~DialogPropertiesTask_TabAlgorythm();
            
            QTextEdit * algorythm;
            
    };
    
};
