#include "ui.h"
#include "start_window.h"
#include "main_window.h"
#include <QApplication>

int main(int argc, char* argv[]) {
    UserInterfaceFactory::getFactory().initialize();
    QApplication app(argc, argv);

    StartWindow* win_start = new StartWindow();

    MainWindow* win_main = MainWindow::getMainWindow();
//    win_main->hide();
    win_main->show();

    win_start->show();

    return app.exec();
}
