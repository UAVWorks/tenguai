// ********************************************************************************************************************
// *                                                                                                                  *
// *                                   One element (one row) of agent properties table.                               *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                 Один элемент (одна строка) таблицы свойств агента.                               *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 20 jun 2017 at 09:38 *
// ********************************************************************************************************************

#pragma once

#include <QString>
#include <QVariant>
#include <QFont>
#include <QColor>
#include <QWidget>

namespace tengu {
    
    class AgentPropertyElement {
        
        public:
            
            AgentPropertyElement();
            AgentPropertyElement( QString text );
            ~AgentPropertyElement();
            
            // The name of Q_PROPERTY to read/write this agent's property 
            // Имя Q_PROPERTY для записи/чтения данного свойства агента.
            
            QString propertyName;
            
            // The text of property name (first column)
            // Текст имени свойства (первый столбец)
            
            QVariant value;
            
            // The font for display property name (first column).
            // Шрифт для вывода имени свойства (первый столбец).
            
            QFont font;
            
            bool readOnly;
            
            QColor backgroundColor;
            
            QColor widgetBackground();
            QColor baseBackground();
            QColor disabledBackground();
            
            // QVariant get();
            // void set( QVariant val );
    };
    
};

