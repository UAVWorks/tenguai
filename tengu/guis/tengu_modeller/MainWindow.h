// ********************************************************************************************************************
// *                                                                                                                  *
// *                                       The main window of the Tengu Modeller.                                     *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                           Главное окно Tengu-моделлера.                                          *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 27 may 2017 at 08:26 *
// ********************************************************************************************************************

#pragma once

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {

    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

    private:

        Ui::MainWindow *ui;
};

