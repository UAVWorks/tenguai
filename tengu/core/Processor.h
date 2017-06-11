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
#include <QProcess>
#include <QVector>

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
        
        enum agent_state_t {
            AST_UNKNOWN,
            AST_WAIT_STARTING,
            AST_RUNNING,
            // AST_LIVE,
            // AST_TIMEOUT,
            AST_WAIT_FINISHING
        };
        
        // The structure with a pointer to the running child process.
        // Структура, содержащая в себе указатель на выполняющийся дочерний процесс.
        
        struct subagent_process_t {
            agent_state_t state;
            // Qt-процесс, который мы породили в операционной системе.
            QProcess * process;
            // Для кого конкретно из детей (коллекция _children) был поднят этот процесс.
            AbstractAgentKernel * child;
        };
        
        signals:
            
            // void signalAgentStarted( AbstractAgentKernel * agent );
            
        public:
            
            Processor ();
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
            
            // Create child (agent) as separate operation system process.
            // Создание ребенка (агента) как отдельного процесса в операционной системе.
            
            void _startSubagentProcess( AbstractAgentKernel * child );
            
            // Load configuration for this processor (from config file).
            // Загрузить конфигурацию для данного процессора (из конфигурационного файла).
            
            bool _loadConfig( QString fileName );            
            
            // Was configuration loaded?
            // Была ли конфигурация загружена?
            
            bool _configLoaded;

            // Running mode (either real or simulation).
            // Режим выполнения (реальный или модельный).
            
            execution_mode_t _execution_mode;
            
            // Subprocesses for this process. The processes which form the children.
            // But not as classes in memory. Instead of that, some (complex) children 
            // can be a separate processes ( a live agents ) internal of the operation system.
            
            // Подпроцессы этого процесса. Процессы, в результате которых получаются
            // агенты - "дети" данного процессора. Но не как классы в памяти, а как
            // отдельные самостоятельно выполняющиеся процессы.
            
            QVector< subagent_process_t > _subagents;
            
        private slots:
            
            void __on_subprocess_started();
            void __on_subprocess_error( QProcess::ProcessError error );
            void __on_subprocess_finished( int exitCode, QProcess::ExitStatus exitStatus );
            // void __on_agent_started( AbstractAgentKernel * agent );            
            
    };
    
};
