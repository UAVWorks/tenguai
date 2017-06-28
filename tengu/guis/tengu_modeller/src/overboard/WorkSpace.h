// ********************************************************************************************************************
// *                                                                                                                  *
// *      The top level for modelling of a system: a set of agents and it's states for graphics representation.       *
// * ---------------------------------------------------------------------------------------------------------------- *
// *    Верхний уровень моделирования системы: множество агентов и их состояний для графического представления.       *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 02 jun 2017 at 16:47 *
// ********************************************************************************************************************

#pragma once

#include <QList>

#include "AbstractAgent.h"
#include "AgentFactory.h"

namespace tengu {

    class WorkSpace : public AbstractAgent {
    
        Q_OBJECT
        public:
            
            // Graphics item types. To avoid checking every time through dynamic_cast only
            
            enum graphics_item_types_t {
                GI_WorkSpace
            };                        
            
            WorkSpace();
            virtual ~WorkSpace();
            
            /**
             * @short Get all entities of specified type, whose name or comment contains the mask.
             * Получить все "сущности" указанного типа, имеющие маску в своем имени или комментарии.
             */
            
            template <class T> QList< T > getEntities ( QString mask ) {

                QList < T > result;
    
                foreach( AbstractAgentKernel * oneChild, _children ) {
        
                    T entity = __satisfiedTheMask< T >( oneChild, mask );
                    if ( entity ) result.append( entity );
        
                    // Re-enterable call procedure for children of this agent.
                    // Реентерабельный вызов процедуры для детей этого агента.
        
                    QList < AbstractAgentKernel *>  reChildrenList = oneChild->children();
                    if ( reChildrenList.count() > 0 ) {
                        for ( int cIndex=0; cIndex < reChildrenList.count(); cIndex ++ ) {
                            AbstractAgentKernel * reChild = reChildrenList.at( cIndex );
                            entity = __satisfiedTheMask< T >( reChild, mask );
                            if ( entity ) result.append( entity );
                        };
                    };
                
                };
        
                return( result );
            };

            
        private:
            
            /**
             * @short Check type of this entity and it contains the mask in it's names.
             * Проверить тип сущности, а так же что она содержит маску в именах.
             */
                        
            template <class T > T __satisfiedTheMask ( tengu::AbstractAgentKernel* child, QString mask ) {

                QString sName = child->getSystemName();
                QString hName = child->getHumanName();
                QString comment = child->getComment();
                
                if ( mask.isEmpty() || ( sName.contains( mask ) ) || ( hName.contains( mask ) ) || ( comment.contains(mask) ) ) {
        
                    T och = dynamic_cast< T >( child );
                    if ( och ) return och;
        
                };  
    
                return nullptr;
            };

    }; // class WorkSpace
    
}; // namespace tengu

