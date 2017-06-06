// ********************************************************************************************************************
// *                                                                                                                  *
// *              "A processor". An agent that runs on a real hardware (computer, board) and that can manage          *
// *                                                 other agents.                                                    *
// * ---------------------------------------------------------------------------------------------------------------- *
// *     "Процессор". Агент, который выполняется на реальном железе (компьютере, плате) и который может управлять     *
// *                                                другими агентами.                                                 *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 29 may 2017 at 11:07 *
// ********************************************************************************************************************

#include "Processor.h"

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                 The constructor.                                                 *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                   Конструктор.                                                   *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::Processor::Processor ( tengu::AbstractAgent * parent, QString systemName ) 
    : AbstractAgent ( parent, systemName )
{
    
    // _setActivity( false );
    _execution_mode = EM_UNKNOWN;
    _configLoaded = _loadConfig( QString( PROCESSOR_CONFIG_FILE ) );    
        
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                           Load configuration from file.                                          *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                           Загрузка конфигурации из файла.                                        *
// *                                                                                                                  *
// ********************************************************************************************************************

bool tengu::Processor::_loadConfig ( QString fileName ) {
    
    QFile settingsFile( fileName );
    if ( settingsFile.exists() ) {
        
        QSettings settings( fileName, QSettings::IniFormat );
        
        QString uuid = settings.value("uuid", "").toString();        
        if ( ! uuid.isEmpty() ) {
            _uuid = uuid;
        };
        
        QString name = settings.value("system_name", "").toString();
        if ( ! name.isEmpty() ) {
            _system_name = name;
        };
        
        QString comment = settings.value("comment", "").toString();
        if ( ! comment.isEmpty() ) {
            _comment = comment;
        };
        
        QString mode = settings.value("execution_mode", "" ).toString();
        if ( ! mode.isEmpty() ) {
            if ( mode.toUpper() == "REAL" ) _execution_mode = EM_REAL;
            else if ( mode.toUpper() == "XPLANE" ) _execution_mode = EM_XPLANE;
            else if ( mode.toUpper() == "PREPAR3D" ) _execution_mode = EM_PREPAR3D;
        };
                
        return true;
    };
    
    return false;        
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                      Is loaded from file configuration correct?                                  *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                               Является ли загруженная из файла конфигурация корректной?                          *
// *                                                                                                                  *
// ********************************************************************************************************************

bool tengu::Processor::configCorrect( bool say ) {
    
    if ( _uuid.isEmpty() ) {
        if ( say ) qDebug() << tr("Processor::configCorrect() : UUID is empty");
        return false;
    };
    
    if ( _system_name.isEmpty() ) {
        if ( say ) qDebug() << tr("Processor::configCorrect() : system_name is empty");
        return false;
    };
    
    if ( ( _system_name.contains(' ')) || ( _system_name.contains('\t') ) ) {
        if ( say ) qDebug() << tr("Processor::configCorrect() : system name should not contains a whitespace or a tab characters.");
        return false;
    };
    
    if ( _execution_mode == EM_UNKNOWN ) {
        if ( say ) qDebug() << tr("Processor::configCorrect() : execution mode unknown.");
        return false;
    };
    
    return true;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                Was config file loaded?                                           *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                         Был ли загружен конфигурационный файл?                                   *
// *                                                                                                                  *
// ********************************************************************************************************************

bool tengu::Processor::configLoaded() {
    return _configLoaded;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                               Make and return the --no-daemon command line option.                               *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                           Сформировать и вернуть параметр командной строки --no-daemon.                          *
// *                                                                                                                  *
// ********************************************************************************************************************

QCommandLineOption tengu::Processor::noDaemonOption() {
    QCommandLineOption o("no-daemon");
    return o;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                      Create and return command line parser.                                      *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                       Сформировать и вернуть распознаватель аргументов командной строки.                         *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::Processor::prepareCommandLineParser ( QCommandLineParser & parser ) {
    
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addOption( Processor::noDaemonOption() );        
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                      Return execution mode of this "processor".                                  *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                    Вернуть режим выполнения этого "процессора".                                  *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::Processor::execution_mode_t tengu::Processor::executionMode() {
    return _execution_mode;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                  Create a separate child's (subagent's) process.                                 *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                          Создание отдельного дочернего (зависимого агентского) процесса.                         *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::Processor::_startSubagentProcess( AbstractAgentKernel * child ) {
    subagent_process_t subagent;
    subagent.child = child;
    subagent.state = AST_WAIT_STARTING;
    subagent.process = new QProcess();
    QObject::connect( subagent.process, SIGNAL( started() ), this, SLOT( __on_subprocess_started() ) );
    QObject::connect( subagent.process, SIGNAL( errorOccured( QProcess::ProcessError) ), this, SLOT( __on_subprocess_error( QProcess::ProcessError ) ) );
    QObject::connect( subagent.process, SIGNAL( finished( int, QProcess::ExitStatus ) ), this, SLOT( __on_subprocess_finished( int, QProcess::ExitStatus ) ) );
    subagent.process->start( child->subProcessPath() );    
    _subagents.append( subagent );
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                             A subprocess was started.                                            *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                               Подпроцесс - стартовал.                                            *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::Processor::__on_subprocess_started() {

    qDebug() << "Processor::__on_subprocess_started";
    QProcess * sender = qobject_cast< QProcess * > ( QObject::sender() );
    for ( int i=0; i<_subagents.length(); i++ ) {
        subagent_process_t sa = _subagents.at(i);
        if ( sa.process == sender ) {
            qDebug() << "Object found.";
            sa.state = AST_RUNNING;
            _subagents.replace( i, sa );
            break;
        }
    };
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                             A subprocess has any error.                                          *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                            У подпроцесса возникла ошибка.                                        *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::Processor::__on_subprocess_error(QProcess::ProcessError error) {
    qDebug() << "Processor::__on_subprocess_error: " << error ;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                             A subprocess was finished.                                           *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                       Подпроцесс - завершил свое выполнение.                                     *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::Processor::__on_subprocess_finished(int exitCode, QProcess::ExitStatus exitStatus) {
    
    qDebug() << "Processor::__on_subprocess_finished, exitCode=" << exitCode << ", status=" << exitStatus;
    QProcess * sender = qobject_cast< QProcess * > ( QObject::sender() );
    for ( int i=0; i<_subagents.length(); i++ ) {
        subagent_process_t sa = _subagents.at( i );
        if ( sa.process == sender ) {
            delete( sa.process );
            _subagents.removeAt( i );
            break;
        }
    };
    
}


// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                   The destructor.                                                *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                      Деструктор.                                                 *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::Processor::~Processor() {
    
    for ( int i=0; i<_subagents.length(); i++ ) {
        
        subagent_process_t subagent = _subagents.at(i);
        
        if ( ( subagent.process->state() == QProcess::Starting ) || ( subagent.process->state() == QProcess::Running ) ) {        
            subagent.process->terminate();
        };
        
        delete ( subagent.process );
        
    };
    
}
