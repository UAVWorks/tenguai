// ********************************************************************************************************************
// *                                                                                                                  *
// *                                      The common constants for Tengu project.                                     *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                         Общие константы для проекта Tengu.                                       *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 27 may 2017 at 07:38 *
// ********************************************************************************************************************

#pragma once

#define MINIMUM_CONSTRAINT -100500
#define MAXIMUM_CONSTRAINT 100500

#define TENGU_ORGANIZATION  "Tengu"

#define TENGU_MODELLER_APPLICATION "Modeller"

#define JSON_COLLECTION_ELEMENT     "collection"
#define JSON_DATABASE_ELEMENT       "database"
#define JSON_UUID_ELEMENT           "uuid"
#define JSON_MONGOID_ELEMENT        "_id"

namespace tengu {
    
    enum error_level_t {
        EL_INFO,
        EL_WARNING,
        EL_CRITICAL
    };
    
};

