// ********************************************************************************************************************
// *                                                                                                                  *
// *                             Agent item factory, the descedant from AgentFactory.                                 *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                        Фабрика графических элементов агентов, потомок от AgentFactory.                           *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 17 jun 2017 at 13:55 *
// ********************************************************************************************************************

#include "AgentItemFactory.h"

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                        Create entity from JSON-format.                                           *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                     Создание "сущности" из формата JSON.                                         *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::AbstractEntity* tengu::AgentItemFactory::createEntity ( QJsonObject json ) {

    AbstractEntity * e = AgentFactory::createEntity( json );
    if ( e ) return e;    
    
    QString className;
    if ( json.contains("class_name") ) className = json["class_name"].toString();
    else if (json.contains(JSON_COLLECTION_ELEMENT) ) className = AgentItemFactory::getClassName( json[JSON_COLLECTION_ELEMENT].toString() );
    if ( ! className.isEmpty() ) {
        
        if ( className == "ProcessStartItem" )  e = new ProcessStartItem();
        if ( className == "ProcessStopItem" )   e = new ProcessStopItem();
        if ( className == "TaskItem" )          e = new TaskItem();
        if ( className == "SproutItem" )        e = new SproutItem();
        if ( className == "ANDorItem" )         e = new ANDorItem();
        if ( className == "ORerItem" )          e = new ORerItem();
        if ( className == "LinkItem" )          e = new LinkItem();
        if ( className == "Vehicle" )           e = new Vehicle();
        
        // XPlaneAgent does not birth yet.
        // XPlaneAgent не порождается пока что.
        // if ( cName == "XPlaneAgentItem" )   e = new XPlaneAgentItem();
        
    };

    if ( e ) e->fromJSON( json );
    
    // else {
    //    qDebug() << "AgentItemFactory::createEntity(), unhandled agent name " << json;
    // };
    
    return e;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                       Return class name by his collection name                                   *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                          Вернуть имя класса по его коллекции                                     *
// *                                                                                                                  *
// ********************************************************************************************************************

QString tengu::AgentItemFactory::getClassName( QString collectionName ) {
    QString cname = AgentFactory::getClassName( collectionName );
    if ( cname.isEmpty() ) {
        if ( collectionName == "vehicles" ) return "Vehicle";
    };
    return cname;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                Create supreme graphics item from abstract agent class                            *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                        Создание надстоящего графического элемента из класса AbstractAgent                        *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::AbstractEntityItem * tengu::AgentItemFactory::createItem ( tengu::AbstractAgent * agent ) {
    
    Task * task = dynamic_cast<Task * > ( agent );                          if ( task ) return new TaskItem( task );
    XPlaneSimulator * xplane = dynamic_cast<XPlaneSimulator *> (agent );    if ( xplane ) return new XPlaneSimulatorItem( xplane );
    Process * process = dynamic_cast< Process * > ( agent );                if ( process ) return new ProcessItem( process );
    ProcessStart * start = dynamic_cast< ProcessStart * > ( agent );        if ( start ) return new ProcessStartItem( start );    
    Vehicle * vehicle = dynamic_cast< Vehicle * > ( agent ) ;               if ( vehicle ) return new VehicleItem( vehicle );
    WorkSpace * workSpace = dynamic_cast <WorkSpace * > ( agent );          if ( workSpace ) return new WorkSpaceItem( workSpace );
    
    
    // Could not recognize the class
    // Не смогли распознать класс.
    
    if ( agent ) qDebug() << "AgentItemFactory::createItem, can not create for " << agent->getHumanName();
    
    return nullptr;
}


