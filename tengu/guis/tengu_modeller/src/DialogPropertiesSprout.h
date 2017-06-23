// ********************************************************************************************************************
// *                                                                                                                  *
// *                                        The dialog with sprout's properties.                                      *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                           Диалог со свойствами "отростка".                                       *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 23 jun 2017 at 13:43 *
// ********************************************************************************************************************

#pragma once

#include <QWidget>
#include <QSlider>
#include <QComboBox>
#include <QLabel>
#include <QCheckBox>
#include <QLineEdit>
#include <QTableWidget>

#include "DialogProperties.h"
#include "SproutItem.h"

namespace tengu {
    
    class DialogPropertiesSprout : public DialogProperties {
        
        Q_OBJECT
        
        public:
            
            DialogPropertiesSprout ( WorkSpace* workSpace );
            virtual ~DialogPropertiesSprout();
            
            void fillFrom( tengu::AbstractEntityItem * item );
            
            void setManualSelection( bool manual );
            
        protected:
        private:
            
            SproutItem * __sprout;    
            QComboBox * __combo_box__type;
            QCheckBox * __check_box__manual;
            QLineEdit * __manual_signal_name;
            
            QLineEdit * __filter_process;
            QLineEdit * __filter_task;
            QLineEdit * __filter_sprouts;
            
            QTableWidget * __table_processes;
            QTableWidget * __table_tasks;
            QTableWidget * __table_sprouts;
            
            void __init_table( QTableWidget * table );
            
        private slots:
            
            void __on__manual_signal_selection_state_changed( int state );
    };
};
