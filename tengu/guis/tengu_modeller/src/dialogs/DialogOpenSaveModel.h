// ********************************************************************************************************************
// *                                                                                                                  *
// *                                         Dialog to work with model's storage (MongoDB).                           *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                       Диалог для работы с хранилищем моделей (MongoDB)                           *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 07 jul 2017 at 05:56 *
// ********************************************************************************************************************

#pragma once

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QFrame>
#include <QComboBox>
#include <QTableWidget>
#include <QHeaderView>
#include <QJsonObject>

#include "DialogWithButtons.h"
#include "MongoStorage.h"
#include "AgentItemFactory.h"

namespace tengu {
    
    class DialogOpenSaveModel : public DialogWithButtons {
        
        Q_OBJECT
        
        public:
            
            // Type of elements to show in dialogs.
            // Тип элементов для отображения в диалоге.
            
            enum selector_t {
                
                OSM_VEHICLE,
                OSM_PROCESS,
                OSM_TASK
            };
            
            enum dialog_mode_t {
                DM_OPEN,
                DM_SAVE
            };
            
            DialogOpenSaveModel( MongoStorage * mongo );
            virtual ~DialogOpenSaveModel();
            
            AbstractAgent * result_agent;
            
            void setCurrentMode( dialog_mode_t mode );
            
        protected:
            
            void showEvent( QShowEvent * event );
            void _on__ok();            
            
        private:
                        
            dialog_mode_t __current_mode;
            QComboBox * __combo_box__type_of_elements;
            QTableWidget * __table_of_elements;
            QLineEdit * __searcher;
            
            MongoStorage * __mongo;
            QJsonObject __selector;
            
            QFrame * __createElementsList();
            QFrame * __createStorageSelector();
            QWidget * __createStorageLabel( QPixmap pixmap, QString text );
            
            void __fill_table_of_elements();                        
        
        private slots:
            
            void __on__type_of_elements_changed( int type );
            void __on__table_element_selected( QItemSelection current, QItemSelection previous );
    };
    
};

