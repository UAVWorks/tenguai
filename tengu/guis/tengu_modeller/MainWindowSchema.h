// ********************************************************************************************************************
// *                                                                                                                  *
// *                            UML-like "schema", the graphics representation of model.                              *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                             UML-подобная "схема", графическое представление модели.                              *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 28 may 2017 at 15:48 *
// ********************************************************************************************************************

#pragma once

#include <QWidget>

namespace tengu {

    class MainWindowSchema : public QWidget {
        
        Q_OBJECT
        
        public:
            MainWindowSchema( QWidget * parent = 0 );
            virtual ~MainWindowSchema();
    };
};

