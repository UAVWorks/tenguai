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
#include "AgentFactory.h"

namespace tengu {
    
    class TreeStructure : public QTreeWidget {
        
        Q_OBJECT
        
        signals:
            
            void signalAgentCreated( AbstractAgent * agent );
            void signalAgentSelected( AbstractAgent * agent );
        
        public:
            
            TreeStructure ( WorkSpace * ws );
            virtual ~TreeStructure();
            
        protected:
            
            void contextMenuEvent( QContextMenuEvent * event );
            
        private:
            
            WorkSpace * __workSpace;
            QTreeWidgetItem * __rootItem;
            QTreeWidgetItem * __selectedItem;
            AbstractAgent * __selectedAgent;
            
            bool __do_not_handle_events;
            
            QAction * __action__create__vehicle;
            QAction * __action__create__process;
            
            void __select_forcibly_with_expanding( QTreeWidgetItem * parent, QTreeWidgetItem * selected );
            
            void __create_actions();
            
        private slots:
            
            void __on__create__vehicle();
            void __on__create__process();
            
            void __on__tree_item_selected( const QItemSelection & current, const QItemSelection & previous );
            
    };
    
};

