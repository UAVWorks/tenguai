// ********************************************************************************************************************
// *                                                                                                                  *
// *                                   MongoC wrapper to store objects in the mongo.                                  *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                Обертка над MongoC для хранения объектов в монго.                                 *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 13 jun 2017 at 15:00 *
// ********************************************************************************************************************

#pragma once

#include <string>

#include <bson.h>
#include <bcon.h>
#include <mongoc.h>

#include <QObject>
#include <QString>
#include <QDebug>
#include <QJsonObject>
#include <QList>
#include <QJsonDocument>

#include "AbstractEntity.h"
#include "Constants.h"

namespace tengu {
    
    class MongoStorage : public QObject {
        
        Q_OBJECT
        public:
            
            MongoStorage ( QString host = "localhost", int port = 27017 );
            virtual ~MongoStorage();    
            
            void store( AbstractEntity * e );
            void store( QJsonObject o );
            bool storageable( QJsonObject o );
            bool storageable( AbstractEntity * e );
            
        protected:
        private:
            
            void __insert_single_object( QJsonObject jsonObject );
            
            mongoc_client_t * __client;
            
            bson_t * __create_bson( QJsonObject o );
            
            QList<QString> __alreadyIndexedCollections;
            
            // mongoc_database_t * __database;
            // mongoc_collection_t * __collection;
    };
    
};

