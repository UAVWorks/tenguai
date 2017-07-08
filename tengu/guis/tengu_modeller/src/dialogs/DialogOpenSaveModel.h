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

#include "DialogWithButtons.h"
#include "MongoStorage.h"

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
            
            DialogOpenSaveModel( MongoStorage * mongo );
            virtual ~DialogOpenSaveModel();
            
        protected:
            
            void showEvent( QShowEvent * event );
            
        private:
                        
            QComboBox * __combo_box__type_of_elements;
            QTableWidget * __table_of_elements;
            QLineEdit * __searcher;
            
            MongoStorage * __mongo;
            
            QFrame * __createElementsList();
            QFrame * __createStorageSelector();
            QWidget * __createStorageLabel( QPixmap pixmap, QString text );
            
            void __fill_table_of_elements();
    };
    
};

