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
        
        Q_PROPERTY( QString system_name READ getSystemName WRITE setSystemName );
        Q_PROPERTY( QString human_name READ getHumanName WRITE setHumanName );
        Q_PROPERTY( QString comment READ getComment WRITE setComment );
        Q_PROPERTY( QString uuid READ getUUID );
        Q_PROPERTY( AbstractEntity::execution_mode_t execution_mode READ getExecutionMode WRITE setExecutionMode );

        signals:
            
            void signalSproutGotValue( QString sproutName, QVariant value );
            
        public:
            
            // The name should be unique around the system. 
            // ------------------------------------------------------------------------------------
            // Имя лучше бы было уникальным в пределах системы. 
            
            AbstractAgent ();
            virtual ~AbstractAgent();
                        
            // Add "the sprout", reaction to redises messages.
            // Добавить "веточку" реакции на сообщения редиса.
            
            void addSprout( Sprout * sprout );
            
            // Total sprout's count for this agent.
            // Общее число "ростков" агента.
            
            int sproutsCount();
            
            // Activity channel, especially channel for change agent activity.
            // Установка канала активности, специально выделенного для управления активностью агента.
            
            // void setActivityChannel( QString activityChannel );
                        
            // Is this agent active (i.e. really work right now)?
            // Является ли данный агент активным (то есть на самом деле работает прямо сейчас)?
            
            bool isActive();
            
            // Has this agent a "focus", i.e. it think about possibility to launch?
            // Имеет ли агент "фокус", то есть он думает о возможности запуска?
            
            bool isFocused();
            
            void disconnect();
            
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
            
            QMap < QString, Sprout * > __sprouts;
            
            // The subscribtion function. Call at the moment subsciber redis connection.            
            // Функция подписки. Вызывается на момент соединения подписчика редиса.
            
            void __subscribe();
            
            // The unsubscribtion function.
            // Функция отписки от сообщений.
            
            void __unsubscribe();
            
            // The focus of this channel.
            // Фокус данного канала.
            
            bool __focus;
            
            // Activity of this agent.
            // Активность данного агента.
            
            bool __activity;
            
            // The function which performed to make decision about activity of this channel (before real work)
            // Функция, которая выполняется для решения об установке активности (перед началом реальной работы)
            
            virtual void __tryActivate();
            
        private slots:
            
            void __on_subscriber_connected();
            void __on_got_message( QString channel, QString messaage );
            void __on_subscribed( QString channel );
            void __on_unsubscribed( QString channel );
            
            // void __on_sprout_got_message();
            
            // void __on_sub_redis_connected();        
            // void __on_activity_channel_message( QVariant value );
            
    };
    
};

