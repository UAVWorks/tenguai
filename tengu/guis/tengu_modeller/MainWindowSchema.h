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
#include <QGraphicsView>
#include <QGraphicsScene>

namespace tengu {

    class MainWindowSchema : public QGraphicsView {
        
        Q_OBJECT
        
        public:
            
            MainWindowSchema( QGraphicsScene * scene );
            virtual ~MainWindowSchema();
            
        private:
                        
    };
};

