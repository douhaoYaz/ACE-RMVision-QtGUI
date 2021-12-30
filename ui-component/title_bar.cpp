#include "include/title_bar.h"

#include <QHBoxLayout>
#include <QMouseEvent>
#include <QStyleOption>
#include <QPainter>

TitleBar::TitleBar(QWidget* parent):
    QWidget(parent), is_pressing(false) {
    this->setFixedHeight(30);

    icon = new QLabel(this);
    icon->setFixedSize(25, 25);

    title = new QLabel(this);
    title->setFixedHeight(25);
    title->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    btn_minimum = new IconButton("../../ACERMVision/materials/icon/minus_light.png",
            "../../ACERMVision/GUI/materials/icon/minus_white.png", this);
    btn_minimum->setFixedSize(25, 25);

    btn_close = new IconButton("../../ACERMVision/materials/icon/close_light.png",
            "../../ACERMVision/materials/icon/close_white.png", this);
    btn_close->setFixedSize(25, 25);

    QHBoxLayout* layout = new QHBoxLayout();
    layout->addWidget(icon);
    layout->addStretch(1);
    layout->addWidget(title);
    layout->addStretch(1);
    layout->addWidget(btn_minimum);
    layout->addWidget(btn_close);
    layout->setAlignment(Qt::AlignVCenter);
    layout->setMargin(0);
    this->setLayout(layout);

    connect(btn_minimum, SIGNAL(clicked()), this, SLOT(onMinimum()));
    connect(btn_close, SIGNAL(clicked()), this, SLOT(onClose()));

}

void TitleBar::mousePressEvent(QMouseEvent* event) {
    if (!is_pressing && event->button() == Qt::LeftButton) {
        is_pressing = true;
        pt_start = event->globalPos();
    }
}

void TitleBar::mouseMoveEvent(QMouseEvent* event) {
    if (is_pressing) {
        QWidget* p = this->parentWidget();
        QRect rect = p->geometry();
        p->move(rect.topLeft() + event->globalPos() - pt_start);
        pt_start = event->globalPos();
    }
}

void TitleBar::mouseReleaseEvent(QMouseEvent* event) {
    if (is_pressing && event->button() == Qt::LeftButton)
        is_pressing = false;

}

void TitleBar::paintEvent(QPaintEvent*) {
    QStyleOption opt;
    opt.init(this);
    QPainter* painter = new QPainter(this);
    this->style()->drawPrimitive(QStyle::PE_Widget, &opt, painter, this);
    painter->end();
}

bool TitleBar::eventFilter(QObject* watched, QEvent* event) {
    auto type = event->type();

    if (type == QEvent::WindowStateChange || type == QEvent::Resize) {
        this->setFixedWidth(this->parentWidget()->width());
        return true;
    }
    return QWidget::eventFilter(watched, event);
}

void TitleBar::onMinimum() {
    this->window()->minimumSize();
}

void TitleBar::onClose() {
    this->window()->close();
}
