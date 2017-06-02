// ********************************************************************************************************************
// *                                                                                                                  *
// *      The top level for modelling of a system: a set of agents and it's states for graphics representation.       *
// * ---------------------------------------------------------------------------------------------------------------- *
// *    Верхний уровень моделирования системы: множество агентов и их состояний для графического представления.       *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 02 jun 2017 at 16:47 *
// ********************************************************************************************************************

#pragma once

#include "AbstractAgent.h"

namespace tengu {

    class GUIModel : public AbstractAgent {
    
        Q_OBJECT
        public:
            GUIModel( AbstractAgent * parent, QString systemName );
            virtual ~GUIModel();
    };
    
}; // namespace tengu

