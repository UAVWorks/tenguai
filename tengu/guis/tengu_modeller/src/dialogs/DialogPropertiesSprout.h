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
#include <QItemSelection>

#include "DialogProperties.h"
#include "SproutItem.h"
#include "SproutProxy.h"

#define LCD_NONE "0.000"

namespace tengu {
    
    class DialogPropertiesSprout : public DialogProperties {
        
        Q_OBJECT
        
        public:
            
            DialogPropertiesSprout ( WorkSpace* workSpace );
            virtual ~DialogPropertiesSprout();
            
            void fillFrom( tengu::AbstractEntityItem * item );
                        
        protected:
            
            void _on__cancel();
            void showEvent( QShowEvent * event );
            
        private:
            
            SproutItem * __oldSproutItem;
            
            SproutItem * __sproutItem;    
            QComboBox * __combo_box__type;
            QComboBox * __combo_box__execution_mode;
            QCheckBox * __check_box__manual;
            QLineEdit * __manual_signal_name;
            
            QLineEdit * __filter_processors;
            QLineEdit * __filter_processes;
            QLineEdit * __filter_tasks;
            QLineEdit * __filter_sprouts;
            
            QTableWidget * __table_processors;
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
            
            void __storeOldSproutParams();
            void __restoreOldSproutParams();
            
            void __init_table( QTableWidget * table );
            void __create_editor_widgets();
            bool __do_not_handle_events;                        
            
            void __setManualSelection( bool manual );
            void __setAsTypeInput( bool isInput );
            
            void __fill_processors_list();
            void __fill_processes_list();
            void __fill_tasks_list();
            void __fill_sprouts_list();
                        
            /**
             * @short Fill any table (processes, tasks, sprouts) from parent's element according filter.
             * Заполнить какую-нибудь таблицу (процессы, задачи, отростки) из родительского элемента с учетом фильтра.
             */
            
            template < class T > void __fill_one_table( AbstractAgent * agent, QTableWidget * table, QLineEdit * filter ) {
                QList< T > list = agent->getEntities< T >( filter->text() );
                for ( int i=0; i<list.count(); i++ ) {
                    T p = list.at(i);
                    table->setRowCount(i+1);
                    QTableWidgetItem * item = new QTableWidgetItem();
                    item->setFlags(item->flags() ^ Qt::ItemIsEditable);
                    item->setData( Qt::UserRole, QVariant::fromValue< T >( p ) );
                    item->setText( p->getHumanName() );
                    table->setItem( i, 0, item );                      
                };    
            };
            
            // Get selected in the table agent of specified type
            // Получить выбранного в таблице агента указанного типа.
            
            template < class T > T __selectedAgent( QTableWidget * table ) {
                
                T agent = 0;
                
                if ( table->selectedItems().count() > 0 ) {
                    QTableWidgetItem * selectedItem = table->selectedItems().at(0);
                    if ( selectedItem ) {
                        agent = qvariant_cast< T >( selectedItem->data( Qt::UserRole ) );
                    };
                };
                
                return agent;
                
            };
            
            // Processor * __selectedProcessor();
            // Process * __selectedProcess();
            // Task * __selectedTask();
            // Sprout * __selectedSprout();
            
            void __stopListen();
            
            // Set slider min/max/pos according to sprout's values.
            // Установить min/max/текущую позицию согласно значениям sprout'а.
            
            void __setSlider();
            
            void __showMinMaxValues();
                        
        private slots:
            
            void __on__manual_signal_selection_state_changed( int state );
            void __on__filter_processors_text_changed( const QString & text );
            void __on__filter_processes_text_changed( const QString & text );
            void __on__filter_tasks_text_changed( const QString & text );
            void __on__filter_sprouts_text_changed( const QString & text );
            void __on__minimum_editor_text_changed( const QString & text );
            void __on__maximum_editor_text_changed( const QString & text );
            void __on__table_processors_item_selected( const QItemSelection & selected, const QItemSelection & deselected );
            void __on__table_processes_item_selected( const QItemSelection & selected, const QItemSelection & deselected );
            void __on__table_tasks_item_selected( const QItemSelection & selected, const QItemSelection & deselected );
            void __on__table_sprouts_item_selected( const QItemSelection & selected, const QItemSelection & deselected );
            void __on__combo_box_type_activated( int index );
            void __on__combo_box__execution_mode__activated( int index );
            void __on__got_value( QVariant value );
            void __on__slider_value_changed( int value );
            
    };
};
