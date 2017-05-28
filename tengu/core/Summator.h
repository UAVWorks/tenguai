// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                     The summator                                                 *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                       Сумматор.                                                  *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 27 may 2017 at 06:30 *
// ********************************************************************************************************************

#pragma once

#include "AbstractAgent.h"

namespace tengu {
    
    /**
     * @short The summator (Сумматор).
     * 
     * There is some number of input channels, there is some number of output channels. 
     * Internal value of summator is sum of each one input multiple his separate coefficient. 
     * Output value of every each output channel is internal value multiple on separate 
     * coefficient for this output channel. In addition, there is a coefficient of internal 
     * inertia of the Summator. Therefore, you can build an inverter, splitter, channel for 
     * redundancy, etc. 
     * 
     * Несколько каналов входа, несколько каналов выхода. Внутреннее значение сумматора
     * равно сумме входных значений, каждое из которых умножается на свой собственный
     * коэффициент. Выходное значение каждого выходного канала равно внутреннему значению
     * сумматора умножить на коэффициент для данного конкретного выхода. Дополнительно
     * введен коэффициент внутренней инерции сумматора. Таким образом, Вы можете построить
     * инвертор, разветвитель, канал для резервирования и др. 
     */
    
    class Summator : public AbstractAgent {
    
        Q_OBJECT
        
        public:
            
            struct appendage_t {
                QString channel;
                float coefficient;
            };
            
            Summator( QString name, QObject * parent = 0 );
            ~Summator();
            
            void addInputChannel( appendage_t channel );
            void addOutputChannel( appendage_t channel );
        
        private:
            
            QList<appendage_t> __input;
            QList<appendage_t> __output;
            
    };
    
};

