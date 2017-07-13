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
    __indexesErrorOccured = false;
    
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
            else if ( val.isBool() ) bson_append_bool( doc, ckey, ckey_len, val.toBool() );
            else if ( val.isObject() ) {
                
                // qDebug() << "Реентерабельный вызов для объекта документа." << val ;
                
                bson_t * bson_as_object = __create_bson( val.toObject() );
                bson_append_document( doc, ckey, ckey_len, bson_as_object );
                
            } else if ( val.isArray() ) {
                
                // An array of elements
                // Массив элементов.
                
                bson_t * bson_array = bson_new(); 
                bson_append_array_begin( doc, ckey, ckey_len, bson_array );
                
                QJsonArray arr = val.toArray();
                for ( int aIndex=0; aIndex < arr.size(); aIndex ++ ) {
                    
                    // Массив с точки зрения bson'а имеет индексы, представленные строкой и начинающиеся с 1.
                    
                    QString arrKey = QString::number( aIndex + 1 );
                    char arr_key[32];
                    memset( arr_key, 0, sizeof( arr_key ) );
                    strcpy( arr_key, arrKey.toLatin1().data() );
                    int arr_key_len = arrKey.length();
                    
                    QJsonValue aElement = arr.at( aIndex );
                    
                    if ( aElement.isObject() ) {
                        // qDebug() << "Реентерабельный вызов для объекта массива" << aElement;
                        bson_t * adoc = __create_bson( aElement.toObject() );
                        bson_append_document( bson_array, arr_key, arr_key_len, adoc );
                        
                    } else if ( aElement.isString() ) bson_append_utf8( bson_array, arr_key, arr_key_len, aElement.toString().toUtf8(), -1 );
                    else if ( aElement.isDouble() ) bson_append_double( bson_array, arr_key, arr_key_len, aElement.toDouble() );
                    else if ( aElement.isBool() ) bson_append_bool( bson_array, arr_key, arr_key_len, aElement.toBool() );
                    else {
                        qDebug() << "MongoStorage::__create_bson, array element is not object: " << aElement;
                    };
                };
                
                bson_append_array_end( doc, bson_array );
                
                
            } else {
                qDebug() << "MongoStorage::__create_bson: field " << key << " was not added, type=" << val.type();
            };
        };
        
    };
    
    // To see it on the screen.
    // Чтобы посмотреть на экране.
    
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
                                                
                    };
                    
                    
                };
                
            };
            
            // printf ("%s\n", str);
            bson_free (str);
            
        } else {
            
            // An error can be only collection does not exists or database iteslf does not exists.
            // Ошибки, которые могут быть, это отсутствие базы данных или коллекции.
            // qDebug() << "MongoStorage::__getExistingIndexes, failed to run command, error=" << QString(error.message);
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
        cmd_jso["createIndexes.createdCollectionAutomatically"] = true;
        QJsonArray arr;
        arr.append( idx.toJSON() );
        cmd_jso["indexes"] = arr;

        bson_t * cmd = __create_bson( cmd_jso );
        if ( cmd ) {
            
            bson_t reply;
            bson_error_t error;
            
            if (mongoc_collection_command_simple ( collection, cmd, NULL, & reply, & error)) {
               
                // Finished successfully.
                // Закончилось успешно.
                
                // char * str = bson_as_json ( &reply, NULL);
                // printf ("Answer was: %s\n", str);
                // bson_free (str);
                
            } else {
                if ( ! __indexesErrorOccured ) {
                    __indexesErrorOccured = true;
                    QString message = QString(error.message) + "\n" + tr("It seems you need update your MongoDB server.");
                    emit signalError( EL_WARNING, "MongoStorage::addIndex()", message );
                    // qDebug() << "MongoStorage::__addIndex, failed to run command, error=" << QString(error.message);
                };
            }
            
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
    if ( __indexesErrorOccured ) return;
    
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
        if ( ( ! existing_indexes.contains( toCreate.name ) ) && ( ! __indexesErrorOccured ) ) __addIndex( o, toCreate );
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
// *                            This value is empty and it must be deleted from object.                               *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                               Значение - пустое, его нужно удалить из объекта.                                   *
// *                                                                                                                  *
// ********************************************************************************************************************

bool tengu::MongoStorage::__valueMustBeDeleted( QJsonValue val ) {
    
    return ( ( val.isNull() ) 
        || ( ( val.isObject() ) && ( val.toObject().keys().count() == 0 ) ) 
        || ( ( val.isArray() ) && ( val.toArray().count() == 0 ) ) 
    );
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                        Can this value to be simplified?                                          *
// * ---------------------------------------------------------------------------------------------------------------- *
// * Может ли данное значение быть "упрощено"? *
// * *
// ********************************************************************************************************************

bool tengu::MongoStorage::__simplifyable( QJsonValue val ) {

    return ( ( val.isObject() ) || ( val.isArray() ) );
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                         Get "the reference": collection name and uuid of this object.                            *
// * ---------------------------------------------------------------------------------------------------------------- *
// * Получить "ссылку": имя коллекции и uuid данного объекта. *
// * *
// ********************************************************************************************************************

QJsonObject tengu::MongoStorage::__getReference( QJsonValue val ) {

    QJsonObject ref;
    if ( val.isObject() ) {
        QJsonObject oval = val.toObject();
        if ( ( oval.contains( JSON_COLLECTION_ELEMENT ) ) && ( oval.contains( JSON_UUID_ELEMENT ) ) ) {
            QString uuid = oval.value( JSON_UUID_ELEMENT ).toString();
            QString collection = oval.value( JSON_COLLECTION_ELEMENT ).toString();
            if ( ( ! uuid.isEmpty() ) && ( ! collection.isEmpty() ) ) {
                ref[ JSON_UUID_ELEMENT ] = uuid;
                ref[ JSON_COLLECTION_ELEMENT ] = collection;
            };
        };
    };
    return ref;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                    Pick up one storageable object from QJsonValue and store it to database.                      *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                        Выцепить один хранимый объект из QJsonValue и записать его в базу.                        *
// *                                                                                                                  *
// ********************************************************************************************************************

bool tengu::MongoStorage::__simplify( QJsonValue & val ) {

    if ( ! __simplifyable( val ) ) return false;
    
    bool result = false;
    
    if ( val.isObject() ) {
        
        QJsonObject oval = val.toObject();
        
        // reenterable call, store child's object.
        // Реентерабельный вызов, запись дитяти.
            
        bool modified = true;
        while ( modified ) {
                
            modified = false;
                
            for ( int i=0; i<oval.keys().count(); i++ ) {
                
                QString one_key = oval.keys().at(i);
                QJsonValue one_value = oval[ one_key ];
                QJsonObject reference = __getReference( one_value );
                
                if ( __simplifyable( one_value ) ) {
                    bool one_element_simplified = __simplify( one_value );                    
                    if ( one_element_simplified ) {
                        result = true;
                        modified = true;
                        if ( __valueMustBeDeleted( one_value ) ) {

                            // Delete this value. But perhaps at the same time replace it with his link.
                            // Удаляем это значение. Но, возможно, при этом заменяем его на его ссылку.
                            
                            oval.remove( one_key );
                            if ( ! reference.isEmpty() ) oval[ one_key ] = reference;
                            
                        } else {
                            
                            // Value has been modified, replace it.
                            // Значение изменилось, замещаем его.
                            
                            oval[ one_key ] = one_value;
                        };
                        break;
                    };
                };
            };
            
            if ( modified ) {
                val = oval;
            };
                
        };
            
        if ( storageable( oval ) ) {
            __insert_single_object( oval );
            result = true;
        };
        
        if ( result ) return result;
    };
    
    if ( val.isArray() ) {
                 
        QJsonArray arr = val.toArray();
        bool modified = true;
        while ( modified ) {
            
            modified = false;
            
            for ( int aIndex=0; aIndex < arr.size(); aIndex ++ ) {
                QJsonValue array_element = arr.at( aIndex );
                QJsonObject reference = __getReference( array_element );
                
                if ( __simplifyable( array_element ) ) {
                    bool one_element_simplified = __simplify( array_element );
                    if ( one_element_simplified ) {
                        
                        result = true;
                        modified = true;
                    
                        if ( ! reference.isEmpty() ) arr.replace( aIndex, reference );
                        else arr.removeAt( aIndex );
                        
                        break;
                        
                    };
                };
            };
            
            if ( modified ) {
                val = arr;
            };
            
        };
        
    };
    
    return result;

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
    
    bool modified = true;
    while ( modified ) {
        
        modified = false;
        
        for ( int i=0; i<keys.size(); i++ ) {            
            QJsonValue val = o.value( keys.at(i) );
            QJsonObject reference = __getReference( val );
            if ( __simplifyable( val ) ) {
                
                if ( __simplify( val ) ) {
                
                    modified = true;
                    if ( __valueMustBeDeleted( val ) ) {
                        o.remove( keys.at(i) );
                        if ( ! reference.isEmpty() ) o[ keys.at(i) ] = reference;
                    } else {
                        o[ keys.at(i) ] = val;
                    };
                    break;
                };
            
            };
            
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
        
        // Create bson_t object for storing.
        // Создание объекта bson_t для записи.
        
        bson_error_t error;
        bson_t * document = __create_bson( jsonObject ); 
                                    
        // Store in database
        // Сохранение в базе данных.
            
        bool ok = mongoc_collection_insert( collection, MONGOC_INSERT_NONE, document, NULL, & error );
        if ( ! ok ) {
            emit signalError( EL_CRITICAL, "MongoStorage::insert_single_object", QString( error.message ) );
            // qDebug() << "MongoStorage::__insert_single_object, insertion was failed. Error code=" << error.code << ", message=" << error.message;
        }
            
        bson_destroy( document );
            
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

