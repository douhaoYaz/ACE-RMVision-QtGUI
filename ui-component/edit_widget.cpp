#include "include/edit_widget.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QSlider>
#include <QSpinBox>
#include <QDebug>


TrackbarItemWidget::TrackbarItemWidget(const QString& name, int w_key,
        int min, int max, int* pvalue, int value_default,
        const Function& callback, QWidget* parent): QWidget(parent),
        ItemWidgetBase<int>(callback) {

    QHBoxLayout* l = new QHBoxLayout();

    QLabel* key = new QLabel(name, this);
    key->setFixedWidth(w_key);
    key->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    QSlider* slider = new QSlider(Qt::Horizontal, this);
    key->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    slider->setRange(min, max);
    slider->setValue(value_default);

    cnt = new QSpinBox(this);
    cnt->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    cnt->setRange(min, max);
    cnt->setFixedWidth(50);
    cnt->setValue(value_default);

    connect(slider, SIGNAL(valueChanged(int)), cnt, SLOT(setValue(int)));
    connect(cnt, SIGNAL(valueChanged(int)), slider, SLOT(setValue(int)));

    l->addWidget(key);
    l->addWidget(slider);
    l->addWidget(cnt);
    setLayout(l);
    adjustSize();

    bind(pvalue);

    connect(cnt, SIGNAL(valueChanged(int)), this, SLOT(onValueChanged(int)));
}

void TrackbarItemWidget::bind(int* integer) {
    i = integer;
}


void TrackbarItemWidget::onValueChanged(int v) {
    if (func_callback)
        func_callback(i, v);

    else
        *i = v;
}

InputItemWidget::InputItemWidget(const QString& name, int w_key, std::string* pvalue, const QString& value_default,
        const Function& callback, QWidget* parent):
        QWidget(parent), ItemWidgetBase<std::string>(callback) {

        QHBoxLayout* l = new QHBoxLayout();

        QLabel* key = new QLabel(name, this);
        key->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        key->setFixedWidth(w_key);

        input = new QLineEdit(this);
        input->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        input->setText(value_default);

        l->addWidget(key);
        l->addWidget(input);
        this->setLayout(l);

        bind(pvalue);

        connect(input, SIGNAL(textChanged(const QString&)),
                this, SLOT(onTextChange(const QString&)));
    }

void InputItemWidget::bind(std::string* s) {
    str = s;
}

void InputItemWidget::onTextChange(const QString& txt) {
    if (func_callback)
        func_callback(str, txt.toStdString());
    else
        *str = txt.toStdString();

}

void EditItem::addInputItem(const QString& name, int w_key, std::string* pvalue,
        const std::function<void(std::string*, const std::string&)>& callback) {
    InputItemWidget* widget = new InputItemWidget(name,
            w_key, pvalue, QString(pvalue->c_str()), callback, this);
    CollapseItem::addItem(widget);
}

void EditItem::addTrackbarItem(const QString& name, int w_key, int min, int max, int* pvalue,
        const std::function<void(int*, int)>& func_callback) {
    TrackbarItemWidget* widget = new TrackbarItemWidget(name,
            w_key, min, max, pvalue, *pvalue, func_callback, this);
    CollapseItem::addItem(widget);
}

void EditWidget::addItemBar(const QString& name) {
    EditItem* itembar = new EditItem(name, box);
    box->addItem(itembar);
}

EditWidget::EditWidget(QWidget* parent): QScrollArea(parent) {
    this->setWidgetResizable(true);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
    box = new CollapseBox(this);
    box->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->setWidget(box);
}


void EditWidget::addInputItem(const QString& name_bar, const QString& name_tag,
        std::string* pvalue, const std::function<void(std::string*, const std::string&)>& callback) {
    auto bar = box->getItem(name_bar);
    if (bar == nullptr)
        return;
    qobject_cast<EditItem*>(bar)->addInputItem(name_tag, 80, pvalue, callback);
}

void EditWidget::addTrackbarItem(const QString& name_bar, const QString& name_tag, int min,
        int max, int* pvalue, const std::function<void(int*, int)>& callback) {
    CollapseItem* bar = box->getItem(name_bar);
    if (bar == nullptr)
        return;
    qobject_cast<EditItem*>(bar)->addTrackbarItem(name_tag, 80, min, max, pvalue, callback);
}
