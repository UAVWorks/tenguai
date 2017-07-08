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
    __alreadyIndexedCollections = QList<QString>();
    
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
        
        // qDebug() << "Key: " << key << ", bson key: " << ckey << ", len=" << ckey_len;
                
        if ( key == "uuid" ) bson_append_utf8( doc, "_id", -1, o.value( key ).toString().toUtf8(), -1  );
        else {
            
            QJsonValue val = o.value( key );
            if ( val.isString() ) bson_append_utf8( doc, ckey, ckey_len, val.toString().toUtf8(), -1 );
            else if ( val.isDouble() ) bson_append_double( doc, ckey, ckey_len, val.toDouble() );
            else if ( val.isArray() ) {
                QJsonArray arr = val.toArray();
                for ( int aIndex=0; aIndex < arr.size(); aIndex ++ ) {
                    QJsonValue aElement = arr.at( aIndex );
                    if ( aElement.isObject() ) {
                        bson_t * adoc = __create_bson( aElement.toObject() );
                    } else {
                        qDebug() << "MongoStorage::__create_bson, array element is not object: " << aElement;
                    };
                };
            } else {
                qDebug() << "MongoStorage::__create_bson: field " << key << " was not added, type=" << val.type();
            };
        };
        
    };
    
    // char * str = bson_as_json ( doc , NULL );
    // qDebug() << "Bson to insert: " << QString(str) ;
    // bson_free( str );
    
    return doc;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                    Get mongoc collection for specified element                                   *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                Получить mongoc-коллекцию для указанного элемента.                                *
// *                                                                                                                  *
// ********************************************************************************************************************

mongoc_collection_t * tengu::MongoStorage::__getCollection(QJsonObject o) {
    
    QString databaseName = o.value("database").toString();
    QString collectionName = o.value("collection").toString();
    
    mongoc_collection_t * collection = mongoc_client_get_collection( 
        __client, 
        databaseName.toLatin1().data(), 
        collectionName.toLatin1().data() 
    );
    
    if ( ! collection ) {
    
        qDebug() << "MongoStorage::__getCollection(), empty collection for element:";
        qDebug() << o;
    };
    
    return collection;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                       Get existing indexes for collection                                        *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                    Получить существующие индексы для коллекции.                                  *
// *                                                                                                                  *
// ********************************************************************************************************************

QMap< QString, tengu::MongoIndex > tengu::MongoStorage::__getExistingIndexes( QJsonObject o ) {
    
    QMap< QString, tengu::MongoIndex > result;
    
    mongoc_collection_t * collection = __getCollection( o );
    
    if ( collection ) {
        
        bson_t * command = BCON_NEW("listIndexes", BCON_UTF8( o.value( JSON_COLLECTION_ELEMENT ).toString().toLocal8Bit().data() ) );
        bson_t reply;
        bson_error_t error;
        
        if (mongoc_collection_command_simple ( collection, command, NULL, & reply, & error)) {
            
            char * str = bson_as_json ( &reply, NULL);
            QJsonDocument adoc = QJsonDocument::fromJson( str );
            QJsonObject answer = adoc.object();
            if ( answer.contains("cursor") ) {
                
                QJsonObject aCursor = answer.value("cursor").toObject();
                if ( aCursor.contains("firstBatch") ) {
                    
                    // Array of existing mongo indexes.
                    // Массив существующих индексов монги.
                    
                    QJsonArray firstBatch = aCursor.value("firstBatch").toArray();
                    
                    for ( int k=0; k<firstBatch.size(); k++ ) {
                        
                        // Representation of one index.
                        // Представление одного индекса.
                        
                        QJsonObject oik = firstBatch.at( k ).toObject();
                        tengu::MongoIndex idx;
                        idx.fromJSON( oik );
                        if ( ! idx.name.isEmpty() ) result[ idx.name ] = idx;
                        
                        /*
                        if ( oik.contains("key") ) {
                            QJsonObject existing_key = oik.value("key").toObject();
                            if ( existing_key.keys().count() != 1 ) {
                                qDebug() << "MongoStorage::__getExistingIndexes, object 'key' has more than one key.";
                                qDebug() << existing_key;
                            } else {
                                QString kName = existing_key.keys().at(0);
                                int kValue = existing_key.value( kName ).toInt();
                                result[kName] = kValue;
                            }                            
                        };
                        */
                        
                    };
                    
                    
                };
                
            };
            
            // printf ("%s\n", str);
            bson_free (str);
            
        } else {
            
            qDebug() << "MongoStorage::__getExistingIndexes, failed to run command, error=" << QString(error.message);
        }
        
        bson_destroy (command);
        bson_destroy (&reply);
        mongoc_collection_destroy( collection );
    };
    
    return result;
};

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                     Add the index to the existing mongo collection.                              *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                     Добавить индекс в существующую коллекцию монги.                              *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::MongoStorage::__addIndex( QJsonObject o, tengu::MongoIndex idx ) {
    
    
    mongoc_collection_t * collection = __getCollection( o );
    
    if ( collection ) {
    
        QJsonObject cmd_jso;
        cmd_jso["createIndexes"] = o.value( JSON_COLLECTION_ELEMENT ).toString();
        QJsonArray arr;
        arr.append( idx.toJSON() );
        cmd_jso["indexes"] = arr;
        
        bson_t * cmd = __create_bson( cmd_jso );
        if ( cmd ) {
            
            bson_t reply;
            bson_error_t error;
            /*
            if (mongoc_collection_command_simple ( collection, command, NULL, & reply, & error)) {
                
                char * str = bson_as_json ( &reply, NULL);
                
                printf ("%s\n", str);
                bson_free (str);
                
            } else {
                
                qDebug() << "MongoStorage::__addIndex, failed to run command, error=" << QString(error.message);
            }
            */
            bson_destroy (cmd);
            bson_destroy (&reply);
            
        };
        
        mongoc_collection_destroy( collection );
    };
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                   Check indexes for this element collection.                                     *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                               Проверка индексов для коллекции данного элемента.                                  *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::MongoStorage::checkIndexes( tengu::AbstractEntity * e ) {
    
    if ( ! storageable( e ) ) return;
    QJsonObject o = e->toJSON();
    if ( __alreadyIndexedCollections.contains( o[ JSON_COLLECTION_ELEMENT ].toString() ) ) return;
        
    QMap<QString, tengu::MongoIndex > existing_indexes = __getExistingIndexes( o );
    QMap<QString, tengu::MongoIndex > indexes = e->indexes();
    
    // Add those indices that are not in the collection.
    // Добавляем те индексы, которых нет в коллекции.
        
    QMapIterator<QString, tengu::MongoIndex> idx( indexes );
    while (idx.hasNext()) {
        idx.next();
        tengu::MongoIndex toCreate = idx.value();
        if ( ! existing_indexes.contains( toCreate.name ) ) __addIndex( o, toCreate );
    };

}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                             Store AbstractEntity object than can contain other objects.                          *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                        Сохранить AbstractEntity объект, который может содержать другие объекты.                  *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::MongoStorage::store ( tengu::AbstractEntity * e ) {
    
    if ( ! storageable( e ) ) {
        qDebug() << "MongoStorage::store( AbstractEntity * ), object is not storageable";
        return;
    };
    
    QJsonObject o = e->toJSON();
    store( o );

}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                      Store the JSON-presented object, procedure can be reenterable                               *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                Сохранить объект, представленный JSON'ом. Процедура может быть реентерабельной.                   *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::MongoStorage::store( QJsonObject o ) {
    
    if ( ! storageable( o ) ) {
        qDebug() << "MongoStorage::store( QJsonObject ), object is not storageable";
        qDebug() << o;
        return;
    };
    
    // We can not insert a set of separate object at once.
    // Мы не можем вставлять множество разных объектов за один раз.

    QStringList keys = o.keys();
    
    for ( int i=0; i<keys.size(); i++ ) {
        
        QJsonValue val = o.value( keys.at(i) );
        
        if ( val.isObject() ) {
            
            // reenterable call, store child's object.
            // Реентерабельный вызов, запись дитяти.
            
            qDebug() << "MongoStorage::store, reenterable call";
            store( o );
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

    qDebug() << "MongoStorage::__insert_single_object: " << jsonObject ;
    
    // if ( ( ! jsonObject.contains("database") ) || ( ! jsonObject.contains("collection") ) || ( ! jsonObject.contains("uuid")  ) ) {
    if ( ! storageable( jsonObject ) ) {
        qDebug() << "MongoStorage::store( QJsonObject ), object does not contains uuid | database | collection";
        qDebug() << jsonObject;
        return;
    };
    
    if ( ! __client ) {
        qDebug() << "MongoStorage::store( QJsonObject ), client is not initialized.";
        return;
    };
    
    mongoc_collection_t * collection = __getCollection( jsonObject );
            
    if ( collection ) {
        
        // qDebug() << "Was create collection: " << collection;
        
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
            qDebug() << "MongoStorage::__insert_single_object, insertion was failed. Error code=" << error.code << ", message=" << error.message;
        } else qDebug() << "MongoStorage::__insert_single_object: Inserted one ok";
            
        bson_destroy( document );
        // bson_destroy( insert );
            
        
        mongoc_collection_destroy( collection );
        
    } else qDebug() << "MongoStorage::__insert_single_object( QJsonObject ), collection is empty";
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                       Can this JSON object representation to be stored into database?                            *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                    Может ли данное JSON-представление объекта быть записано в базу данных?                       *
// *                                                                                                                  *
// ********************************************************************************************************************

bool tengu::MongoStorage::storageable(QJsonObject o) {
    
    return ( o.contains( JSON_DATABASE_ELEMENT ) && o.contains( JSON_COLLECTION_ELEMENT ) && o.contains( JSON_UUID_ELEMENT ) );

}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                    Can this object to be stored into database?                                   *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                 Может ли данный объект быть записан в базу данных?                               *
// *                                                                                                                  *
// ********************************************************************************************************************

bool tengu::MongoStorage::storageable(tengu::AbstractEntity* e) {
    
    QJsonObject o = e->toJSON();
    return storageable( o );
    
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

