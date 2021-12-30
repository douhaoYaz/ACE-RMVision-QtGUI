#include "include/start_window.h"
#include <QGridLayout>
#include <QDebug>
#include <QDir>
#include <QSettings>

CapturePage::CapturePage(QWidget* parent): QWidget(parent) {

    QGridLayout* l = new QGridLayout();

    lbl_cap1 = new QLabel("摄像头1参数：", this);
    lbl_cap1->setFixedSize(100, 30);
    l->addWidget(lbl_cap1, 1, 1);

    lbl_cap2 = new QLabel("摄像头2参数：", this);
    lbl_cap2->setFixedSize(100, 30);
    l->addWidget(lbl_cap2, 2, 1);

    lbl_type = new QLabel("兵种:", this);
    l->addWidget(lbl_type, 3, 1);

    QDir dir("../../../ACERMVision_new_douhao/parameter/capture");
    auto list = dir.entryList({ "*.xml" }, QDir::Files|QDir::Readable, QDir::Name);


    selector_cap1 = new QComboBox(this);
    selector_cap1->setFixedSize(200, 30);
    selector_cap1->addItems(list);

    l->addWidget(selector_cap1, 1, 2);

    selector_cap2 = new QComboBox(this);
    selector_cap2->setFixedSize(200, 30);
    selector_cap2->addItem("");
    selector_cap2->addItems(list);

    l->addWidget(selector_cap2, 2, 2);

    selector_type = new QComboBox(this);
    selector_type->addItems({ "步兵", "哨兵", "英雄", "工程", "无人机", "前哨站" });
    selector_type->setFixedSize(200, 30);
    l->addWidget(selector_type, 3, 2);

    l->setAlignment(Qt::AlignCenter);

    this->setLayout(l);

}

void CapturePage::getParameter(QVariant& cap1, QVariant& cap2, int& type) {
    cap1 = selector_cap1->currentText();
    cap2 = selector_cap2->currentText();
    type = selector_type->currentIndex();
}

VideoPage::VideoPage(QWidget* parent):
        QWidget(parent) {

    QGridLayout* l = new QGridLayout();

    lbl_video = new QLabel("视频：", this);
    lbl_video->setFixedSize(50, 30);
    l->addWidget(lbl_video, 1, 1);

//    lbl_fps = new QLabel("帧数：", this);
//    lbl_fps->setFixedSize(50, 30);
//    l->addWidget(lbl_fps, 2, 1);

    selector = new QComboBox(this);
    selector->setFixedSize(245, 30);
    QDir dir("../video");
    auto list = dir.entryList({ "*.avi", "*.mp4" }, QDir::Files|QDir::Readable, QDir::Name);
    selector->addItems(list);
    l->addWidget(selector, 1, 2);

    this->setLayout(l);

    QSettings settings("../../../ACERMVision_new_douhao/parameter/GUI/gui.ini", QSettings::IniFormat);
    QString video = settings.value("start/video").toString();
    selector->setCurrentText("video");
}

void VideoPage::getParameter(QVariant& video) {
    video = selector->currentText();
}

StartWindow::StartWindow(QWidget *parent):
        QDialog(parent) {

    this->setWindowFlags(Qt::FramelessWindowHint);
    QVBoxLayout* l = new QVBoxLayout();

    titlebar = new TitleBar(this);
    l->addWidget(titlebar);

    QHBoxLayout* l_icon = new QHBoxLayout();

    QLabel* icon = new QLabel(this);
    icon->setFixedSize(60, 60);
    icon->setStyleSheet("image: url(../../../ACERMVision_new_douhao/materials/icon/logo_white.png)");
    QLabel* icon_txt = new QLabel(this);
    icon_txt->setFixedSize(150, 50);
    icon_txt->setStyleSheet("image: url(../../../ACERMVision_new_douhao/materials/icon/txt.png)");
    l_icon->addStretch(1);
    l_icon->addWidget(icon);
    l_icon->addWidget(icon_txt);
    l_icon->addStretch(1);

    l->addStretch(1);
    l->addLayout(l_icon);
    l->addStretch(1);

    widget_tab = new TabWidget(this);
    widget_tab->setFixedSize(330, 180);
    page_cap = new CapturePage(widget_tab);
    widget_tab->add(page_cap, "摄像头模式");
    page_video = new VideoPage(widget_tab);
    widget_tab->add(page_video, "视频模式");

    l->addWidget(widget_tab, 0, Qt::AlignCenter);
    l->addStretch(1);

    btn_start = new IconButton("../../../ACERMVision_new_douhao/materials/icon/start_light.png",
            "../../../ACERMVision_new_douhao/materials/icon/start_white.png", this);
    btn_start->setCursor(Qt::PointingHandCursor);
    btn_start->setFixedSize(50, 50);
    l->addWidget(btn_start, 0, Qt::AlignCenter);
    l->addStretch(1);

    l->setAlignment(Qt::AlignCenter);
    l->setMargin(0);
    this->setLayout(l);

    QObject::connect(btn_start, SIGNAL(clicked()), this, SLOT(onStart()));


    QSettings settings("../../../ACERMVision_new_douhao/parameter/GUI/gui.ini", QSettings::IniFormat);
    QPoint pos = settings.value("start/pos").toPoint();

    int page = settings.value("start/page").toInt();
    QString cap1 = settings.value("start/cap1").toString();
    QString cap2 = settings.value("start/cap2").toString();
    int type = settings.value("start/type").toInt();

    QString video = settings.value("start/video").toString();

    this->setGeometry(pos.x(), pos.y(), 350, 450);
    widget_tab->switchPage(page);
    page_cap->selector_cap1->setCurrentText(cap1);
    page_cap->selector_cap2->setCurrentText(cap2);
    page_cap->selector_type->setCurrentIndex(type);

    page_video->selector->setCurrentText(video);

}

void StartWindow::closeEvent(QCloseEvent* event) {
    QSettings settings("../../../ACERMVision_new_douhao/parameter/GUI/gui.ini", QSettings::IniFormat);
    settings.setValue("start/pos", QPoint(this->pos()));
    settings.setValue("start/page", widget_tab->getCurrentIndex());

    QVariant cap1;
    QVariant cap2;
    int type;
    page_cap->getParameter(cap1, cap2, type);
    settings.setValue("start/cap1", cap1);
    settings.setValue("start/cap2", cap2);
    settings.setValue("start/type", type);

    QVariant video;
    page_video->getParameter(video);
    settings.setValue("start/video", video);

    QDialog::closeEvent(event);
}


void StartWindow::onStart() {

    int idx = widget_tab->getCurrentIndex();
    QVariant param1;
    QVariant param2;
    int type = 0;
    if (idx == 0)
        page_cap->getParameter(param1, param2, type);
    else if (idx == 1)
        page_video->getParameter(param1);
    emit openByMode(idx, param1, param2, type);
    accept();
}
