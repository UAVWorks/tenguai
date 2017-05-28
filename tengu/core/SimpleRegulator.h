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

#include "PrefixedAgent.h"
#include "Constants.h"

// All of regulators are on board. There is nothing to regulate on the ground.
// Все регуляторы - они на борту. Нечего регулировать на земле-то.

#include "../onboard/onboard_common.h"

namespace tengu {
    
    class SimpleRegulator : public PrefixedAgent {
        
        Q_OBJECT
        
        public:
            
            SimpleRegulator( QString name, float min_value = MINIMUM_CONSTRAINT, float max_value = MAXIMUM_CONSTRAINT );
            virtual ~SimpleRegulator();
            
            virtual bool usable();
            
        protected:
            
            // PID * _pid;            
            
            QString _input_channel;
            float _input_value;
            
            QString _desired_channel;
            float _desired_value;
            
            QString _output_channel;
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
            
            // void __subscribe();
            void __on_input_received( QString channel, QString message );
            void __on_desired_received( QString channel, QString message );
                        
        private slots:                                              
            
            
    };
};
