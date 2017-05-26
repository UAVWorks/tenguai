// ********************************************************************************************************************
// *                                                                                                                  *
// *                                             Abstract reguator class.                                             *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                          Класс абстрактного регулятора.                                          *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 24 may 2017 at 10:53 *
// ********************************************************************************************************************

#include <QObject>
#include <QTimer>
#include <QString>
#include <QTime>
#include <QDebug>
#include <QString>
#include <QSettings>

#include "AbstractAgent.h"

namespace tengu {
    
    class AbstractRegulator : public AbstractAgent {
        
        Q_OBJECT
        
        public:
            
            AbstractRegulator( QString section, float min_value, float max_value );
            virtual ~AbstractRegulator();
            
            static QString prefix( QString section );
            
        protected:
            
                        
            QString _section;
            QString _message_prefix;
            
            // PID * _pid;            
            
            QString _input_channel;
            float _input_value;
            QString _desired_channel;
            float _desired_value;
            QString _output_channel;
                                              
        private:
            
            
            bool __active;
            
            void __subscribe();
            
            void __do_step();
                        
        private slots:                                              
            
            
    };
};
