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
#include <QJsonArray>

#include "MongoIndex.h"
#include "AbstractEntity.h"
#include "Constants.h"

namespace tengu {
    
    class MongoStorage : public QObject {
        
        Q_OBJECT
        
        signals:
            
            void signalError( tengu::error_level_t errorLevel, QString procedureName, QString errorMessage );
        
        public:
            
            MongoStorage ( QString host = "localhost", int port = 27017 );
            virtual ~MongoStorage();    
            
            void store( AbstractEntity * e );
            void store( QJsonObject o );
            bool storageable( QJsonObject o );
            bool storageable( AbstractEntity * e );
            
            void checkIndexes( AbstractEntity * e );
            
            // Get some or all objects from collection.
            // Получить некоторые или все объекты из коллекции.
            
            QList<QJsonObject> read( QJsonObject selector, bool recursive );
            
        protected:
        private:
            
            
            mongoc_client_t * __client;
            QList<QString> __alreadyIndexedCollections;
            
            bson_t * __create_bson( QJsonObject o );
            
            // void __insert_single_object( QJsonObject jsonObject );
            
            bool __upsert_single_object( QJsonObject json );
            
            mongoc_collection_t * __getCollection( QJsonObject o );
            QMap<QString, tengu::MongoIndex > __getExistingIndexes( QJsonObject o );
            
            // Add index to the mongo collection
            // Добавить индекс в коллекцию монги.
            
            void __addIndex( QJsonObject o, tengu::MongoIndex idx );
            
            bool __simplify( QJsonValue & val );
            
            // There were already attempts to do something with the indexes and they led to an error. 
            // Most likely, the version of MongoDB is too old. There is no point in continuing to do things with the indices.
            
            // Уже были попытки делать что-то с индексами и они привели к ошибке. Скорее всего,
            // версия монги - слишком старая. Нет смысла продолжать делать что-то с индексами.
            
            bool __indexesErrorOccured;
            
            // mongoc_database_t * __database;
            // mongoc_collection_t * __collection;
            
            bool __valueMustBeDeleted( QJsonValue val );
            
            bool __simplifyable( QJsonValue val );
            
            // Get "the reference": collection name and uuid for this object
            // Получить "ссылку": имя коллекции и uuid данного объекта.
            
            QJsonObject __getReference( QJsonValue val );
            
            bool __read_recursive( QString database, QJsonValue & val );
            
            // Add to hiuse element, for later using references between collections.
            // Добавить к элементу hiuse, для последующего использования ссылок между коллекциями.
            
            // void __hiuse( QJsonObject obj, QJsonObject ref );
    };
    
};

