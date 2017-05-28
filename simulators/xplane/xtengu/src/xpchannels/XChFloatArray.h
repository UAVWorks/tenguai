// ********************************************************************************************************************
// *                                                                                                                  *
// *                                 An array. Reading and setting for array-based X-Plane values.                    *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                        Массив. Чтение и управление каналов X-Plane, основанных на массиве данных.                *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 08 may 2017 at 16:49 *
// ********************************************************************************************************************

#pragma once

#include <QList>

#include "XPlaneChannel.h"

namespace tengu {
    
    class XChFloatArray : public XPlaneChannel {
        
        public:
            
            XChFloatArray (  QString aName, QString path, int aMinValue = MINIMUM_CONSTRAINT, int aMaxValue = MAXIMUM_CONSTRAINT  );
            virtual ~XChFloatArray();
            
            virtual void set( int value );
            virtual void set( float value );
            
        protected:
            
            QList<int> _templateArray;
    };
    
}; // namespace tengu

