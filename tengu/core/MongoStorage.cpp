// ********************************************************************************************************************
// *                                                                                                                  *
// *                                   MongoC wrapper to store objects in the mongo.                                  *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                Обертка над MongoC для хранения объектов в монго.                                 *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 13 jun 2017 at 15:00 *
// ********************************************************************************************************************

#include "MongoStorage.h"

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                 The constructor.                                                 *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                   Конструктор.                                                   *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::MongoStorage::MongoStorage( QString host, int port ) 
    : QObject()
{
    mongoc_init();
    __client = nullptr;
    
    QString connectionString = QString("mongodb://") + host + ":" + QString::number( port );
    __client = mongoc_client_new ( connectionString.toLocal8Bit().data() );
    
    if ( ! __client ) {
        qDebug() << "MongoStorage::MongoStorage(): failed to parse URL for client.";
        return;
    };
    
    mongoc_client_set_error_api( __client, MONGOC_ERROR_API_VERSION_2 );
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                       Create bson_t document from json object.                                   *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                      Создание bson_t документа из JSON объекта.                                  *
// *                                                                                                                  *
// ********************************************************************************************************************

bson_t * tengu::MongoStorage::__create_bson ( QJsonObject o ) {
    
    bson_t * doc = bson_new();
    QStringList keys = o.keys();
    for ( int i=0; i<keys.length(); i++ ) {
        
        QString key = keys.at(i);        
        char ckey[255];
        memset( ckey, 0, sizeof( ckey) );
        strcpy( ckey, key.toLatin1().data() );
        int ckey_len = keys.at(i).length();
        qDebug() << "Key: " << key << ", bson key: " << ckey << ", len=" << ckey_len;
                
        if ( key == "uuid" ) bson_append_utf8( doc, "_id", -1, o.value( key ).toString().toUtf8(), -1  );
        else {
            
            QJsonValue val = o.value( key );
            if ( val.isString() ) bson_append_utf8( doc, ckey, ckey_len, val.toString().toUtf8(), -1 );
            else if ( val.isDouble() ) bson_append_double( doc, ckey, ckey_len, val.toDouble() );
            else {
                qDebug() << "MongoStorage::__create_bson: field " << key << " was not added, type=" << val.type();
            };
        };
        
    };
    
    char * str = bson_as_json ( doc , NULL );
    qDebug() << "Bson to insert: " << QString(str) ;
    bson_free( str );
        
    
    return doc;
}


// ********************************************************************************************************************
// *                                                                                                                  *
// *                             Store AbstractEntity object than can contain other objects.                          *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                        Сохранить AbstractEntity объект, который может содержать другие объекты.                  *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::MongoStorage::store ( tengu::AbstractEntity * e ) {
    
    // We can not insert a set of separate object at once.
    // Мы не можем вставлять множество разных объектов за один раз.
    
    QJsonObject o = e->toJSON();
    QStringList keys = o.keys();
    
    for ( int i=0; i<keys.size(); i++ ) {
        
        QJsonValue val = o.value( keys.at(i) );
        
        if ( val.isObject() ) {
            o.remove( keys.at(i) );
            continue;
        };
    };
    
    __insert_single_object( o );
}


// ********************************************************************************************************************
// *                                                                                                                  *
// *                                         Store one JSON object to database.                                       *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                       Сохранить один JSON-объект в базе данных.                                  *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::MongoStorage::__insert_single_object ( QJsonObject jsonObject ) {
    
    if ( ( ! jsonObject.contains("database") ) || ( ! jsonObject.contains("collection") ) || ( ! jsonObject.contains("uuid")  ) ) {
        qDebug() << "MongoStorage::store( QJsonObject ), object does not contains uuid | database | collection";
        qDebug() << jsonObject;
        return;
    };
    
    if ( ! __client ) {
        qDebug() << "MongoStorage::store( QJsonObject ), client is not initialized.";
        return;
    };
    
    // Replace UUID <-> _id
    // Заменяем UUID <-> _id
    
    // QString uuid = jsonObject.value("uuid").toString();
    // jsonObject.remove("uuid");
    // jsonObject.insert("_id", uuid );
        
    QString databaseName = jsonObject.take("database").toString();
    QString collectionName = jsonObject.take("collection").toString();
    
    mongoc_collection_t * collection = mongoc_client_get_collection( __client, databaseName.toLatin1().data(), collectionName.toLatin1().data() );
    if ( collection ) {
        
        qDebug() << "Was create collection: " << collection;
        
        // Convert our json object to string.
        // Преобразуем наш json-объект в строку.
        // QJsonDocument doc( jsonObject );
        // QString strJson(doc.toJson( QJsonDocument::Indented ));
        // uint8_t * dataJson = (uint8_t * ) strJson.toLocal8Bit().data();
        
        // Create bson_t object for storing.
        // Создание объекта bson_t для записи.
        bson_error_t error;
        bson_t * document = __create_bson( jsonObject ); // bson_new(); // _from_json( dataJson, strlen( (const char *) dataJson ), & error );
                                    
        // Store in database
        // Сохранение в базе данных.
            
        // bson_t * insert = BCON_NEW ("hello", BCON_UTF8 ("world"));
                        
        bool ok = mongoc_collection_insert( collection, MONGOC_INSERT_NONE, document, NULL, & error );
        if ( ! ok ) {
            qDebug() << "Insertion was failed. Error code=" << error.code << ", message=" << error.message;
        } else qDebug() << "Inserted one ok";
            
        bson_destroy( document );
        // bson_destroy( insert );
            
        
        mongoc_collection_destroy( collection );
        
    } else qDebug() << "MongoStorage::store( QJsonObject ), collection is empty";
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                  The destructor.                                                 *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                    Деструктор.                                                   *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::MongoStorage::~MongoStorage() {
    
    // if ( __collection ) mongoc_collection_destroy ( __collection );
    // if ( __database ) mongoc_database_destroy ( __database );
    if ( __client) mongoc_client_destroy ( __client );
    mongoc_cleanup ();
    
}

