// ********************************************************************************************************************
// *                                           Yet another C++ wrapper for hiredis                                    *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                             Еще одна C++ - обертка для редиса.                                   *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       created 21 apr 2017 at 16:12 *
// ********************************************************************************************************************

#pragma once

// system includes
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include "hiredis/hiredis.h"
#include "hiredis/async.h"
#include "hiredis/adapters/libevent.h"

#include <iostream>
#include <thread>

// qt includes

#include <QObject>
#include <QDebug>
#include <QString>
#include <QList>
#include <QThread>
#include <QMutex>
#include <QVariant>


// My include files.
// Мои инклудники.

#include "LoRedisReactor.h"

/**
 * @short Qt-based Redis C++ API.
 * 
 * LoRedis is C++ Qt and libevent based anynchronous class for working with redis using a hiredis,
 * an official C Redis API. LoRedis performs almost all operations in an asynchronous manner, 
 * using the Qt signal / slot mechanism Qt. It use libevent for internal working with redis.
 * 
 * LoRedis - это обертка над hiredis, официальным C API клиентом хранилища Redis IO. LoRedis 
 * базируется на C++ Qt и libevent. бОльшая часть операций выполняется ассинхронно, если иное
 * не оговорено специально. 
 * 
 * @see <a href="https://redis.io/">Redis, an open source (BSD licensed), in-memory data structure 
 * store, used as a database, cache and message broker. </a>
 * 
 * @author Eugene G. Sysoletin <unclesal@mail.ru>, Ural Federal University, the Institute of 
 * radioelectronics and information tehnology, IT department. 
 * 
 * @date 24 apr 2017 
 */

class LoRedis : public QObject {
    
    friend class __LoRedis;
        
    Q_OBJECT
    
    signals:
        
        /**
         * @short The signal that we are connected to the redis.
         * 
         * Сигнал о том, что мы соединились с редисом.
         */
        
        void signalConnected();
        
        /**
         * @short We are disconnected from redis.
         * 
         * Мы рассоединились с редисом.
         */
        
        void signalDisconnected();
        
        /**
         * @short We have an error internal of the redis.
         * 
         * У нас есть внутренняя ошибка редиса.
         * 
         * @param message Text message which describes occured error.
         */
        
        void signalError( QString message );
        
        /**
         * @short We have got the "raw" ( or "crude" ) text message.
         * 
         * We have received a pub/sub message from a specific channel. At a higher level, 
         * messages can be converted to other types of data. But they come from a redis 
         * in a string form anyway.
         * 
         * Мы получили pub/sub сообщение из указанного канала. На более высоком уровне
         * в качестве "сообщений" могут выступать другие типы данных, но на самом
         * деле от редиса мы по-любому полуучаем текстовую строку.
         * 
         * @param channel Name of the channel we are subscribed too.
         * @param message Text representation of message.
         */
        
        void signalGotMessage( QString channel, QString message );
        
        /**
         * @short We have subscribed to the channel.
         * 
         * Мы подписались на канал.
         */
        
        void signalSubscribed( QString channel );
        
        /**
         * @short We are unsubscribed from the channel.
         * 
         * Мы отписались от канала.
         */
        
        void signalUnsubscribed( QString channel );
        
        /**
         * @short We have got value from redis.
         * 
         * Мы получили значение переменной из редиса.
         */
        
        void signalGotValue( QString varuableName, QVariant variableValue );
        
        /**
         * @short signal after SET command.
         * 
         * Сигнал, который вырабатывается после GET команды.
         */
        
        void signalSetted( int status );
    
    public:
        
        /**
         * @short Emtpy constructor.
         * 
         * Constructor does not connect automatically, just initialize the class. 
         * You need to call connect() explicitely.
         * 
         * Пустой конструктор. Ничего не делает, просто инициализирует сам класс.
         * Вам все еще нужно вызвать connect() в явном виде.
         */
        
        LoRedis();
        
        /**
         * @short Constructor with low-level event reactor.
         * 
         * Конструктор с низко-уровневым "реактором" на события.
         */
        
        LoRedis( LoRedisReactor * reactor );
        
        /**
         * @short Desctructor.
         */
        
        ~LoRedis();
        
        /**
         * @short Connect to redis server.
         * 
         * The connection process is asynchronous. You will get a signalConnected or signalError after
         * connection depending on result.
         * 
         * Процесс соединения - ассинхронный. Вы получите signalConnected или signalError - в зависимости
         * от результата. Но получите Вы их - не сейчас, а после того, как соединение пройдет фактически.
         */
        
        void connect( QString host = "127.0.0.1", int port = 6379 );
        
        /**
         * @short set the value assynchronously (normally).
         * 
         * Return nothing. signalSetted will be generated after really execution.
         * 
         * Ничего не возвращает. signalSetted будет сгенерирован после того, как установка действительно выполнится.
         */
        
        void set( QString name, QVariant data );
        
        /**
         * @short Set the value - synchronously.
         * 
         * signalSetted still generated. Do not use this function if you think 
         * about execution time. It will be waiting the actual answer.
         * The method does not return anything. If there is an error, a signalError 
         * will be generated.
         * 
         * Синхронный метод установки значения переменной. signalSetted вырабатывается в любом
         * случае, хоть при синхронном, хоть при ассинхронном вызове. Данный метод ничего
         * не возвращает, но вы получите signalError, если будут ошибки.
         * 
         * @see signalError
         */
        
        void set_sync( QString name, QVariant value );
        
        /**
         * @short Get the value - asynchronously ("normally").
         * 
         * Получить значение - ассинхронно (то есть "нормально"). Функция не возвращает ничего, вместо
         * этого будет сгенерирован сингал signalGotValue на момент фактического получения значения.
         * 
         * @return nothing. signalGotValue will be emitted.
         */
        
        void get( QString name );
        
        /**
         * @short Get the value - synchronously.
         * 
         * Получить значение переменной - синхронно (то есть будет висеть в этой функции до тех пор,
         * пока не получит либо значение, либо ошибку).
         */
        
        QVariant get_sync( QString name );
                
        /**
         * @short Publish text message to the definite channel.
         * 
         * Опубликовать сообщение в указанном канале.
         * 
         * @param channel Name of the channel where message will be published.
         * @param message Published message.
         */

        void publish( QString channel, QString message );
        
        /**
         * @short Subscribe to specific channel.
         * 
         * Redis does not allow you to do anything else if there was a subscription in this connection. 
         * Remember this. If you need something else - open several connections with redis (by creating 
         * different objects of this class).
         * 
         * Подписаться на получение сообщений из указанного канала. Редис не позволяет делать что-нибудь
         * еще, если в данном соединении произошла подписка. Помните об этом. Если нужно что-то еще -
         * откройте несколько соединений с редисом (путем создания разных объектов данного класса).
         */
        
        void subscribe( QString channel );
        
        // Unsubscribe for specific redis channel. Without parameters - from all channels we has been subscribed to.
        // Отписаться от определенного канала редиса. Без параметров - от всех каналов, на которые была подписка.
        
        void unsubscribe( QString channel = QString("") );

        
        /**
         * @short Disconnect from Redis server.
         * 
         * Function returns nothing. Will be generated signalDisconnected if disconect
         * process was successfull.
         * 
         * Рассоединиться с редисом. Функция ничего не возвращает, вместо этого будет сгенерирован
         * сигнал signalDisconnected на момент фактического рассоединения (если оно, конечно,
         * прошло успешно).
         */
        
        void disconnect();
        
        // Currently, there is no separate thread for processing events. This function processes the 
        // event's queue in all existing LoRedis instances simultaneously.
        
        // В настоящее время отдельного потока для обработки событий больше нет. Данная функция обрабатывает
        // имеющиеся в очереди события - во всех экземплярах LoRedis'а.
        
        static void processEvents();
        
    protected:
        
        redisAsyncContext * _asyncContext;
    
    private:
        
        static QList< LoRedis * > __redisers;
        
        struct event_base * __event_base;
        
        /**
         * @short Return "the owner" by his hiredis asyncContext.
         * 
         * Search object in objects collection and return it if found.
         */
        
        static LoRedis * __getRediser( const redisAsyncContext * ctx );
        
        /**
         * @short QThread run() realization.
         * 
         * This is endless cycle.
         */
        
        // void run();
        
        /**
         * @short The flag indicates that the data in the redis was actually setted.
         */
        
        volatile bool __setted;
        
        /**
         * @short this flag indicates that the data was getted from redis.
         */
        
        volatile bool __getted;
        
        /**
         * @short Value which has been got from redis.
         * 
         * For synchronously GET operation.
         */
        
        QVariant __gettedValue;
        
        /**
         * @short Reactor for non-Qt (libevent and pthread) variant.
         * 
         * "Реактор" для не-Qt-шного варианта работы (через libevent / pthread ).
         */
        
        LoRedisReactor * __reactor;
        
        // Libevent loop mutex
        // Мутекс для цикла libevent
        
        QMutex __eMutex;
};

