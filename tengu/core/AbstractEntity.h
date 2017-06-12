// ********************************************************************************************************************
// *                                                                                                                  *
// *                                An "entity" class, common parent for agents and sprouts.                          *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                        Класс "сущности", общий предок для агентов и ростков (связей агентов)                     *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 11 jun 2017 at 12:34 *
// ********************************************************************************************************************

#pragma once

#include <QObject>
#include <QUuid>

namespace tengu {
    
    class AbstractEntity : public QObject {
        
        Q_OBJECT
        
        Q_PROPERTY( QString name READ getName WRITE setName )
        Q_PROPERTY( QString uuid READ getUUID WRITE setUUID )
        Q_PROPERTY( QString comment READ getComment WRITE setComment )
        Q_PROPERTY( execution_mode_t executionMode READ getExecutionMode WRITE setExecutionMode )

        public:
            
            // The execution mode of model (of this "processor" and all of it's children)
            // Режим выполнения модели (данного "процессора" и всех его детей).
        
            enum execution_mode_t {
                EM_UNKNOWN,
                EM_ALWAYS,
                EM_REAL,
                EM_XPLANE,
                EM_PREPAR3D
            };                    

            explicit AbstractEntity ( QObject* parent = nullptr );
            virtual ~AbstractEntity();
            
            // true, really unique identifier for this object
            // Действительно уникальный идентификатор для данного объекта.
            
            QString getUUID();
            void setUUID( QString uuid );            
            
            // The name of this agent.
            // Имя агента.
            
            QString getName();
            virtual void setName( QString name );
            
            // The comment of this entity.
            // Комментарий данной "сущности".
            
            QString getComment();
            void setComment( QString comment );
            
            // The execution mode of this entity (real, x-plane simulation, always e.t.c.)
            // Режим выполнения данной сущности (реальный, симуляция в X-Plane, всегда и др).
            
            execution_mode_t getExecutionMode();
            virtual void setExecutionMode( execution_mode_t mode );
            
            // Has been this entity changed?
            // Была ли данная сущность изменена?
            
            bool hasChanged();
            
        protected:
            
            bool _changed;
            
        private:
            
            // Entity name
            // Имя сущности
            
            QString __name;
            
            // Any human readable comment for this entity.
            // Любой человеко-ориентированный комментарий для сущности.
            
            QString __comment;
            
            // Unique (in entire system range) identifier of this agent.
            // Уникальный (в пределах вообще всей системы) идентификатор данного агента.
            
            QString __uuid;
            
            // Execution mode
            // Режим выполнения.
            
            execution_mode_t __execution_mode;
            
    };
};

