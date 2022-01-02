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

ControlArea::ControlArea(QWidget* parent): QWidget(parent) {
    QGridLayout* layout = new QGridLayout();

    layout->addWidget(new QLabel("mode:", this), 0, 0);

    radio_armor = new QRadioButton("armor", this);
    layout->addWidget(radio_armor, 0, 1);

    radio_buff = new QRadioButton("buff", this);
    layout->addWidget(radio_buff, 0, 2);

    radio_top = new QRadioButton("top", this);
    layout->addWidget(radio_top, 0, 3);

    radios_mode = new QButtonGroup();
    radios_mode->addButton(radio_armor, 0);
    radios_mode->addButton(radio_buff, 1);
    radios_mode->addButton(radio_top, 2);
    radio_armor->setChecked(true);

    layout->addWidget(new QLabel("color:", this), 1, 0);

    radio_red = new QRadioButton("red", this);
    layout->addWidget(radio_red, 1, 1);

    radio_blue = new QRadioButton("blue", this);
    layout->addWidget(radio_blue, 1, 2);

    radios_color = new QButtonGroup();
    radios_color->addButton(radio_red, 0);
    radios_color->addButton(radio_blue, 1);
    radio_blue->setChecked(true);

    btn_save = new CheckableButton("save", this);
    layout->addWidget(btn_save, 2, 0, 1, 4);
    this->setLayout(layout);
}

void ControlArea::paintEvent(QPaintEvent*) {
    QStyleOption opt;
    opt.init(this);
    QPainter* painter = new QPainter(this);
    this->style()->drawPrimitive(QStyle::PE_Widget, &opt, painter, this);
    painter->end();
}

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
