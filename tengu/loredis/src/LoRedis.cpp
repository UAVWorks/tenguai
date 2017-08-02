// ********************************************************************************************************************
// *                                                                                                                  *
// *                                           Yet another C++ wrapper for hiredis.                                   *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                             Еще одна C++ - обертка для редиса.                                   *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       created 21 apr 2017 at 16:14 *
// ********************************************************************************************************************

#include "LoRedis.h"

QList<LoRedis * > LoRedis::__redisers;

// extern void xplugin__log( QString message );

// ====================================================================================================================
//
// 
//                               The Wrapper class, for access to private LoRedis fields
// --------------------------------------------------------------------------------------------------------------------
//                                  Надстоящий класс для доступа к приватным полям. 
//
// @todo It may be better to make all of these functions hidden in the object file.
// ====================================================================================================================

class __LoRedis {
        
    public:
        
        /**
         * @short Structure for get callback handler.
         * 
         * It is useful to have along with value also the name of this variable.
         */
        
        struct named_redis_t {
            QString variableName;
            LoRedis * redis;
        };
        
        /**
         * @short Find the LoRedis class in the list of objects by its redisAsyncContext.
         * 
         * @param cts pointer to hiredis redisAsyncContext value. We will search for an LoRedis object 
         * by the correspondence of this value.
         */
        
        static LoRedis * getRediser ( const redisAsyncContext * ctx ) {
    
            if ( ctx ) {
                for ( int i=0; i< LoRedis::__redisers.size(); i++ ) {
                    LoRedis * r = LoRedis::__redisers.at( i );
                    if ( r->_asyncContext == ctx ) {
                        return r;
                    };
                }
            }
            
            return nullptr;
        } 
        
        /**
         * @short Someone was connected.
         * 
         * The drawback of the redis connectCallback callback is that there is no way to 
         * pass a pointer to the calling object as a parameter. Thus, I do a pass-around via 
         * private (hidden) fields.
         * 
         * @param ctx pointer to redisAsyncContext, value from hiredis, where connection was happen.
         */
        
        static void someoneConnected( const redisAsyncContext * ctx ) {
                        
            LoRedis * rediser = getRediser( ctx );
            if ( rediser ) {
                
                if ( rediser->__reactor ) {
                    rediser->__reactor->cbConnected( rediser );                
                } else {
                    rediser->emit signalConnected();
                };
            };
            
        };
        
        /**
         * @short Someone from our clients was disconnected.
         * 
         * @param ctx pointer to hiredis redisAsyncContext where connection was broken.
         */
        
        static void someoneDisconnected( const redisAsyncContext * ctx ) {
            LoRedis * rediser = getRediser( ctx );
            if ( rediser ) {
                
                if ( rediser->__reactor ) {
                    rediser->__reactor->cbDisconnected( rediser );
                } else {
                    rediser->emit signalDisconnected();
                };
            };
        };
        
        /**
         * @short Exactly this LoRedis element have an error.
         */
        
        static void hiHasError( LoRedis * rds, QString error ) {
            
            if ( rds->__reactor ) {
                rds->__reactor->cbError( rds, error );
            } else {
                rds->emit signalError( error );
            };
        };
        
        /**
         * @short Someone from our clients has an error.
         * 
         * @param ctx pointer to hiredis redisAsyncContext value where error was occured.
         * @param error Text representation of occured error.
         */
        
        static void someoneHasError( const redisAsyncContext * ctx, QString error ){
            
            LoRedis * rediser = getRediser( ctx );
            if ( rediser ) {
                
                if ( rediser->__reactor ) {
                    rediser->__reactor->cbError( rediser, error );
                } else {
                    rediser->emit signalError( error );
                };
            };

        };
        
        /**
         * @short Emit error signal from all existing LoRedis object.
         * 
         * It is necessary to inform all objects in memory if a serious error has occurred.
         */
        
        static void anyoneHasError( QString error ) {
            
            for ( int i=0; i<LoRedis::__redisers.size(); i++ ) {
                LoRedis * rediser = LoRedis::__redisers.at(i);
                
                if ( rediser->__reactor ) {
                    rediser->__reactor->cbError( rediser, error );
                } else {
                    rediser->emit signalError( error );
                };
            };
        };
        
        /**
         * @short We has been subscribed to specific channel.
         */
        
        static void subscribed( LoRedis * rds, QString channel ) {
            if ( rds ) {
                
                if ( rds->__reactor ) {
                    rds->__reactor->cbSubscribed( rds, channel );
                } else {
                    rds->emit signalSubscribed( channel );
                };
            }
        };
        
        
        /**
         * @short Flag after SET synchronously redis command.
         */
        
        static void setted( LoRedis * redis ) {
            if ( redis ) redis->__setted = true;
        };
        
        /**
         * @short There was correctly setted value.
         */
        
        static void setted( LoRedis * rds, int status ) {
            
            if ( rds ) {
                
                rds->__setted = true;
                
                if ( rds->__reactor ) {
                    rds->__reactor->cbSetted( rds, status );
                } else {
                    rds->emit signalSetted( status );
                };
            }
                        
        };
        
        /**
         * @short flag after GET synchronously command.
         */
        
        static void getted( LoRedis * redis, QVariant result ) {
            
            if ( redis ) {
                redis->__gettedValue = result;
                redis->__getted = true;                
            };
        };
        
        static void getted( LoRedis * redis, QString channel, QVariant value ) {
            
            __LoRedis::getted( redis, value );
            
            if ( redis ) {
                
                if ( redis->__reactor ) {
                    redis->__reactor->cbGotValue( redis, channel, value );                
                } else {
                    redis->emit signalGotValue( channel,  value );
                };
            }
            
        };
        
        static void gotMessage( LoRedis * rds, QString channel, QString message ) {
            
            if ( rds ) {
                
                if ( rds->__reactor ) {
                    rds->__reactor->cbGotMessage( rds, channel, message );
                } else {
                    rds->emit signalGotMessage( channel, message );
                };
            }
            
        };
        
        // We are unsubscribed from specific redis channel.
        // Мы отписались от указанного канала редиса.
        
        static void unsubscribed ( LoRedis * rds, QString channel ) {
            
            if ( rds ) {
                
                if ( rds->__reactor ) {
                    rds->__reactor->cbUnsubscribed( rds, channel );
                } else {
                    rds->emit signalUnsubscribed( channel );
                };
            }            
        };                
    
};


// ====================================================================================================================
//
//
//             The callback functions in global namespace for asynchronous interactions with redis database.
//
//
// ====================================================================================================================



// ********************************************************************************************************************
// *                                                                                                                  *
// *                      Callback-function, performed at the time of connection with the redis.                      *
// *                                                                                                                  *
// ********************************************************************************************************************

void __cb_connect(const redisAsyncContext * ctx, int status ) {
        
    if (status != REDIS_OK) {
        qDebug() << "Rediser connect error: " << ctx->errstr;
        QString error( ctx->errstr) ;
        __LoRedis::someoneHasError( ctx, error );
        return;
    };
    
    __LoRedis::someoneConnected( ctx );
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                callback-function, performed at the disconnection moment.                         *
// *                                                                                                                  *
// ********************************************************************************************************************

void __cb_disconnect( const redisAsyncContext * ctx, int status) {
    
    if (status != REDIS_OK) {
        
        QString error( ctx->errstr );
        __LoRedis::someoneHasError( ctx, error );
        return;
        
    }
    
    __LoRedis::someoneDisconnected( ctx );
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                         callback-function, performed after publish the message in the channel.                   *
// *                                                                                                                  *
// ********************************************************************************************************************

void __cb_published( redisAsyncContext * ctx, void * vreply, void * privdata ) {
    
    redisReply * reply = static_cast< redisReply * >( vreply );
    LoRedis * rds = static_cast< LoRedis * > ( privdata );
    if ( reply )  {
        
        if ( reply->type == REDIS_REPLY_ERROR ) {
            
            QString error( reply->str );
            
            if ( rds ) {
                
                // Here we have a more correct option than to go 
                // through all the existing objects in the loop.
                
                __LoRedis::hiHasError( rds, error );
                                
            } else __LoRedis::someoneHasError( ctx, error );
            
        } else if ( reply->type == REDIS_REPLY_INTEGER ) {
            
            // If reply type is number, this is is a number of
            // clients who has actually received this message.
            
            // qDebug() << "Сообщение получили " << reply->integer << " клиентов";
            
        } else {
            
            // For debugging purposes only, not affect on programm execution.
            
            qDebug() << "__cb_published(): unhandled reply type " << reply->type << ", msg=" << reply->str;
        };
    };           
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *        callback-function which performed on the moment receiving message in the channel we are subscribed to     *
// *                                                                                                                  *
// ********************************************************************************************************************

void __cb_subscribed( redisAsyncContext * ctx, void * vreply, void * privdata ) {
    
    redisReply * reply = static_cast< redisReply * >( vreply );
    LoRedis * rds = static_cast< LoRedis * > ( privdata );
    if ( reply ) {
        
        if ( reply->type == REDIS_REPLY_ERROR ) {
            
            QString errMessage( reply->str );
            
            if ( rds ) __LoRedis::hiHasError( rds, errMessage );                
            else __LoRedis::someoneHasError( ctx, errMessage );
            
        } else if ( reply->type == REDIS_REPLY_ARRAY ) {
            
            if ( reply->elements == 3 ) {
                
                QString firstElement( reply->element[ 0 ]->str );
                QString secondElement( reply->element[ 1 ]->str );                
                QString thirdElement( reply->element[ 2 ]->str );
                
                if ( firstElement.toUpper() == "SUBSCRIBE" ) {
                    
                    __LoRedis::subscribed( rds, secondElement );
                    
                } else if ( firstElement.toUpper() == "MESSAGE" ) {
                    
                    __LoRedis::gotMessage( rds, secondElement, thirdElement );
                                        
                } else if ( firstElement.toUpper() == "UNSUBSCRIBE" ) {
                    __LoRedis::unsubscribed( rds, secondElement );
                }
                
            } else {
                
                QString errMessage = QString( "__cb_subscribed, parameter error. Got ") + QString::number( reply->elements ) + QString( ", but want exactly 3." );
                if ( rds ) __LoRedis::hiHasError( rds, errMessage );
                else __LoRedis::someoneHasError( ctx, errMessage );
                
            }
            
        } else {
            
            qDebug() << "cb_subscribed(): Got reply type=" << reply->type;
            
        };
    }        
    
}


// ********************************************************************************************************************
// *                                                                                                                  *
// *                            callback-function for handle fatal libevent errors.                                   *
// *                                                                                                                  *
// ********************************************************************************************************************

/*
void __cb_event_fatal( int err ) {
    
    // This is a serious error. Here we inform all LoRedis objects 
    // about the error that occurred.
    
    QString errorString = QString( "LoRedis event fatal error: " ) + QString::number( err );
    errorString += QString(", errMesssage=") + QString( strerror( err ) );
    LoRedisWrapper::anyoneHasError( errorString );
};
*/

// ********************************************************************************************************************
// *                                                                                                                  *
// *                               callback-function which performed after SET command.                               *
// *                                                                                                                  *
// ********************************************************************************************************************

void __cb_setted( redisAsyncContext * ctx, void * vreply, void * privdata ) {
    
    redisReply * reply = static_cast< redisReply * >( vreply );
    LoRedis * rds = static_cast< LoRedis * > ( privdata );
    __LoRedis::setted( rds );
    
    if ( reply ) {
        
        if ( reply->type == REDIS_REPLY_ERROR ) {
            // We have an error.
            QString errorMessage = QString("__cb_setted:") + QString( reply->str );
            
            if ( rds ) __LoRedis::hiHasError( rds, errorMessage ); 
            else __LoRedis::someoneHasError( ctx, errorMessage );
                
        } else  if ( reply->type == REDIS_REPLY_STATUS ) {
            
            __LoRedis::setted( rds, reply->integer );
                            
        } else {
            qDebug() << "__cb_setted type=" << reply->type;
        }
        
    }
        
};

// ********************************************************************************************************************
// *                                                                                                                  *
// *                               callback-function which performed after GET command.                               *
// *                                                                                                                  *
// ********************************************************************************************************************

void __cb_got_value( redisAsyncContext * ctx, void * vreply, void * privdata ) {
    
    redisReply * reply = static_cast< redisReply * >( vreply );
    __LoRedis::named_redis_t * named_redis_ptr = static_cast< __LoRedis::named_redis_t * > ( privdata );
    
    if ( reply ) {
        
        if ( reply->type == REDIS_REPLY_ERROR ) {
            
            QString errorMessage = QString("__cb_got_value(): ") + QString( reply->str );
            if ( named_redis_ptr ) {
                __LoRedis::hiHasError( named_redis_ptr->redis, errorMessage );
            }
            else __LoRedis::someoneHasError( ctx, errorMessage );
                            
            __LoRedis::getted( named_redis_ptr->redis, QVariant() );
            
        } else if ( reply->type == REDIS_REPLY_STRING ) {            
            
            // This is the string value.
            
            QVariant result( reply->str );
            if ( named_redis_ptr ) {
                // To copy QString from structures which will be deleted soon.
                QString vName = named_redis_ptr->variableName;
                __LoRedis::getted( named_redis_ptr->redis, vName, result );                                
            }
            
        } else if ( reply->type == REDIS_REPLY_NIL ) {
            
            // We have got the NULL value.
            // Мы получили значение NULL.
            
            qDebug() << "LoRedis::__cb_got_value, got nill, ";
            if ( named_redis_ptr ) {
                QString vName = named_redis_ptr->variableName;
                qDebug() << "-- got null, vName=" << vName;
            }
            
        } else {
            __LoRedis::getted( named_redis_ptr->redis, QVariant() );
            qDebug()<< "__cb_got_value, type=" << reply->type;
        }
    }
    
    // We have created named_redis_t structure in the memory using malloc().
    // We should free this memory.
    
    if ( named_redis_ptr ) {
        delete( named_redis_ptr );
        named_redis_ptr = nullptr;
    }
    
    
}

// ====================================================================================================================
//
//                                                LoRedis class itself. 
// --------------------------------------------------------------------------------------------------------------------
//                                              Собственно класс LoRedis'а.
//
// ====================================================================================================================


// ********************************************************************************************************************
// *                                                                                                                  *
// *                                               The constructor.                                                   *
// *                                                                                                                  *
// ********************************************************************************************************************

LoRedis::LoRedis() 
    : QObject()    
{     
    __reactor = nullptr;
    __event_base = nullptr;
    __setted = false;
    __getted = false;
    
    _asyncContext = nullptr;
                
    __redisers.append( this );
    
    // setTerminationEnabled( true );    
    // We will ignore SIGPIPE signal from operation system.        
    signal(SIGPIPE, SIG_IGN);            
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                    The constructor with low-level event reactor                                  *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                 Конструктор с низкоуровневым реактором на события.                               *
// *                                                                                                                  *
// ********************************************************************************************************************

LoRedis::LoRedis( LoRedisReactor * reactor ) 
    : LoRedis()
{
    __reactor = reactor;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                             Connect to Redis server.                                             *
// *                                                                                                                  *
// ********************************************************************************************************************

void LoRedis::connect( QString host, int port ) {
    
    qDebug() << "LoRedis::connect()";
    
    if ( ! _asyncContext ) {
        
        __event_base = event_base_new();
        
        // event_set_fatal_callback( __cb_event_fatal );
        
        _asyncContext = redisAsyncConnect( host.toLatin1().constData(), port );
        
        if ( _asyncContext->err) {
            QString error(_asyncContext->errstr);
            redisAsyncFree( _asyncContext );
            _asyncContext = nullptr;
            __LoRedis::hiHasError( this, error );
            return;
        }
        
        redisLibeventAttach( _asyncContext, __event_base );
        
        redisAsyncSetConnectCallback( _asyncContext, __cb_connect );
        redisAsyncSetDisconnectCallback( _asyncContext, __cb_disconnect );

        // qDebug() << "Call event_base_dispatch...";
        //event_base_dispatch( __event_base );
        
        // redisAsyncCommand(c, NULL, NULL, "SET key %b", argv[argc-1], strlen(argv[argc-1]));
        // redisAsyncCommand(c, getCallback, (char*)"end-1", "GET key");
        
    } else __LoRedis::hiHasError( this, QString("LoRedis: Called connect() whereas connection exists already.") );
        
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                      Switch off connection with Redis-server.                                    *
// *                                                                                                                  *
// ********************************************************************************************************************

void LoRedis::disconnect() {
        
    
    if ( _asyncContext ) {          
                                              
        if ( __event_base ) {
            
            event_base_loopexit( __event_base, NULL );            
            event_base_free( __event_base );
            
            __event_base = nullptr;
            
            // this->terminate();
            // this->wait( 200 );            
    
        };
        
        // redisAsyncDisconnect( _asyncContext );   
        redisAsyncFree( _asyncContext );
        _asyncContext = nullptr;        
        
    } else __LoRedis::hiHasError( this, QString("LoRedis::disconnect() Called disconnect() function without having context initialization.") );
            
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                       Execution metod of QThread descendant. Handling libevent message loop.                     *
// *                                                                                                                  *
// ********************************************************************************************************************

//void LoRedis::run() {
//    
//    xplugin__log( QString("LoRedis::run() started.") );
//            
//    if ( __event_base ) {        
//        
//        event_base_dispatch( __event_base );
//        
//        /*
//        for ( ;; ) {
//            event_base_loop( __event_base, EVLOOP_ONCE | EVLOOP_NONBLOCK );
//            this->usleep( 2 );
//        };
//        */
//    };
//            
//    xplugin__log( QString("!!! LoRedis::run() finished!!!" ) );
//        
//}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                   Publish text message in the specific channel                                   *
// *                                                                                                                  *
// ********************************************************************************************************************

void LoRedis::publish( QString channel, QString message ) {
    
    if ( _asyncContext ) {
        
        QString cmd = QString("PUBLISH %s %s");
                    
        redisAsyncCommand( _asyncContext, __cb_published, this, 
            cmd.toLatin1().data(), 
            channel.toLocal8Bit().data(),
            message.toLocal8Bit().data()                 
        );
                
    } else __LoRedis::hiHasError( this, QString("LoRedis::publish( channel, message): Attempt to publish message without having context initialization.") );
                    
}


// ********************************************************************************************************************
// *                                                                                                                  *
// *                                 Set the value in redis asynchronously (normally).                                *
// *                                                                                                                  *
// ********************************************************************************************************************

void LoRedis::set( QString name, QVariant value ) {
    
    if ( _asyncContext ) {
        
        QString cmd = QString("SET ") + name + QString(" %s");
        QString str_value = value.toString();
        redisAsyncCommand( _asyncContext, __cb_setted, this, cmd.toLocal8Bit().data(), str_value.toLocal8Bit().data() );        
        
    } else __LoRedis::hiHasError( this, QString("LoRedis::set(): attempt to set the value without having context initialization.") );
        
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                        Set the value in redis synchronously.                                     *
// *                                                                                                                  *
// ********************************************************************************************************************

void LoRedis::set_sync ( QString name, QVariant value ) {
    
    if ( _asyncContext ) {
        __setted = false;
        set( name, value );
        
        // usleep to avoid CPU usage.
        // Чутка спим, чтобы не загружать процессор.
        
        while ( ! __setted ) { 
            LoRedis::processEvents();
            usleep( 3 );
        };
        
        
        /*
          
         Does not work, will return empty reply.
          
        redisReply * reply = ( redisReply * ) redisCommand( & _asyncContext->c, cmd.toLatin1().constData(), value.toString().toLocal8Bit().constData() );
        
        if ( reply ) {
            
            if ( reply->type == REDIS_REPLY_ERROR ) {
                QString errorMessage = QString("LoRedis::set_sync(): ") + QString( reply->str ) ;
                emit signalError( errorMessage ); 
            };
            freeReplyObject( reply );
            
        } else emit signalError("LoRedis::set_sync(): got empty reply from redis.");
        */
        
    } else __LoRedis::hiHasError( this, QString("LoRedis::set_sync(): attempt to set value without having context initialization.") );
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                      Get the value from redis asynchronously.                                    *
// *                                                                                                                  *
// ********************************************************************************************************************

void LoRedis::get ( QString name ) {
    
    if ( _asyncContext ) {
        
        // We created this structure because it need exists all time, until it will not be handled.
        // We will free memory in callback function.
        
        __LoRedis::named_redis_t * named_redis_ptr = new __LoRedis::named_redis_t ();
        
        // Copying the parameters to structure because execution of this block will ended soon.
        
        named_redis_ptr->variableName = name;
        named_redis_ptr->redis = this;
        
        QString cmd = QString("GET ") + name;
        redisAsyncCommand( _asyncContext, __cb_got_value, named_redis_ptr, cmd.toLocal8Bit().data() );
        
    } else __LoRedis::hiHasError( this, QString("LoRedis::get(): attempt to get value without having context initialization.") );    
    
}


// ********************************************************************************************************************
// *                                                                                                                  *
// *                                     Get the value from redis synchronously.                                      *
// *                                                                                                                  *
// ********************************************************************************************************************

QVariant LoRedis::get_sync ( QString name ) {
    
    QVariant result;
    
    if ( _asyncContext ) {
        __getted = false;
        get( name );
        while ( ! __getted ) { 
            LoRedis::processEvents();
            usleep( 3 );      
            
        };
        result = __gettedValue;
        
        /*
        
        -- does not work, Reply will be empty.
        
        // QString cmd = QString("GET ") + name;
        redisReply * reply =  ( redisReply * ) redisCommand( & _asyncContext->c, cmd.toLatin1().constData() );
        if ( reply ) {
            if ( reply->type == REDIS_REPLY_ERROR ) {
                
                QString errorMessage = QString("LoRedis::get_sync(): ") + QString( reply->str ) ;
                emit signalError( errorMessage );
                
            } else if ( reply->type == REDIS_REPLY_STRING ) {
                result = QVariant::fromValue<QString>( QString(reply->str) );
            } else {
                qDebug() << "LoRedis::get_sycn(), got reply type=" << reply->type;
            }
            freeReplyObject( reply );
            
        } else {
            emit signalError("LoRedis::get_sync(): get command return nothing");
        };
        */
    } else __LoRedis::hiHasError( this, QString("LoRedis::get_sync(): attempt to get value without having context initialization. " ) );    
    
    return result;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                       Subscribe to the specific channel                                          *
// *                                                                                                                  *
// ********************************************************************************************************************

void LoRedis::subscribe ( QString channel ) {
    
    if ( _asyncContext ) {
        QString cmd = QString("SUBSCRIBE %s");
        redisAsyncCommand( _asyncContext, __cb_subscribed, this, cmd.toLocal8Bit().data(), channel.toLocal8Bit().data() );
    } else __LoRedis::hiHasError( this, QString( "LoRedis::subscribe(): context was not initialized." ) );
            
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                        unsubscribe from specific channel                                         *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                         Отписаться от указанного канала.                                         *
// *                                                                                                                  *
// ********************************************************************************************************************

void LoRedis::unsubscribe ( QString channel ) {
    
    if ( _asyncContext ) {
        
        
        QString cmd = QString("UNSUBSCRIBE ");
        if ( channel.length() ) {
            cmd += channel;
        };
                
        redisAsyncCommand( _asyncContext, __cb_subscribed, this, cmd.toLocal8Bit().data() );
        
        /*
        redisContext c = _asyncContext->c;
        qDebug() << "LoRedis::unsubscribe, got context.";
                        
        void * vReply = redisCommand( & c, "%s", cmd.toLocal8Bit().data() );
        qDebug() << "RedisCommand unsubscribe done, got reply " << vReply;
        if ( vReply ) {
            redisReply * reply = (redisReply * ) vReply;
            qDebug() << "In reply type=" << reply->type;
            freeReplyObject( vReply );
        };
        */
                
    } else __LoRedis::hiHasError( this, QString("LoRedis::unsubscribe(), context was not initialized") );
        
    
}


// ********************************************************************************************************************
// *                                                                                                                  *
// *                                              Process event's queue.                                              *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                             Обработка очереди событий.                                           *
// *                                                                                                                  *
// ********************************************************************************************************************

void LoRedis::processEvents() {
    
    for ( int i=0; i<__redisers.length(); i++ ) {
        
        LoRedis * redis = __redisers.at(i);
        if ( redis->__event_base ) {
            
            bool locked = redis->__eMutex.tryLock( 50 );
            if ( locked ) {
                event_base_loop( redis->__event_base, EVLOOP_NONBLOCK );
                redis->__eMutex.unlock();
            }
        }
    };
    
}


// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                 The desctructor.                                                 *
// *                                                                                                                  *
// ********************************************************************************************************************

LoRedis::~LoRedis() {

    if ( _asyncContext )  disconnect();
        
    __redisers.removeOne( this );        
    
    // if ( _asyncContext ) redisAsyncFree( _asyncContext );
    

}


