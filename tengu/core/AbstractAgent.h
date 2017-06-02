// ********************************************************************************************************************
// *                                                                                                                  *
// *      The abstract agent. Any logical completed piece which can do something. Usually it is a separate process    *
// *                       inside operation system. This is "kernel" plus communication capability                    *
// * ---------------------------------------------------------------------------------------------------------------- *
// *    Абстрактный агент. Любой логически законченный кусок, который может что-то делать. Как правило - отдельный    *
// *               процесс операционной системы. Представляет собой "ядро" агента плюс способность общаться.          *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 26 may 2017 at 11:57 *
// ********************************************************************************************************************

#pragma once

#include <QObject>
#include <QString>
#include <QTimer>
#include <QTime>
#include <QDateTime>
#include <QMutex>
#include <QDebug>

#include "AbstractAgentKernel.h"
#include "Sprout.h"

namespace tengu {

    class AbstractAgent : public AbstractAgentKernel {
        
        Q_OBJECT
        
        public:
            
            // The name should be unique around the system. 
            // ------------------------------------------------------------------------------------
            // Имя лучше бы было уникальным в пределах системы. 
            
            AbstractAgent ( AbstractAgent * parent, QString systemName );
            virtual ~AbstractAgent();
                        
            // Add "the sprout", reaction to redises messages.
            // Добавить "веточку" реакции на сообщения редиса.
            
            void addSprout( Sprout * sprout );
            
            // Activity channel, especially channel for change agent activity.
            // Установка канала активности, специально выделенного для управления активностью агента.
            
            void setActivityChannel( QString activityChannel );
                        
            
        protected:
            
            QString _activityChannel;            
            
            
        private:
            
            // QList <Sprout * > __sprouts;
            QMap < QString, Sprout * > __sprouts;
            
            // The subscribtion function. Call at the moment subsciber redis connection.            
            // Функция подписки. Вызывается на момент соединения подписчика редиса.
            
            void __subscribe();
            
        private slots:
            
            void __on_subscriber_connected();
            void __on_got_message( QString channel, QString messaage );
            void __on_subscribed( QString channel );
            void __on_unsubscribed( QString channel );
            void __on_activity_channel_message( QVariant value );
            
    };
    
};

