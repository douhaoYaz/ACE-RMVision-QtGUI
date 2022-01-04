#include "ui.h"
#include "start_window.h"
#include "main_window.h"
#include "thread_task.h"
#include <QApplication>
#include <QFile>

int main(int argc, char* argv[]) {

    UserInterfaceFactory::getFactory().initialize();

    QApplication app(argc, argv);

    QFile qss("../../materials/stylesheet/main.qss");
    qss.open(QFile::ReadOnly|QFile::Text);
    app.setStyleSheet(qss.readAll());
    qss.close();

    StartWindow* win_start = new StartWindow();

    MainWindow* win_main = MainWindow::getMainWindow();
    win_main->hide();

    QObject::connect(
            win_start,
            SIGNAL(openByMode(int, const QVariant&, const QVariant&, int)),
            win_main,
            SLOT(onInitializeSetting(int, const QVariant&, const QVariant&, int)));

    Setting setting;
    win_main->setSetting(setting);

    std::shared_ptr<ThreadRunner> tasks(new ThreadRunner(win_main->getSettingReference()));

    win_main->setTasks(tasks);

    win_start->show();

    if (win_start->exec() != QDialog::Accepted) {
        win_start->close();
        win_main->close();
        app.exit();
        return 0;
    }
    else {
        printf("in");
        win_main->show();
        win_start->close();
    }
    return app.exec();
}
