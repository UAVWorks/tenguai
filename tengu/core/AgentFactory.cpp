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
    
    if ( json.contains("class_name" ) ) {
        
        QString cName = json.value("class_name").toString();
        
        if ( cName == "ProcessStart" )  e = new ProcessStart();
        if ( cName == "ProcessStop" )   e = new ProcessStop();
        if ( cName == "Task" )          e = new Task(); 
        if ( cName == "Sprout" )        e = new Sprout();
        if ( cName == "ORer" )          e = new ORer();
        if ( cName == "ANDor" )         e = new ANDor();
        
    };

    if ( e ) e->fromJSON( json );
    
//    else {
//        qDebug() << "AgentFactory::hasAgent(), unhandled agent name " << json;
//    };
    
    return e;
    
}

