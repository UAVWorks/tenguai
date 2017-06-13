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
    __database = nullptr;
    __collection = nullptr;
    
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
// *                                         Store one JSON object to database.                                       *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                       Сохранить один JSON-объект в базе данных.                                  *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::MongoStorage::store ( QJsonObject jsonObject ) {
    
    if ( ( ! jsonObject.contains("database") ) || ( ! jsonObject.contains("collection") ) || ( ! jsonObject.contains("uuid")  ) ) {
        qDebug() << "MongoStorage::store( QJsonObject ), object does not contains uuid | database | collection";
        qDebug() << jsonObject;
        return;
    };
    
    if ( ! __client ) {
        qDebug() << "MongoStorage::store( QJsonObject ), client is not initialized.";
        return;
    };
    
    QString databaseName = jsonObject.take("database").toString();
    QString collectionName = jsonObject.take("collection").toString();
    
    mongoc_collection_t * collection = mongoc_client_get_collection( __client, databaseName.toLatin1().data(), collectionName.toLatin1().data() );
    qDebug() << "Was create collection: " << collection;
    mongoc_collection_destroy( collection );
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                  The destructor.                                                 *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                    Деструктор.                                                   *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::MongoStorage::~MongoStorage() {
    
    if ( __collection ) mongoc_collection_destroy ( __collection );
    if ( __database ) mongoc_database_destroy ( __database );
    if ( __client) mongoc_client_destroy ( __client );
    mongoc_cleanup ();
    
}

