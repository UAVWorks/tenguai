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
            
        private:
            
        private slots:
            
            void __on__ok_pressed();
            void __on__cancel_pressed();
    };
};

