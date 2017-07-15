// ********************************************************************************************************************
// *                                                                                                                  *
// *                                        Agent factory, agent's classes recognizer.                                *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                   "Фабрика агентов", распознавание классов агентов.                              *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 16 jun 2017 at 09:40 *
// ********************************************************************************************************************

#include "AgentFactory.h"

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                            Is there any top-level agent?                                         *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                   Есть ли здесь какой-нибудь высокоуровневый агент?                              *
// *                                                                                                                  *
// ********************************************************************************************************************
/*
bool tengu::AgentFactory::hasAgent ( QJSonObject json ) {
    
    if ( ProcessStart().fromJSON( json ) ) return true;
    if ( ProcessStop().fromJSON( json ) ) return true;
    if ( Task().fromJSON() ) return true;
    return false;
}
*/
// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                Create agent from JSON                                            *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                Создать агента из JSONа.                                          *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::AbstractEntity * tengu::AgentFactory::createEntity ( QJsonObject json ) {
    
    AbstractEntity * e = nullptr;
    
    QString className;
    if ( json.contains("class_name") ) className = json[ "class_name" ].toString();
    else if ( json.contains( JSON_COLLECTION_ELEMENT ) ) className = AgentFactory::getClassName( json[ JSON_COLLECTION_ELEMENT ].toString() );
    
    if ( ! className.isEmpty() )  {
        
        if ( className == "Process" )       e = new Process();
        if ( className == "ProcessStart" )  e = new ProcessStart();
        if ( className == "ProcessStop" )   e = new ProcessStop();
        if ( className == "Task" )          e = new Task(); 
        if ( className == "Sprout" )        e = new Sprout();
        if ( className == "ORer" )          e = new ORer();
        if ( className == "ANDor" )         e = new ANDor();
        
    };

    if ( e ) {
        _readFromJSON( e, json );
    };
    
//    else {
//        qDebug() << "AgentFactory::hasAgent(), unhandled agent name " << json;
//    };
    
    return e;
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                            Recursive reading object from JSON.                                   *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                           Рекурсивное чтение объекта из JSON'а.                                  *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AgentFactory::_readFromJSON ( tengu::AbstractEntity * entity, QJsonObject json ) {
        
    entity->fromJSON( json );
    QList<QString> hisKeys = json.keys();
    for ( int i=0; i<hisKeys.count(); i++ ) {
        
        QString oneKey = hisKeys.at( i );                
        QJsonValue val = json.value( oneKey );
        
        if ( val.isObject() ) {
            
            QJsonObject oval = val.toObject();
            _append_object( entity, oneKey, oval );
            
        } else if ( val.isArray() ) {
            
            QJsonArray aval = val.toArray();
            
            for ( int ai=0; ai<aval.count(); ai++ ) {
                QJsonValue array_value = aval.at( ai );
                if ( array_value.isObject() ) {
                    QJsonObject oval = array_value.toObject();
                    _append_object( entity, oneKey, oval );
                };
            }
        }
    };
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                        Possible append an object to AbstractEntity (or to his descendants)                       *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                          Возможное добавление объектов к AbstractEntity (либо ее потомку).                       *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AgentFactory::_append_object ( tengu::AbstractEntity * entity, QString key, QJsonObject json ) {
    
    AbstractEntity * child = AgentFactory::createEntity( json );

    if ( child ) {
                
        if ( key == "children" ) {
            
            AbstractAgent * agent = dynamic_cast< AbstractAgent * > ( entity );
            AbstractAgent * childAgent = dynamic_cast < AbstractAgent * >( child );
            
            if ( ! agent ) qDebug() << "AgentFactory::_append_object(), child collection, but parent entity is not agent. Key=" << key << ", json=" << json;
            if ( ! childAgent ) qDebug() << "AgentFactory::_append_object(), child collection, but child is not agent. Key=" << key << ", json=" << json;
            
            if ( ( agent ) && ( childAgent ) ) {                                
                agent->addChild( childAgent );
            };
            
        } else qDebug() << "AgentFactory::_append_object(): We have an object, but do not know how to add it. Key=" << key << ", json=" << json;
        
    };
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                        Get class name for this collection name                                   *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                        Получить имя класса по имени коллекции.                                   *
// *                                                                                                                  *
// ********************************************************************************************************************

QString tengu::AgentFactory::getClassName( QString collectionName ) {
    if ( collectionName == "tasks" ) return "Task";
    if ( collectionName == "processes" ) return "Process";
    if ( collectionName == "vehicles" ) return "Vehicle";
    return "";
}

