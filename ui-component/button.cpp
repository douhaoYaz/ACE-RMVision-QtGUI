#include "include/button.h"
#include <QPainter>
#include <QPaintEvent>
#include <QStyleOption>

Button::Button(QWidget* parent): QPushButton(parent) { }

Button::Button(const QString& txt, QWidget* parent):
QPushButton(txt, parent) { }

//IconButton::IconButton(QImage common, QImage hover, QWidget* parent):
//        QPushButton(parent), img_common(common), img_hover(hover), is_hover(false) { }

IconButton::IconButton(const QString& common, const QString& hover,
        QWidget* parent): QPushButton(parent) {
    QString qss("IconButton { image: url(");
    qss += common;
    qss += "); } IconButton::hover { image: url(";
    qss += hover;
    qss += "); }";
    this->setStyleSheet(qss);
}

//void IconButton::enterEvent(QEvent*) {
//    this->is_hover = true;
//}

//void IconButton::leaveEvent(QEvent*) {
//    this->is_hover = false;
//}

void IconButton::paintEvent(QPaintEvent*) {
    QStyleOption opt;
    opt.init(this);
    QPainter* painter = new QPainter(this);
    this->style()->drawPrimitive(QStyle::PE_Widget, &opt, painter, this);
    painter->end();
//    QPainter* painter = new QPainter(this);
//    if (is_hover)
//        painter->drawImage(event->rect(), img_hover, img_hover.rect());
//    else
//        painter->drawImage(event->rect(), img_common, img_hover.rect());
//    painter->end();
}

CheckableButton::CheckableButton(const QString& txt, QWidget* parent):
        QPushButton(txt, parent) {
    this->setCheckable(true);
    this->setChecked(false);
    this->setCursor(Qt::PointingHandCursor);
}
