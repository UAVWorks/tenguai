// ********************************************************************************************************************
// *                                                                                                                  *
// *                                              Simple reguator class.                                              *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                            Класс простого регулятора.                                            *
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
#include "Constants.h"

// All of regulators are on board. There is nothing to regulate on the ground.
// Все регуляторы - они на борту. Нечего регулировать на земле-то.

#include "../onboard/onboard_common.h"

namespace tengu {
    
    class SimpleRegulator : public AbstractAgent {
        
        Q_OBJECT
        
        public:
            
            
            SimpleRegulator( AbstractAgentKernel * parent, QString name, float min_value = MINIMUM_CONSTRAINT, float max_value = MAXIMUM_CONSTRAINT );
            virtual ~SimpleRegulator();
            
            void setInputChannel( QString channel );
            void setDesiredChannel( QString channel );
            void setOutputChannel( QString channel );            
            
            virtual bool usable();
            
        protected:
            
            // PID * _pid;            
            
            QString _inputChannel;
            float _input_value;
            
            QString _desiredChannel;
            float _desired_value;
            
            QString _outputChannel;
            float _output_value;
                        
            float _P;
            float _I;
            float _D;
            
            float _min_value;
            float _max_value;
            
            // One step of regulation
            // Один шаг регулирования.
            
            void _do_step();
            
        private:
                        
        private slots:                                              
            
            void __on_input_received( QVariant value );
            void __on_desired_received( QVariant value );
            
            
    };
};
