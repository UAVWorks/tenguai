// ********************************************************************************************************************
// *                                                                                                                  *
// *                                             Abstract reguator class.                                             *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                          Класс абстрактного регулятора.                                          *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 24 may 2017 at 10:53 *
// ********************************************************************************************************************

#include <QObject>
#include <QTimer>
#include <QString>
#include <QTime>
#include <QDebug>
#include <QString>
#include <QSettings>

#include "PID.h"
#include "LoRedis.h"
#include "../onboard_common.h"

namespace tengu {
    class AbstractRegulator : public QObject {
        
        Q_OBJECT
        
        public:
            
            AbstractRegulator( QString section, float min_value, float max_value );
            virtual ~AbstractRegulator();
            
            static QString prefix( QString section );
            
        protected:
            
            // Redis publicator and writer
            // Редис - который публикует и записывает.
            
            LoRedis * __pub_redis;
            
            // Redis subscriber
            // Редис - который подписчик.
            
            LoRedis * __sub_redis;
                        
            QString _section;
            QString _message_prefix;
            
            PID * _pid;            
            
            QString _input_channel;
            float _input_value;
            QString _desired_channel;
            float _desired_value;
            QString _output_channel;
                                              
        private:
            
            bool __pub_redis_connected;
            bool __sub_redis_connected;
            
            QTimer * __ping_timer;
            QTimer * __connect_timer;
            
            bool __active;
            
            void __subscribe();
            
            void __do_step();
                        
        private slots:                                              
            
            void __on_pub_redis_connected();
            void __on_pub_redis_disconnected();
            
            void __on_sub_redis_connected();
            void __on_sub_redis_disconnected();
            
            void __on_error( QString message );
            
            void __on_ping_timer();
            void __on_connect_timer();
            
            void __on_got_message( QString channel, QString message );
    };
};
