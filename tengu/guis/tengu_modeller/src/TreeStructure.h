// ********************************************************************************************************************
// *                                                                                                                  *
// *                              Tree-like representation of the structure of the workspace                          *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                              Древовидное представление структуры рабочего пространства.                          *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 24 jun 2017 at 14:55 *
// ********************************************************************************************************************

#pragma once

#include <QTreeView>
#include <QTreeWidget>
#include <QHeaderView>
#include <QTreeWidgetItem>
#include <QContextMenuEvent>
#include <QDebug>
#include <QMenu>
#include <QAction>

#include "WorkSpace.h"

namespace tengu {
    
    class TreeStructure : public QTreeWidget {
        
        Q_OBJECT
        
        public:
            
            TreeStructure ( WorkSpace * ws );
            virtual ~TreeStructure();
            
        protected:
            
            void contextMenuEvent( QContextMenuEvent * event );
            
        private:
            
            WorkSpace * __workSpace;
            QTreeWidgetItem * __rootItem;
            
            QAction * __action__create__vehicle;
            
            void __create_actions();
            
        private slots:
            
            void __on__create__vehicle();
    };
    
};

