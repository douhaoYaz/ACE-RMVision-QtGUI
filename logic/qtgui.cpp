#include "qtgui.h"

#include "main_window.h"
QtGraphicalUserInterface::QtGraphicalUserInterface()
    :UserInterface() { }

void QtGraphicalUserInterface::log(const char* str) {
    MainWindow::getMainWindow()->log(str);
}

void QtGraphicalUserInterface::registerImage(const char* name_title,
        cv::Mat* img, int type) {
    ImageWidget::registerImage(name_title, img, type);
}

void QtGraphicalUserInterface::registerDataTable(const char* name_table,
        const std::vector<const char*>& keys, const std::vector<Pointer>& ptrs) {
    QVector<QPair<QString, Pointer>> vec;
    auto iter1=keys.begin();
    auto iter2=ptrs.begin();
    for (; iter1!=keys.end() && iter2!=ptrs.end(); ++iter1, ++iter2)
        vec.push_back(qMakePair(QString(*iter1), *iter2));
    MainWindow::getMainWindow()->registerTable(name_table, vec);
}

void QtGraphicalUserInterface::setCurrentTable(const char* name) {
    MainWindow::getMainWindow()->setCurrentTable(name);
}

void QtGraphicalUserInterface::createEditItemBar(const char* name) {
    MainWindow::getMainWindow()->createEditItem(name);
}

void QtGraphicalUserInterface::registerTrackBar(const char* name_bar,
        const char* name, int* data, int min, int max) {
    MainWindow::getMainWindow()->registerTrackBar(name_bar, name, min, max, data);
}
