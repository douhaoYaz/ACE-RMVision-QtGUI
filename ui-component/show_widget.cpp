#include "include/show_widget.h"
#include <QHeaderView>
#include <QDebug>
void ShowWidgetItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& opt,
        const QModelIndex& idx) const {
    QStyleOptionViewItem opt_view(opt);
    QPalette& p = opt_view.palette;
//    p.setColor(QPalette::Text, idx.data(Qt::ForegroundRole).value<QColor>());
    p.setColor(QPalette::Text, QColor(255, 255, 255));
    return QItemDelegate::paint(painter, opt_view, idx);
}

ShowWidget::ShowWidget(QWidget* parent): QTableView(parent), table_cur(nullptr) {
//    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
//    this->setSelectionMode(QAbstractItemView::NoSelection);
//    this->setSelectionMode(QAbstractItemView::NoSelection);
    QStandardItemModel* model_default = new QStandardItemModel();
    this->setModel(model_default);
    this->setItemDelegate(new ShowWidgetItemDelegate(this));
}

void ShowWidget::addModel(const QString& name, const QStringList& keys) {
    QStandardItemModel* model = new QStandardItemModel();

    model->setRowCount(keys.length());
    model->setColumnCount(2);
    QHash<QString, int> map_idx;
    int i = 0;
    for (QString key: keys) {
        QStandardItem* item_key = new QStandardItem(key);
        item_key->setFlags(item_key->flags() & ~Qt::ItemIsEnabled
                & ~Qt::ItemIsSelectable);
        model->setItem(i, 0, item_key);
        QStandardItem* item_value = new QStandardItem();
        item_value->setFlags(item_key->flags());
        model->setItem(i, 1, item_value);
        map_idx.insert(key, i);
        i++;
    }
    ModelTable table;
    table.name = name;
    table.model = model;
    table.idxs = map_idx;

    models.insert(name, table);
}

QString ShowWidget::getCurrentTableName() const {
    if (table_cur)
        return table_cur->name;
    return "";
}

void ShowWidget::changeModel(const QString& name) {
    auto iter = models.find(name);
    if (iter != models.end()) {
        this->setModel(iter->model);
        auto header_h = this->horizontalHeader();
        header_h->hide();
        header_h->setSectionResizeMode(QHeaderView::Stretch);
        header_h->setSectionResizeMode(0, QHeaderView::Fixed);
        auto header_v = this->verticalHeader();
        header_v->hide();
//        table_cur = const_cast<decltype(table_cur)>(&*iter);
        table_cur = &*iter;
    }
}

void ShowWidget::setItem(int r, const QString& val) {
    QStandardItem* item = new QStandardItem(val);
    item->setFlags(item->flags() & ~Qt::ItemIsEnabled & ~Qt::ItemIsSelectable);
    table_cur->model->setItem(r, 1, item);
}

void ShowWidget::setItem(const QString& key, const QString& val) {
    if (table_cur) {
        auto iter = table_cur->idxs.find(key);
        if (iter != table_cur->idxs.end())
            setItem(iter.value(), val);
    }
}
