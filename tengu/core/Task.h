// ********************************************************************************************************************
// *                                                                                                                  *
// *                       The task. The agent which can calculate something using QJSEngine.                         *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                    Задача. Агент, который может что-нибудь вычислять, используя QJSEngine.                       *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 09 jun 2017 at 09:38 *
// ********************************************************************************************************************

#pragma once

#include "SproutableAgent.h"

namespace tengu {
    
    class Task : public SproutableAgent {
        Q_OBJECT
        public:
            
            Task();
            virtual ~Task();
            
            virtual QJsonObject toJSON();
            
        protected:
        private:
    };
    
};

