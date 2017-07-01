// ********************************************************************************************************************
// *                                                                                                                  *
// *                               Common ancestor for AbstractAgentItem properties dialogs.                          *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                   Общий предок диалогов свойств AbstractAgentItem                                *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 23 jun 2017 at 13:48 *
// ********************************************************************************************************************

#pragma once

#include <QDialog>
#include <QIcon>
#include <QPixmap>
#include <QLayout>
#include <QPushButton>
#include <QFrame>
#include <QHeaderView>
#include <QTableWidget>

#include "WorkSpace.h"
#include "AbstractEntityItem.h"

namespace tengu {
    
    class DialogProperties : public QDialog {
        
        Q_OBJECT
        
        public:
            
            DialogProperties ( WorkSpace * workSpace );
            virtual ~DialogProperties();
            
            virtual void fillFrom( tengu::AbstractEntityItem * item );
            
        protected:
            
            WorkSpace * _workSpace;
            
            QFrame * _centerWidget;
            QPushButton * _buttonOk;
            QPushButton * _buttonCancel;
            
            /**
             * Virtual function to cancel operation.
             * Виртуальная функция для операций при отмене.
             */
            
            virtual void _on__cancel();
            
            /**
             * @short Clear table contents and set it's row count to 0.
             * Очистить содержимое таблицы и установить число строк в 0.
             */
            
            void _clearTable( QTableWidget * table );
            
        private:
            
        private slots:
            
            void __on__ok_pressed();
            void __on__cancel_pressed();
    };
};

