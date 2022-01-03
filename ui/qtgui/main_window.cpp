#include "include/main_window.h"
#include "opencv2/imgproc.hpp"
#include "data.h"
#include "parameter.h"
#include <QGridLayout>
#include <QLabel>
#include <QStyleOption>
#include <QPainter>
#include <QDockWidget>
#include <QListWidget>
#include <QDebug>
#include <QMenuBar>


MainWindow* MainWindow::win = nullptr;

MainWindow::MainWindow(QWidget* parent): QMainWindow(parent) {

}




MainWindow* MainWindow::getMainWindow() {
    if (win == nullptr)
        win = new MainWindow();
    return win;
}

void MainWindow::log(const QString& str) {
}

void MainWindow::closeEvent(QCloseEvent* event) {
    QMainWindow::closeEvent(event);
}

void MainWindow::registerTable(const QString& name_table,
        const QVector<QPair<QString, Pointer>>& kvs) {
}

void MainWindow::setCurrentTable(const QString& name) {
}

void MainWindow::createEditItem(const QString& name) {
}


void MainWindow::registerTrackBar(const QString& name_bar,
        const QString& key, int min, int max, int* pvalue) {
}
