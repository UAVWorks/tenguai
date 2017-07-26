// ********************************************************************************************************************
// *                                                                                                                  *
// *                                       Task QML editor with syntax highlight.                                     *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                    Редактор QMLя задачи с подсветкой синтаксиса.                                 *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 26 jul 2017 at 11:11 *
// ********************************************************************************************************************

#pragma once

#include <QTextEdit>
#include <QFrame>
#include <QLayout>
#include <QFont>
#include <QQmlEngine>

#include "QMLHighlighter.h"

namespace tengu {

    class DialogPropertiesTask_TabQML : public QFrame {
        
        Q_OBJECT
        
        public:
            
            DialogPropertiesTask_TabQML ( QWidget* parent = 0, Qt::WindowFlags f = 0 );
            virtual ~DialogPropertiesTask_TabQML();
        
        private:
            
            QTextEdit * __textEditor;
            QQmlEngine * __qmlEngine;
            QMLHighlighter * __highlighter;
    };
};

