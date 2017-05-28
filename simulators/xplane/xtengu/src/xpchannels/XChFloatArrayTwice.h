// ********************************************************************************************************************
// *                                                                                                                  *
// *               Some variables are defined through two arrays at the same time (simultaneously).                   *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                       Некоторые переменные определяются через два массива одновременно.                          *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 10 may 2017 at 12:53 *
// ********************************************************************************************************************

#pragma once

#include <QList>
#include "XChFloatArray.h"

namespace tengu {
    
    class XChFloatArrayTwice : public XChFloatArray {
        
        public:
            
            XChFloatArrayTwice ( QString aName, QString path, int aMinValue = MINIMUM_CONSTRAINT, int aMaxValue = MAXIMUM_CONSTRAINT );
            virtual ~XChFloatArrayTwice();
            
            virtual void set( int value );
            virtual void set( float value );
            
            virtual bool usable();
        
        protected:
            
            XPLMDataRef _dataref_1;
            XPLMDataRef _dataref_2;
            
            QList<int> _template_1;
            QList<int> _template_2;
    };
};
