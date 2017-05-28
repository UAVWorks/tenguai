// ********************************************************************************************************************
// *                                                                                                                  *
// *         An abstract class for obtaining reactions from lordis directly, without the QApplication event cycle.    *
// * ---------------------------------------------------------------------------------------------------------------- *
// *          Абстрактный класс для получения реакций от LoRedis напрямую, без событийного цикла QApplication         *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       created 04 may 2017 at 20:59 *
// ********************************************************************************************************************

#pragma once

#include <QString>
#include <QVariant>

class LoRedis;


/**
 * @short This is interface description for non-Qt libevent reactions.
 * 
 * Описание интерфейса для реакций без использования цикла событий Qt, только на libevent.
 */

class LoRedisReactor {
    
    public: 
        virtual void cbConnected( LoRedis * redis ) = 0; 
        virtual void cbDisconnected( LoRedis * redis ) = 0;
        virtual void cbError( LoRedis * redis, QString message ) = 0;
        virtual void cbSubscribed( LoRedis * redis, QString channel ) = 0;
        virtual void cbUnsubscribed( LoRedis * redis, QString channel ) = 0;
        virtual void cbGotMessage( LoRedis * redis, QString channel, QString message ) = 0;
        virtual void cbSetted( LoRedis * redis, int status ) = 0;
        virtual void cbGotValue( LoRedis * redis, QString name, QVariant value ) = 0;
        
};

