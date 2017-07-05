// ********************************************************************************************************************
// *                                                                                                                  *
// *                           The stop process point. They can be several within on process.                         *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                   Точка останова процесса. Их может быть несколько в пределах одного процесса.                   *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 09 jun 2017 at 10:44 *
// ********************************************************************************************************************

#pragma once

#include "AbstractAgentKernel.h"

namespace tengu {

    class ProcessStop : public AbstractAgentKernel {
        
        Q_OBJECT
        
        public:
            
            ProcessStop ();
            virtual ~ProcessStop();
            
            // virtual QJsonObject toJSON();
            // virtual bool fromJSON( QJsonObject json );
            
    };
    
};

