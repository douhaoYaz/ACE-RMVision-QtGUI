#include "include/collapse_box.h"
#include <QVBoxLayout>

CollapseItem::CollapseItem(const QString& title, QWidget* parent):
        QWidget(parent), name(title), visible(false) {
    QVBoxLayout* layout = new QVBoxLayout;

    btn = new CheckableButton(title, this);
    btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    layout->addWidget(btn);
    setLayout(layout);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    connect(btn, SIGNAL(clicked()), this, SLOT(onClick()));
}

void CollapseItem::addItem(QWidget* item) {
    item->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    items.push_back(item);
    this->layout()->addWidget(item);
    item->setVisible(visible);
}

void CollapseItem::showOrHide() {
    visible = !visible;
    for (auto iter=items.begin(); iter!=items.end(); ++iter)
        (*iter)->setVisible(visible);

    this->adjustSize();
}

QString CollapseItem::getName() {
    return name;
}

void CollapseItem::onClick() {
    emit clicked();
}

CollapseBox::CollapseBox(QWidget* parent): QWidget(parent) {
    QVBoxLayout* layout = new QVBoxLayout();
    layout->setAlignment(Qt::AlignTop);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    setLayout(layout);
}

void CollapseBox::addItem(CollapseItem* item) {
    item->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    items.insert(item->getName(), item);
    this->layout()->addWidget(item);
    connect(item, SIGNAL(clicked()), this, SLOT(onClick()));
}

CollapseItem* CollapseBox::getItem(const QString& name) {
    auto iter = items.find(name);
    return iter != items.end() ? iter.value() : nullptr;
}

void CollapseBox::onClick() {
    CollapseItem* itm = (CollapseItem*)sender();
    itm->showOrHide();
    this->adjustSize();
}
