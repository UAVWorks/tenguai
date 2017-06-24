// ********************************************************************************************************************
// *                                                                                                                  *
// *                   Entity entering the workspace (an aiircraft, an airport. an vehicle e.t.c. )                   *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                   Сущность, входящая в рабочее пространство (самолет, аэропорт, машинка и др.)                   *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 24 jun 2017 at 16:07 *
// ********************************************************************************************************************

#pragma once

#include "Processor.h"

namespace tengu {
    
    class TopLevelEntity : public Processor {
        
        Q_OBJECT
        
        public:
            
            TopLevelEntity();
            virtual ~TopLevelEntity();
        protected:
        private:
    };
};
