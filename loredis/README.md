# LoRedis

[Описание на русском языке](README.ru.md)

Yet another lightweight C ++ wrapper for hiredis, the standard C API library
of a remarkable in-memory data storage [Redis](https://redis.io)

## There are several [C ++ client libraries](https://redis.io/clients#c--) for the redis already. Why need "yet another"?

The purpose of the library is to work as message mechanism in a multi-agent environment. Therefore, there are hard requirements 
for the execution time, no one wants to wait. 

Yes, there are a set of libraries. I began to look at them when I needed to work 
with redis. Some did not work at all, because they did not match the current version of the redis. Some worked and had a very 
interesting implementation, but did not have any documentation nor comments in source code nor the examples. I do not know how 
to work with such texts. 

In the general, eventually I found it easier to write it again. 

Generally speaking, **LoRedis** is the part of Tengu, the Artifical Intelligence scentific research project. But you can use it 
as you wish, under the GPLv3 terms.

## What are the features of this implementation?

- Assynchronism, except for specially reported (stipulated) methods. 
- Implemented as a Qt object, the interaction occurs using the Qt signal/slot mechanism.
- It has the ability to work without a Qt events loop. In those cases where it makes no sense to create QApplication, 
this one can do with libevent alone and event processing based on callback functions. Libevent takes precedence, if the "reactor" 
libevent is installed, then the Qt signals will not be generated.
- It contains 3 source code files only and can be added into any project directly, without using static or dynamic libraries.

## How to build?

To build, you need **libevent-dev** as well as **libhiredis-dev** itself.

The building process is standard for qmake. First you need to compile the library itself ( the **src** directory ).

```
qmake
make
```

You can do if you whish:

```
make install
```

The library will be installed into /usr/local in this case.

Further, if desired, you can do all the same for the **examples** catalog. This is true, there already 
will be installed nothing and make install is not provided, it's just for a look at it or try it.

## How to use?

```C++
// First, you need to create an object and connect yourself handlers with him signals.
LoRedis * redis = new LoRedis();
QObject::connect( redis, SIGNAL( signalConnected() ), this, SLOT( onConnected()) );
QObject::connect( redis, SIGNAL( signalDisconnected() ), this, SLOT( onDisconnected()) );
QObject::connect( redis, SIGNAL( signalError(QString) ), this, SLOT( onError(QString)) );

// Connect another existing signals, see src/LoRedis.h and/or examples/RedisTester.cpp
// QObject::connect( ... ); 

// The connect() function starts internal event handler loop
redis->connect();

// Then you can do something with him. Do not forget, the most functions are asynchronously,
// they return nothing. You should waits an result in the slots instead. 

redis->set( "VariableName", "VariableValue" );
redis->publish( "ChannelName", "Text of message" );

// Syncronously variant. It return a result right here. But the signal will be
// generated (emitted) in any case.

QVariant val = redis->get_sync("VariableName");

```
Using with a "reactor" class and his callback-functions (without Qt). 

```C++
#include "LoRedisReactor.h"
class MyClass : public LoRedisReactor {
    MyClass::MyClass() {
        LoRedis * redis = new LoRedis( this );
    };
};

```
