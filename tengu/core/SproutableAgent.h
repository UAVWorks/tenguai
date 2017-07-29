// ********************************************************************************************************************
// *                                                                                                                  *
// *                                           The agent which can have a sprouts.                                    *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                          Агент, который может иметь "отростки".                                  *
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

#include "Sprout.h"

namespace tengu {

    class SproutableAgent : public tengu::AbstractAgent {
        
        Q_OBJECT
                
        signals:
            
            void signalSproutGotValue( QString sproutName, QVariant value );
            
        public:
            
            // The name should be unique around the system. 
            // ------------------------------------------------------------------------------------
            // Имя лучше бы было уникальным в пределах системы. 
            
            SproutableAgent ();
            virtual ~SproutableAgent();
                        
            // Add "the sprout", reaction to redises messages.
            // Добавить "веточку" реакции на сообщения редиса.
            
            void addSprout( tengu::Sprout * sprout );
            void removeSprout( tengu::Sprout * sprout );
            
            // Total sprout's count for this agent.
            // Общее число "ростков" агента.
            
            int sproutsCount();
            QList<Sprout * > sprouts();
            
            // Activity channel, especially channel for change agent activity.
            // Установка канала активности, специально выделенного для управления активностью агента.
            
            // void setActivityChannel( QString activityChannel );
                        
            // Get sprout list suitable (connectable) for specified sprout
            // Получить лист sprout'ов, подходящих (соединябельных) с представленным sprout'ом.
            
            QList< tengu::Sprout * > sutiableSproutsFor( tengu::Sprout * sp );
            
            void disconnect();
            
            virtual QJsonObject toJSON() override;
            virtual bool fromJSON( QJsonObject json ) override;
            
        protected:
            
            // Output channel name to display real activity of this agent.
            // Имя выходного канала для отображения реальной активности агента.
            
            QString _oActivityChannel;
            
            // Input channel name for setting focus for this agent.
            // Имя входного канала для установки фокуса данного агента.
            
            QString _iFocusChannel;
            
            
            // virtual void _setActivity( bool a );                        
            
            
        private:
            
            /**
             * @short Sprouts of this agent.
             * 
             * The "Sprouts" of this agent, communication with an environment.
             * "Ростки" данного агента, связь с окружающей средой.
             */
            
            QMap < QString, tengu::Sprout * > __sprouts;
            
            void __deleteSprouts();            
                        
            // The subscribtion function. Call at the moment subsciber redis connection.            
            // Функция подписки. Вызывается на момент соединения подписчика редиса.
            
            void __subscribe();
            
            // The unsubscribtion function.
            // Функция отписки от сообщений.
            
            void __unsubscribe();
                                    
            
        private slots:
            
            void __on_subscriber_connected();
            void __on_got_message( QString channel, QString messaage );
            void __on_subscribed( QString channel );
            void __on_unsubscribed( QString channel );
            void __on__sprout_removed( Sprout * sprout );
            
            // void __on_sprout_got_message();
            
            // void __on_sub_redis_connected();        
            // void __on_activity_channel_message( QVariant value );
            
    };
    
};

