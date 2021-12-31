#ifndef COLLAPSE_BOX_H
#define COLLAPSE_BOX_H

#include "button.h"
#include <QMap>

class CollapseItem: public QWidget {
    Q_OBJECT
public:
    CollapseItem(const QString& title, QWidget* parent=nullptr);

    void addItem(QWidget* item);

    void showOrHide();

    QString getName();

signals:
    void clicked();

protected slots:
    void onClick();

protected:
    CheckableButton* btn;
    QString name;
    QVector<QWidget*> items;
    bool visible;
};

class CollapseBox: public QWidget {
    Q_OBJECT
public:

    explicit CollapseBox(QWidget* parent=nullptr);

    void addItem(CollapseItem* item);

    CollapseItem* getItem(const QString& name);

protected slots:
    void onClick();

protected:
    QHash<QString, CollapseItem*> items;
};

#endif // COLLAPSE_BOX_H
