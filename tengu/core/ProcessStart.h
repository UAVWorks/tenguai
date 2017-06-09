// ********************************************************************************************************************
// *                                                                                                                  *
// *                       The start point for process. There should be only one within the process.                  *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                          Точка старта процесса. Должна быть только одна в пределах процесса.                     *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 09 jun 2017 at 10:38 *
// ********************************************************************************************************************

#pragma once 

#include "AbstractAgent.h"

namespace tengu {
    
    class ProcessStart : public AbstractAgent {
        
        Q_OBJECT
        
        public:
            
            ProcessStart ( QString systemName );
            virtual ~ProcessStart();
            
    };
    
};

