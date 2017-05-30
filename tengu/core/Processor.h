// ********************************************************************************************************************
// *                                                                                                                  *
// *              "A processor". An agent that runs on a real hardware (computer, board) and that can manage          *
// *                                                 other agents.                                                    *
// * ---------------------------------------------------------------------------------------------------------------- *
// *     "Процессор". Агент, который выполняется на реальном железе (компьютере, плате) и который может управлять     *
// *                                                другими агентами.                                                 *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 29 may 2017 at 11:03 *
// ********************************************************************************************************************

#pragma once

#include <QFile>
#include <QSettings>
#include <QCoreApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>

#include "AbstractAgent.h"

#define PROCESSOR_CONFIG_FILE   "/etc/tengu/processor.conf"

namespace tengu {
    
    class Processor : public AbstractAgent {
        
        Q_OBJECT
        
        // The execution mode of model (of this "processor" and all of it's children)
        // Режим выполнения модели (данного "процессора" и всех его детей).
        
        enum execution_mode_t {
            EM_UNKNOWN,
            EM_REAL,
            EM_XPLANE,
            EM_PREPAR3D
        };
        
        public:
            
            Processor ( AbstractAgentKernel* parent, QString name );
            virtual ~Processor();
            
            // The config file was found and loaded.
            // Конфигурационный файл был найден и загружен.
            
            bool configLoaded();
            
            // The configuration is acceptable for our, we can work.
            // Нас устраивает конфигурация, можно работать.
            
            bool configCorrect( bool say = false );
            
            execution_mode_t executionMode();
            
            static void prepareCommandLineParser( QCommandLineParser & parser );
            static QCommandLineOption noDaemonOption();
            
        protected:
            
            bool _loadConfig( QString fileName );            
            bool _configLoaded;
            
            execution_mode_t _execution_mode;
            
    };
    
};
