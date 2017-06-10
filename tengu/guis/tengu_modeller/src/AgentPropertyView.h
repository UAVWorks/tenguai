// ********************************************************************************************************************
// *                                                                                                                  *
// *                                        The MVC-based view for agent property.                                    *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                   Основанное на MVC представление свойств агента.                                *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 01 jun 2017 at 10:16 *
// ********************************************************************************************************************

#pragma once

#include <QTableView>
#include <QHeaderView>

namespace tengu {

    class AgentPropertyView : public QTableView {
        
        Q_OBJECT
        
        public:
            
            AgentPropertyView( QWidget * parent = nullptr );
            ~AgentPropertyView();
    };
};

