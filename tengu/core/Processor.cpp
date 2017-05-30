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

tengu::Processor::Processor ( tengu::AbstractAgentKernel * parent, QString name ) 
    : AbstractAgent ( parent, name )
{
    
    _setActivity( false );
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
        
        QString name = settings.value("name", "").toString();
        if ( ! name.isEmpty() ) {
            _name = name;
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
    
    if ( _name.isEmpty() ) {
        if ( say ) qDebug() << tr("Processor::configCorrect() : name is empty");
        return false;
    };
    
    if ( ( _name.contains(' ')) || ( _name.contains('\t') ) ) {
        if ( say ) qDebug() << tr("Processor::configCorrect() : name should not contains a space or a tab characters.");
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
// *                                                   The destructor.                                                *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                      Деструктор.                                                 *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::Processor::~Processor() {

}
