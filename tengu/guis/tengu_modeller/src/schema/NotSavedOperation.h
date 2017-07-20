// ********************************************************************************************************************
// *                                                                                                                  *
// *                            The operation which has beed performed but still not saved.                           *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                             Операция, которая была выполнена, но все еще не сохранена.                           *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 20 jul 2017 at 11:27 *
// ********************************************************************************************************************

#pragma once

#include <QJsonObject>

namespace tengu {
    
    class NotSavedOperation {
        
        public:
            
            enum operations_t {
                OT_UNKNOWN,
                OT_DELETE
            };
            
            NotSavedOperation();
            NotSavedOperation( operations_t op, QJsonObject obj );
            ~NotSavedOperation();
            
            operations_t operation;
            QJsonObject object;
    };
    
};

