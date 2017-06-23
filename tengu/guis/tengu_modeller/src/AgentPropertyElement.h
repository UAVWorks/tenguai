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
            
            enum agent_property_t {
                
                // The simple string property (by default)
                // Просто строковое свойство (по умолчанию)
                
                String,
                
                // The float value
                // Значение с плавающей запятой
                
                Float,
                
                // The agent's execution mode combo box
                // Выпадающий список режима выполнения агента
                
                ExecutionModeSelector,
                
                // Type of sprout (input or output)
                // Тип "росточка" (вход или выход)
                
                SproutTypeSelector,
                
                // Sprout rotation angle at the chart
                // Угол поворота "росточка" на диаграмме
                
                SproutAngleSelector
            };
            
            AgentPropertyElement();
            AgentPropertyElement( QString text );
            ~AgentPropertyElement();
            
            // The name of Q_PROPERTY to read/write this agent's property 
            // Имя Q_PROPERTY для записи/чтения данного свойства агента.
            
            QString propertyName;
            
            // The value of this element
            // Значение элемента
            
            QVariant value;
            
            // The font for display property name (first column).
            // Шрифт для вывода имени свойства (первый столбец).
            
            QFont font;
            
            bool readOnly;
            
            QColor backgroundColor;
            
            QColor widgetBackground();
            QColor baseBackground();
            QColor disabledBackground();
            
            // QWidget * editorWidget;
            
            agent_property_t type;
            
            static AgentPropertyElement captionElement( QString title );
            
            // QVariant get();
            // void set( QVariant val );
    };
    
};

