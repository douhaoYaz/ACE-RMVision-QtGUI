#include "include/button.h"
#include "include/tab_widget.h"

TabWidget::TabWidget(QWidget* parent): QWidget(parent) {

    QVBoxLayout* l = new QVBoxLayout();

    hlayout_btn = new QHBoxLayout();
    l->addLayout(hlayout_btn);

    gp_tab = new QButtonGroup();

    pages = new QStackedWidget(this);
    l->addWidget(pages);

    l->setAlignment(Qt::AlignCenter);
    l->setMargin(0);
    this->setLayout(l);

    connect(gp_tab, SIGNAL(buttonClicked(int)), this, SLOT(onTabClicked(int)));

}

void TabWidget::add(QWidget* widget, const QString& txt_btn) {
    CheckableButton* btn = new CheckableButton(txt_btn, this);
    int cnt = pages->count();
    if (cnt == 0)
        btn->setChecked(true);

    int width = this->width();
    auto btns = gp_tab->buttons();

    cnt++;
    for (auto iter=btns.begin(); iter!=btns.end(); ++iter)
        (*iter)->setFixedWidth(width / cnt);
    btn->setFixedWidth(width/cnt);
    hlayout_btn->addWidget(btn);
    hlayout_btn->setAlignment(Qt::AlignVCenter);
    hlayout_btn->setMargin(0);
    hlayout_btn->setSpacing(0);
    gp_tab->addButton(btn, pages->count());
    pages->addWidget(widget);


}

int TabWidget::getCurrentIndex() {
    return pages->currentIndex();
}

void TabWidget::switchPage(int page) {
    if (gp_tab->checkedId() != page)
        gp_tab->button(page)->click();
}

void TabWidget::onTabClicked(int id) {
    pages->setCurrentIndex(id);
}
