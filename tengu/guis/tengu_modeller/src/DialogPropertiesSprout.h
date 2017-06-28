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
#include <QLCDNumber>

#include "DialogProperties.h"
#include "SproutItem.h"

namespace tengu {
    
    class DialogPropertiesSprout : public DialogProperties {
        
        Q_OBJECT
        
        public:
            
            DialogPropertiesSprout ( WorkSpace* workSpace );
            virtual ~DialogPropertiesSprout();
            
            void fillFrom( tengu::AbstractEntityItem * item );
                        
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
            
            // QWidget * __input_topic;
            // QWidget * __output_topic;
            
            QWidget * __slider_widget;
            QWidget * __lcd_widget;
            QLCDNumber * __lcd;
            QSlider * __output_slider;
            QLabel * __output_current_value_label;
            QLineEdit * __editor_minimum;
            QLineEdit * __editor_maximum;
            
            QWidget * __minimum_editor_widget;
            QWidget * __maximum_editor_widget;
            
            void __init_table( QTableWidget * table );
            void __create_editor_widgets();
            bool __do_not_handle_events;                        
            
            void __setManualSelection( bool manual );
            void __setAsTypeInput( bool isInput );
            
            void __fill_processes_list();
            void __fill_tasks_list();
            void __fill_sprouts_list();
            
            
        private slots:
            
            void __on__manual_signal_selection_state_changed( int state );
            void __on__minimum_editor_text_changed( const QString & text );
            void __on__maximum_editor_text_changed( const QString & text );
            void __on__combo_box_type_activated( int index );
            
    };
};
