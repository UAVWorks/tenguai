// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                 The MongoDB index class.                                         *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                  Класс MongoDB индекса.                                          *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 08 jul 2017 at 12:32 *
// ********************************************************************************************************************

#pragma once

#include <QJsonObject>
#include <QJsonArray>
#include <QList>
#include <QDebug>

namespace tengu {

    class MongoIndex {
        
        public:
            
            struct one_key_t {
                QString name;
                int asc;
            };
            
            MongoIndex();
            ~MongoIndex();
            
            QJsonObject toJSON();
            void fromJSON( QJsonObject );
            QString name;
            QList<one_key_t> key;
            bool unique;
            bool background;
            
        protected:
            
        private:
            
            bool __getOneKey( QJsonObject o, struct one_key_t & k );
            
    };
    
};

