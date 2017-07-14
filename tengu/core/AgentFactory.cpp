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

    if ( e ) e->fromJSON( json );
    
//    else {
//        qDebug() << "AgentFactory::hasAgent(), unhandled agent name " << json;
//    };
    
    return e;
    
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
    return "";
}
