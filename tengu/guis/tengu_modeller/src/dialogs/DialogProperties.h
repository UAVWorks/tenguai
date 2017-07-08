// ********************************************************************************************************************
// *                                                                                                                  *
// *                                          Schema element properties dialog.                                       *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                        Диалог со свойствами элемента схемы.                                      *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 07 jul 2017 at 06:01 *
// ********************************************************************************************************************

#pragma once

#include "DialogWithButtons.h"

#include "WorkSpace.h"
#include "AbstractEntityItem.h"

namespace tengu {
    
    class DialogProperties : public DialogWithButtons {
        
        Q_OBJECT
        
        public:
            
            DialogProperties ( WorkSpace* workSpace );
            virtual ~DialogProperties();
            
            virtual void fillFrom( tengu::AbstractEntityItem * item );
            
        protected:
            
            WorkSpace * _workSpace;
                        
        private:
            
            
            
    };
};

