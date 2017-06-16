// ********************************************************************************************************************
// *                                                                                                                  *
// *                         Abstract interface for entity than can be stored into some database.                     *
// * ---------------------------------------------------------------------------------------------------------------- *
// *              Абстрактный интерфейс для сущности, которая может быть записана в какой-нибудь базе данных.         *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 14 jun 2017 at 12:29 *
// ********************************************************************************************************************

#pragma once

#include <QDateTime>
#include <QString>

namespace tengu {

    class AbstractStorageableEntity {
        
        Q_PROPERTY( QString name READ getName WRITE setName )
        Q_PROPERTY( QString uuid READ getUUID )
        Q_PROPERTY( QString comment READ getComment WRITE setComment )
        Q_PROPERTY( execution_mode_t executionMode READ getExecutionMode WRITE setExecutionMode )
        Q_PROPERTY( QDateTime last_modified READ lastModified )
                
        public:
            
            // The execution mode for the model, processor, task e.t.c
            // Режим выполнения данной модели, процесса, задачи и др.
        
            enum execution_mode_t {
                EM_ALWAYS       = 0,
                EM_REAL         = 1,
                EM_XPLANE       = 2,
                EM_PREPAR3D     = 3
            };
            
            AbstractStorageableEntity();
            virtual ~AbstractStorageableEntity();
            
            virtual QString getUUID() = 0;
            virtual QString getName() = 0;
            virtual void setName( QString name ) = 0;
            virtual QString getComment() = 0;
            virtual void setComment( QString comment ) = 0;     
            virtual execution_mode_t getExecutionMode() = 0;
            virtual void setExecutionMode( execution_mode_t mode ) = 0;
            virtual bool hasChanged() = 0;
            virtual QDateTime lastModified() = 0;   
            virtual QJsonObject toJSON() = 0;
            virtual bool fromJSON( QJsonObject json ) = 0;
            
            
    };
    
};

