// ********************************************************************************************************************
// *                                                                                                                  *
// *                                Graphics representation of X-Plane as the agent.                                  *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                 Графическое представление X-Plane как агента.                                    *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 12 jun 2017 at 09:02 *
// ********************************************************************************************************************

#include "XPlaneAgentItem.h"

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                               The constructor.                                                   *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                  Конструктор.                                                    *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::XPlaneAgentItem::XPlaneAgentItem ( XPlaneAircraft * entity, QGraphicsItem * parent ) 
    : AbstractEntityItem ( entity , parent )
{
    setVisible( false );    
}
// ********************************************************************************************************************
// *                                                                                                                  *
// *                                            Check the entity is valid.                                            *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                      Проверяем, валидна ли ссылка на entity                                      *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::XPlaneAgentItem::checkEntity() {
    // if ( ! _entity ) _entity = new XPlaneAgent();
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                  The destructor.                                                 *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                     Деструктор.                                                  *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::XPlaneAgentItem::~XPlaneAgentItem() {
    // qDebug() << "XPlaneAgentItem::~XPlaneAgentItem()";
}

