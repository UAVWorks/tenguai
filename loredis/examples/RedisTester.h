#pragma once

#include <unistd.h>

#include <QThread>
#include <QDebug>
#include <QString>
#include <QCoreApplication>

#include "../src/LoRedis.h"

class RedisTester : public QThread {
    
    Q_OBJECT
    
    public:
        
        RedisTester();
        ~RedisTester();
        
    private:
        
        volatile bool connected;
        
        /**
         * @short The "basic" redis client to perform the most of operations.
         */
        
        LoRedis * redis;        
        
        /**
         * @short Another client for receiving messages from subscribed channels.
         * 
         * "Once the client enters the subscribed state it is not supposed to issue any other commands"
         * (from redis.io site).
         */
        
        LoRedis * subscriber;
        void run();
        
    private slots:
        
        void onConnected();
        void onDisconnected();
        void onError( QString message );
        void onSubscribed( QString channel );
        void onGotMessage( QString channel, QString message );
};