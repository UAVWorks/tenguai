// ********************************************************************************************************************
// *                                                                                                                  *
// *                                 The channel for aircraft relocation using one command.                           *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                             Канал для перемещения самолета с использованием одной команды.                       *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 25 may 2017 at 18:40 *
// ********************************************************************************************************************

#pragma once

#include <QString>
#include <QStringList>

#include "XPLMGraphics.h"
#include "XPlaneChannel.h"

namespace tengu {
    
    class XChRelocate : public XPlaneChannel {
        
        public:
            
            XChRelocate( int index );
            virtual ~XChRelocate();
            
            virtual bool usable();
            virtual void set( QString message );
            
        private:
            
            XPLMDataRef __x_dataref;
            XPLMDataRef __y_dataref;
            XPLMDataRef __z_dataref;
            XPLMDataRef __heading_dataref;
            
    };
    
}; // namespace

