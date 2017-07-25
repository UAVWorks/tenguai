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
            
            virtual QJsonObject toJSON() override;
            virtual bool fromJSON(QJsonObject json) override;
            
            QString startCondition();
            void setStartCondition( QString start );
            
            QString stopCondition();
            void setStopCondition( QString stop );
            
            QString algorythm();
            void setAlgorythm( QString alg );
            
        protected:
        private:
            
            QString __start_condition;
            QString __algorythm;
            QString __stop_condition;
    };
    
};

