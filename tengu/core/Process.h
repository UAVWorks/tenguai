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
#include "ProcessStart.h"
#include "ProcessStop.h"
#include "ANDor.h"
#include "ORer.h"

namespace tengu {
    
    class Process : public AbstractAgent {
        
        Q_OBJECT
                
        public:
            
            Process ();
            // Process( const Process & o );
            virtual ~Process();
            virtual QJsonObject toJSON();
            
            virtual void addChild( AbstractAgent * child );
            
            virtual void start();
            
            bool emptyInputFocus();
                    
        protected:
            
            virtual void _prepare_for_execution();
            virtual void _free_after_execution();
            
        private:
                        
            
        private slots:
            
            void __on__agent__focused( bool focus );
            void __on__agent__activated( bool activity );
            void __on__agent__finished();
            void __on__agent__failed( QString errorMessage );
            
    };
    
};

// Q_DECLARE_METATYPE( tengu::Process );


