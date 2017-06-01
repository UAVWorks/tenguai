// ********************************************************************************************************************
// *                                                                                                                  *
// *                                          Table MVC-model for agent's properties.                                 *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                          Табличная MVC-модель для свойств агента.                                *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 01 jun 2017 at 11:32 *
// ********************************************************************************************************************

#pragma once

#include <QObject>
#include <QAbstractItemModel>

namespace tengu {

    class AgentPropertyModel : public QAbstractItemModel {
        
        Q_OBJECT
        
        public:
            
            AgentPropertyModel( QObject * parent = nullptr );
            ~AgentPropertyModel();
            
    };
};

