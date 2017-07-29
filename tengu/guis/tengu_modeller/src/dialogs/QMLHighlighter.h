// ********************************************************************************************************************
// *                                                                                                                  *
// *                                               Syntax highlighter for QML                                         *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                 Подсветка QML синтаксиса                                         *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 26 jul 2017 at 11:32 *
// ********************************************************************************************************************

#pragma once

#include <QSyntaxHighlighter>
#include <QTextDocument>
#include <QRegularExpression>
#include <QTextCharFormat>

namespace tengu {        
    
    class QMLHighlighter : public QSyntaxHighlighter {        
          
        Q_OBJECT
        
        public:
            
            QMLHighlighter( QTextDocument * parent );
            virtual ~QMLHighlighter();
            
        protected:
            
            void highlightBlock( const QString & text);
            
        private:
            
            struct HighlightingRule {
                QRegularExpression pattern;
                QTextCharFormat format;
            };
            
            QVector<HighlightingRule> highlightingRules;
            
            QRegularExpression commentStartExpression;
            QRegularExpression commentEndExpression;

            QTextCharFormat keywordFormat;
            QTextCharFormat classFormat;
            QTextCharFormat singleLineCommentFormat;
            QTextCharFormat multiLineCommentFormat;
            QTextCharFormat quotationFormat;
            QTextCharFormat functionFormat;
            
    };
    
};

