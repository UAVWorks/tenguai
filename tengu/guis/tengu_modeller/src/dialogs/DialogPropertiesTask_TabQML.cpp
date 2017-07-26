// ********************************************************************************************************************
// *                                                                                                                  *
// *                                       Task QML editor with syntax highlight.                                     *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                    Редактор QMLя задачи с подсветкой синтаксиса.                                 *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 26 jul 2017 at 11:14 *
// ********************************************************************************************************************

#include "DialogPropertiesTask_TabQML.h"

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                The constructor.                                                  *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                  Конструктор.                                                    *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::DialogPropertiesTask_TabQML::DialogPropertiesTask_TabQML ( QWidget* parent, Qt::WindowFlags f ) 
    : QFrame ( parent, f ) 
{
    QHBoxLayout * lay = new QHBoxLayout();
    lay->setMargin( 5 );
    lay->setSpacing( 5 );
    lay->setAlignment( Qt::AlignCenter );
    setLayout( lay );
    
    __textEditor = new QTextEdit();
    lay->addWidget( __textEditor );
    
    __highlighter = new QMLHighlighter( __textEditor->document() );
    
    QFont font;    
    //font.setFamily("DejaVu Sans Mono");    
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(10);
    __textEditor->setFont( font );
    
    __qmlEngine = new QQmlEngine( this );
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                               The destructor.                                                    *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                  Деструктор.                                                     *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::DialogPropertiesTask_TabQML::~DialogPropertiesTask_TabQML() {
    
    __qmlEngine->collectGarbage();
    delete( __qmlEngine );    
    __qmlEngine = nullptr;
    
}



