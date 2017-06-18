// ********************************************************************************************************************
// *                                                                                                                  *
// *                             Agent item factory, the descedant from AgentFactory.                                 *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                        Фабрика графических элементов агентов, потомок от AgentFactory.                           *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 17 jun 2017 at 13:55 *
// ********************************************************************************************************************

#include "AgentItemFactory.h"

tengu::AbstractEntity* tengu::AgentItemFactory::createEntity ( QJsonObject json ) {

    AbstractEntity * e = AgentFactory::createEntity( json );
    if ( e ) return e;    
    
    if ( json.contains("class_name" ) ) {
        
        QString cName = json.value("class_name").toString();
        
        if ( cName == "ProcessStartItem" )  e = new ProcessStartItem();
        if ( cName == "ProcessStopItem" )   e = new ProcessStopItem();
        if ( cName == "TaskItem" )          e = new TaskItem();
        if ( cName == "ANDorItem" )         e = new ANDorItem();
        if ( cName == "ORerItem" )          e = new ORerItem();
        if ( cName == "LinkItem" )          e = new LinkItem();
        
        // XPlaneAgent does not birth yet.
        // XPlaneAgent не порождается пока что.
        // if ( cName == "XPlaneAgentItem" )   e = new XPlaneAgentItem();
        
    };

    if ( e ) e->fromJSON( json );
    
//    else {
//        qDebug() << "AgentFactory::hasAgent(), unhandled agent name " << json;
//    };
    
    return e;
}


