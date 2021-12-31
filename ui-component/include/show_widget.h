#ifndef SHOW_WIDGET_H
#define SHOW_WIDGET_H

#include <QTableView>
#include <QString>
#include <QStandardItemModel>
#include <QItemDelegate>
#include <QStyleOptionViewItem>
#include <unordered_map>
#include <QVector>

class ShowWidgetItemDelegate: public QItemDelegate {
    Q_OBJECT
public:
    explicit ShowWidgetItemDelegate(QWidget* parent=nullptr): QItemDelegate(parent) { }

    void paint(QPainter* painter, const QStyleOptionViewItem& opt,
            const QModelIndex& idx) const override;
};

class ShowWidget: public QTableView {
    Q_OBJECT    

public:
    explicit ShowWidget(QWidget* parent=nullptr);

    void addModel(const QString& name, const QStringList& keys);

    QString getCurrentTableName() const;

    void setItem(int r, const QString& val);

    void setItem(const QString& key, const QString& name);

public slots:
    void changeModel(const QString& name);

private:
    struct ModelTable {
        QString name;
        QStandardItemModel* model;
        QHash<QString, int> idxs;
    };
    QHash<QString, ModelTable> models;
    ModelTable* table_cur;
};

#endif // SHOWWIDGET_H
