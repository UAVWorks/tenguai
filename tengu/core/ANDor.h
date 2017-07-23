// ********************************************************************************************************************
// *                                                                                                                  *
// *                The agent which will be waiting the availability of all his inputs after activity.                *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                    Агент, который будет ждать наличия всех входов после получения активности.                    *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 09 jun 2017 at 10:02 *
// ********************************************************************************************************************

#pragma once

#include "AbstractAgent.h"

namespace tengu {

    class ANDor : public AbstractAgent {
        
        Q_OBJECT
        
        public:
            
            ANDor ();
            virtual ~ANDor();
            
        protected:
            
            virtual void _step();
            virtual void _prepare_for_execution();
            virtual void _free_after_execution();
        
        private:
            
            // List previous by focus agent which was still not finished.
            // Список предыдущих по фокусу агентов, которые еще не закончили выполнение.
            
            QList<QString> __previous_agents_unfinished;
            
            // List next by focus agent which are not started (not activated)
            // Список следующих по фокусу агентов, которые еще не запустились (не были активизированы).
            
            QList<QString> __next_agents_unstarted;
    
        private slots:
            
            void __on__agent__activated( bool activity );
            
    };
    
};


