// ********************************************************************************************************************
// *                                                                                                                  *
// *                                The graphics item which can change executable mode.                               *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                            Графический элемент, который может менять режим выполнения.                           *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 05 jul 2017 at 21:25 *
// ********************************************************************************************************************

#include "EMChangableItem.h"

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                     The constructor.                                             *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                       Конструктор.                                               *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::EMChangableItem::EMChangableItem ( tengu::AbstractEntity* entity, QGraphicsItem* parent ) 
    : AbstractEntityItem ( entity, parent ) 
{

}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                 Get the properties list.                                         *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                  Получить список свойств.                                        *
// *                                                                                                                  *
// ********************************************************************************************************************

QList< QList< tengu::AgentPropertyElement > > tengu::EMChangableItem::properties() {
    
    QList < QList < tengu::AgentPropertyElement > > list = tengu::AbstractEntityItem::properties();
    
    // Agent execution mode selector. This is an combo-box.
    // Выбор режима выполнения данного агента. Это - комбо-бокс.
    
    QList<AgentPropertyElement> executionModeElement;    
    executionModeElement.append( AgentPropertyElement::captionElement(  tr("Execution mode") ) );    
    AgentPropertyElement executionModeValue;
    executionModeValue.type = AgentPropertyElement::ExecutionModeSelector;
    executionModeValue.readOnly = false;
    executionModeValue.propertyName = "execution_mode";
    executionModeValue.value = QVariant( (int) getExecutionMode() );
    executionModeValue.backgroundColor = executionModeValue.widgetBackground();        
    
    executionModeElement.append( executionModeValue );
    
    list.append( executionModeElement );

    return list;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                     The destructor.                                              *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                       Деструктор.                                                *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::EMChangableItem::~EMChangableItem() {

}

