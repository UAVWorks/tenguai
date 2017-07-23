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
        if ( className == "Processor" )     e = new Processor();
        if ( className == "ProcessStart" )  e = new ProcessStart();
        if ( className == "ProcessStop" )   e = new ProcessStop();
        if ( className == "Task" )          e = new Task(); 
        if ( className == "Sprout" )        e = new Sprout();
        if ( className == "ORer" )          e = new ORer();
        if ( className == "ANDor" )         e = new ANDor();
        if ( className == "Vehicle" )       e = new Vehicle();
        
    };

    if ( e ) {
        
        _readFromJSON( e, json );
        _initFocusNeighbors( e );
           
        /*
        int prevCount = 0;
        int nextCount = 0;
        
        AbstractAgent * a = dynamic_cast< AbstractAgent * >( e ) ;
        if ( a ) {
            
            prevCount = a->_uninitedPrevious.count();
            nextCount = a->_uninitedNext.count();
        
            QList< AbstractAgent * > chi;
            a->childrenRecursive( chi );
            for ( int i=0; i<chi.count(); i++ ) {
                AbstractAgent * c = chi.at(i);
                prevCount += c->_uninitedPrevious.count();
                nextCount += c->_uninitedNext.count();
                // qDebug() << "   Один дитя " << c->_class_name << ", " << c->getHumanName() << ", uuid=" << c->getUUID();
                // qDebug() << "      неин. пред=" << c->_uninitedPrevious.count() << ", неин. исх." << c->_uninitedNext.count();
                // qDebug() << "      фокус пред=" << c->previousByFocus().count() << ", фокус исх" << c->nextByFocus().count();
            };
        };
        
        // qDebug() << "<--createEntity end, "<< e->_class_name << ", " << e->getHumanName() << "неин. вход." << prevCount << ", неин.исх=" << nextCount;
        */
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
// *                                           Initialization agent's links.                                          *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                           Инициализация связей агента.                                           *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::AgentFactory::_initFocusNeighbors ( tengu::AbstractEntity * entity ) {
        
    AbstractAgent * agent = dynamic_cast< AbstractAgent * > ( entity );
    
    if ( agent ) {
        
        QList<AbstractAgent * > chi;
        agent->childrenRecursive( chi );
        
        for ( int i=0; i<chi.count(); i++ ) {
            
            AbstractAgent * one_child = chi.at(i);
            
            if ( one_child->_uninitedPrevious.count() > 0 ) {
                
                QList<QString> l = one_child->_uninitedPrevious;
                for ( int k=0; k<l.count(); k++ ) {
                    
                    AbstractAgent * neighbor = agent->findChildByUUID( l.at(k) );                    
                    
                    if ( neighbor ) {
                        one_child->addPreviousByFocus( neighbor );
                        one_child->_uninitedPrevious.removeOne( l.at(k) );
                    };
                };
            }
            
            if ( one_child->_uninitedNext.count() > 0 ) {
                
                QList<QString> l = one_child->_uninitedNext;
                
                for ( int k=0; k<l.count(); k++ ) {
                                        
                    AbstractAgent * neighbor = agent->findChildByUUID( l.at(k) );
                    
                    if ( neighbor ) {
                        one_child->addNextByFocus( neighbor );
                        one_child->_uninitedNext.removeOne( l.at(k) );
                    }
                };
            }
        };
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

