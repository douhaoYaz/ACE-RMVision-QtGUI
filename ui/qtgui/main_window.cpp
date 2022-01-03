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


//    this->setDockNestingEnabled(true);
    this->initializeMainImage();
    this->initializeEditWidget();
    this->initializeShowWidget();
    this->initializeControlWidget();
    this->initializeLogWidget();
    this->initializeMenuBar();
    this->initializeImageWidget();

}

void MainWindow::initializeMenuBar() {
    QMenuBar* menubar = new QMenuBar(this);

    QMenu* menu_win = new QMenu("窗口", this);
    menubar->addMenu(menu_win);

    QMenu* menu_create = new QMenu("创建", menubar);

    menubar->addMenu(menu_create);
    this->setMenuBar(menubar);
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

void MainWindow::initializeMainImage() {
    QLabel* img = new QLabel(this);
    dock_main = new QDockWidget("main", this);
    dock_main->setWidget(img);
    dock_main->setFeatures(QDockWidget::DockWidgetMovable|
            QDockWidget::DockWidgetFloatable);
    this->addDockWidget(Qt::LeftDockWidgetArea, dock_main);
}

void MainWindow::initializeEditWidget() {
    dock_edit = new QDockWidget("edit", this);
    EditWidget* w = new EditWidget(this);
    dock_edit->setWidget(w);
    this->addDockWidget(Qt::RightDockWidgetArea, dock_edit);
}

void MainWindow::initializeShowWidget() {

    dock_show = new QDockWidget("show", this);
    ShowWidget* w = new ShowWidget(this);
    dock_show->setWidget(w);
    this->addDockWidget(Qt::LeftDockWidgetArea, dock_show);
}

void MainWindow::initializeControlWidget() {
    dock_ctrl = new QDockWidget("control", this);
    ControlArea* w = new ControlArea(this);
    dock_ctrl->setFixedSize(300, 200);
    dock_ctrl->setWidget(w);
    this->addDockWidget(Qt::RightDockWidgetArea, dock_ctrl);
    connect(w->radios_mode, SIGNAL(buttonClicked(int)),
            this, SLOT(onModeChanged(int)));
    connect(w->radios_color, SIGNAL(buttonClicked(int)),
            this, SLOT(onColorChanged(int)));
    connect(w->btn_save, SIGNAL(clicked(bool)),
            this, SLOT(onSave(bool)));
}

void MainWindow::initializeLogWidget() {
    dock_log = new QDockWidget("log", this);
    QListWidget* w = new QListWidget(this);
    dock_log->setWidget(w);
    this->addDockWidget(Qt::LeftDockWidgetArea, dock_log);
}

void MainWindow::initializeImageWidget() {
//    Data& data = Data::getData();
//    ImageWidget::registerImage("main", &(data.img_show));
//    ImageWidget::registerImage("color", &(data.img_color));
//    ImageWidget::registerImage("gray", &(data.img_gray));
//    ImageWidget::registerImage("roi", &(data.img_roi));
//    ImageWidget::registerImage("id", &(data.img_id));
}
