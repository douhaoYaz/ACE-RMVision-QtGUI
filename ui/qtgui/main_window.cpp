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

    timer = new QTimer(this);
    this->setDockNestingEnabled(true);
    this->initializeMainImage();
    this->initializeEditWidget();
    this->initializeShowWidget();
    this->initializeControlWidget();
    this->initializeLogWidget();
    this->initializeMenuBar();
    this->initializeImageWidget();
    connect(timer, &QTimer::timeout,
            [this]() {
                updateMainImage();
                updateShowWidget();
                checkControlArea();
            });
}

void MainWindow::initializeMenuBar() {
    QMenuBar* menubar = new QMenuBar(this);

    QMenu* menu_win = new QMenu("窗口", this);

//    auto callback_action = [](QWidget* widget, bool hidden) {
//        if (widget->isHidden() == hidden)

//    };

    QAction* action_edit = new QAction("Edit", this);
    action_edit->setCheckable(true);
    menu_win->addAction(action_edit);
    connect(dock_edit, SIGNAL(visibilityChanged(bool)), action_edit, SLOT(setChecked(bool)));
    connect(action_edit, SIGNAL(toggled(bool)), dock_edit, SLOT(setVisible(bool)));

    QAction* action_show = new QAction("Show", this);
    action_show->setCheckable(true);
    menu_win->addAction(action_show);
    connect(dock_show, SIGNAL(visibilityChanged(bool)), action_show, SLOT(setChecked(bool)));
    connect(action_show, SIGNAL(toggled(bool)), dock_show, SLOT(setVisible(bool)));

    QAction* action_log = new QAction("Log", this);
    action_log->setCheckable(true);
    menu_win->addAction(action_log);
    connect(dock_log, SIGNAL(visibilityChanged(bool)), action_log, SLOT(setChecked(bool)));
    connect(action_log, SIGNAL(toggled(bool)), dock_log, SLOT(setVisible(bool)));

    QAction* action_ctrl = new QAction("Control", this);
    action_ctrl->setCheckable(true);
    menu_win->addAction(action_ctrl);
    connect(dock_ctrl, SIGNAL(visibilityChanged(bool)), action_ctrl, SLOT(setChecked(bool)));
    connect(action_ctrl, SIGNAL(toggled(bool)), dock_ctrl, SLOT(setVisible(bool)));

    menubar->addMenu(menu_win);

    QMenu* menu_create = new QMenu("创建", menubar);

    QAction* action_create_img = new QAction("Image Widget");
    connect(action_create_img, &QAction::triggered,
            this, &MainWindow::addImageWidget);
    menu_create->addAction(action_create_img);

    menubar->addMenu(menu_create);
    this->setMenuBar(menubar);
}



MainWindow* MainWindow::getMainWindow() {
    if (win == nullptr)
        win = new MainWindow();
    return win;
}

void MainWindow::initialize() {
    timer->start(1000/60);
}

void MainWindow::log(const QString& str) {
    QListWidget* w = qobject_cast<QListWidget*>(dock_log->widget());
    w->addItem(str);
}

void MainWindow::addImageWidget() {
    QDockWidget* dock = new QDockWidget("image", this);
    dock->setFloating(true);
    dock->setAttribute(Qt::WA_DeleteOnClose);
    ImageWidget* w = new ImageWidget(dock);
    w->setAttribute(Qt::WA_DeleteOnClose);
    dock->setWidget(w);
    dock->show();
    connect(timer, SIGNAL(timeout()), w, SLOT(refresh()));
}

void MainWindow::closeEvent(QCloseEvent* event) {
    task.reset();
    QMainWindow::closeEvent(event);
}

void MainWindow::registerTable(const QString& name_table,
        const QVector<QPair<QString, Pointer>>& kvs) {
    table.insert(name_table, kvs);
    QStringList l;
    for (auto& pair: kvs)
        l.push_back(pair.first);
    auto w = qobject_cast<ShowWidget*>(dock_show->widget());
    w->addModel(name_table, l);
}

void MainWindow::setCurrentTable(const QString& name) {
    auto w = qobject_cast<ShowWidget*>(dock_show->widget());
    w->changeModel(name);
}

void MainWindow::createEditItem(const QString& name) {
    auto w = qobject_cast<EditWidget*>(dock_edit->widget());
    w->addItemBar(name);
}


void MainWindow::registerTrackBar(const QString& name_bar,
        const QString& key, int min, int max, int* pvalue) {
    int offset = min;
    bool& flag = Data::getData().flag_update_param;
    auto callback = [offset, &flag](int* p, int src) {
        *p = src-offset;
        flag = true;
    };
    auto w = qobject_cast<EditWidget*>(dock_edit->widget());
    w->addTrackbarItem(name_bar, key, min, max, pvalue, callback);
}

void MainWindow::setSetting(const Setting& setting) {
    this->setting = setting;
}

Setting& MainWindow::getSettingReference() {
    return setting;
}

void MainWindow::setTasks(std::shared_ptr<ThreadRunnerBase> task) {
    this->task = task;
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

void MainWindow::updateMainImage() {
    cv::Mat& img_show = Data::getData().img_show;
    if (img_show.empty() == false) {
        cv::Mat img_rgb;
        cv::cvtColor(img_show, img_rgb, cv::COLOR_BGR2RGB);
        QLabel* img = qobject_cast<QLabel*>(dock_main->widget());
        img->setPixmap(QPixmap::fromImage(QImage(img_rgb.data,
                img_rgb.cols, img_rgb.rows, QImage::Format_RGB888)));
        img->adjustSize();
        dock_main->setFixedSize(img->size());
    }
}

void MainWindow::updateShowWidget() {
    ShowWidget* w = qobject_cast<ShowWidget*>(dock_show->widget());
    QString name = w->getCurrentTableName();

    auto iter = table.find(name);
    if (iter == table.end())
        return;
    auto& vec = iter.value();

    int i = 0;
    for (auto& pair: vec) {
        switch (pair.second.getFlag()) {
        case Pointer::BOOLEAN:
            w->setItem(i, *static_cast<bool*>(pair.second.get()) ? "True" : "Flase");
            break;
        case Pointer::INTEGER:
            w->setItem(i, QString::number(*static_cast<int*>(pair.second.get())));
            break;
        case Pointer::SHORT:
            w->setItem(i, QString::number(*static_cast<short*>(pair.second.get())));
            break;
        case Pointer::FLOAT:
            w->setItem(i, QString::number(*static_cast<float*>(pair.second.get())));
            break;
        case Pointer::DOUBLE:
            w->setItem(i, QString::number(*static_cast<double*>(pair.second.get())));
            break;
        case Pointer::STRING:
            w->setItem(i, static_cast<char*>(pair.second.get()));
            break;
        default:
            w->setItem(i, "");
            break;
        };
        i++;
    }
}

void MainWindow::checkControlArea() {
    ControlArea* w = qobject_cast<ControlArea*>(dock_ctrl->widget());
    Parameter& param = Parameter::getParameter();
    Parameter& param_tmp = Parameter::getTempParameter();
    if (param.other.color_enemy == param_tmp.other.color_enemy &&
            param.other.color_enemy != w->radios_color->checkedId())
        w->radios_color->button(param.other.color_enemy)->click();
    if (param.other.mode_detect == param_tmp.other.mode_detect &&
            param.other.mode_detect != w->radios_mode->checkedId())
        w->radios_mode->button(param.other.mode_detect)->click();
}

void MainWindow::onInitializeSetting(int mode, const QVariant& param1,
        const QVariant& param2, int type) {
    std::string path_param;
    if (mode == 0) {
        switch (type) {
        case ROBOT_INFANTRY: default:
            if (param2.toString().size() == 0)
                setting.setInfantryParameter("../../parameter/capture/"+param1.toString().toStdString());
            else
                setting.setInfantryParameter("../../parameter/capture/"+param1.toString().toStdString(),
                        "../../parameter/capture/"+param2.toString().toStdString());
            break;
       case ROBOT_GUARD:
            setting.setGuardParameter("../../parameter/capture/"+param1.toString().toStdString());
            break;
       }
    }
    else if (mode == 1)
        setting.setVideoParameter("../../video/"+param1.toString().toStdString());

    qDebug() << QString(setting.path_classifier_arm_caffe_net.c_str());
    task->run();


//    UITool::log("Thread task start!", FONT_NONE);
//    if (mode == 0) {
//        switch (type) {
//        case CAR_INFANTRY: default:
//            UITool::log("Now it is INFANTRY mode!", FONT_NONE);
//            break;
//       case CAR_GUARD:
//            UITool::log("Now it is GUARD mode!", FONT_NONE);
//            break;
//       case CAR_HERO:
//            UITool::log("Now it is HERO mode!", FONT_NONE);
//            break;
//       }
//    }
//    else if (mode == 1)
//        UITool::log("Now it is VIDEO mode!", FONT_NONE);

}

void MainWindow::onModeChanged(int mode) {
    DetectMode& m = Parameter::getTempParameter().other.mode_detect;
    if (m == mode)
        return;
    m = static_cast<DetectMode>(mode);
    if (mode == DETECT_ARMOR)
        log("Switch mode to ARMOR!");
    else if (mode == DETECT_BUFF)
        log("Switch mode to BUFF!");
    else if (mode == DETECT_TOP)
        log("Switch mode to TOP");
    Data::getData().flag_update_param = true;
}

void MainWindow::onSave(bool clicked) {
    Parameter::getTempParameter().other.flag_save = clicked;
    Data::getData().flag_update_param = true;
}

void MainWindow::onColorChanged(int color) {
    TargetColor& c = Parameter::getTempParameter().other.color_enemy;
    if (color == c)
        return;
    c = static_cast<TargetColor>(color);
    if (color == DETECT_RED)
        log("Detect color is RED!");
    else if (color == DETECT_BLUE)
        log("Detect color is BLUE!");
    Data::getData().flag_update_param = true;
}

