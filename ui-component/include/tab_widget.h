#ifndef TAB_WIDGET_H
#define TAB_WIDGET_H

#include <QButtonGroup>
#include <QStackedWidget>
#include <QHBoxLayout>

class TabWidget: public QWidget {
    Q_OBJECT
public:
    explicit TabWidget(QWidget* parent=nullptr);

    void add(QWidget* widget, const QString& txt_btn);

    int getCurrentIndex();

    void switchPage(int page);

public slots:
    void onTabClicked(int id);


protected:
    QButtonGroup* gp_tab;
    QStackedWidget* pages;
    QHBoxLayout* hlayout_btn;
};


#endif //TAB_WIDGET_H
