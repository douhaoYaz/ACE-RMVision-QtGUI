#ifndef EDIT_WIDGET_H
#define EDIT_WIDGET_H

#include "collapse_box.h"
#include <QScrollArea>
#include <QVariant>
#include <QSpinBox>
#include <QLineEdit>
#include <functional>

struct EditData {
    int type;
    QString key;
    QVariant value;
};

template <typename _Type>
class ItemWidgetBase {
public:
    typedef std::function<void(_Type*, const _Type&)> Function;

    ItemWidgetBase(const Function& callback): func_callback(callback) { }

    inline void setCallbackFunction(const Function& callback) {
        func_callback = callback;
    }

    inline Function getCallbackFunction() const {
        return func_callback;
    }

protected:
    Function func_callback;
};

class TrackbarItemWidget: public QWidget, public ItemWidgetBase<int> {
    Q_OBJECT
public:
    TrackbarItemWidget(const QString& name, int w_key, int min, int max, int* pvalue,
            int value_default, const Function& callback={ },
            QWidget* parent=nullptr);

    void bind(int* integer);

private slots:
    void onValueChanged(int v);

private:
    int* i;
    QSpinBox* cnt;
};

class InputItemWidget: public QWidget, public ItemWidgetBase<std::string> {
    Q_OBJECT
public:
    InputItemWidget(const QString& name, int w_key, std::string* pvalue,
            const QString& value_default="", const Function& callback={ },
            QWidget* parent=nullptr);

    void bind(std::string* s);

private slots:
    void onTextChange(const QString& txt);

private:
    std::string* str;
    QLineEdit* input;

};

class EditItem: public CollapseItem {
    Q_OBJECT
public:
    EditItem(const QString& name, QWidget* parent=nullptr):
            CollapseItem(name, parent) { }

    void addInputItem(const QString& name, int w_key, std::string* pvalue,
            const std::function<void(std::string*, const std::string&)>& callback={});

    void addTrackbarItem(const QString& name, int w_key, int min, int max, int* pvalue,
            const std::function<void(int*, int)>& func_callback={});

private:

    QVector<EditData> datas;

};

class EditWidget: public QScrollArea {
    Q_OBJECT
public:
    explicit EditWidget(QWidget* parent=nullptr);

    void addItemBar(const QString& name);

    void addInputItem(const QString& name_bar, const QString& name_tag, std::string* pvalue,
            const std::function<void(std::string*, const std::string&)>& callback={});

    void addTrackbarItem(const QString& name_bar, const QString& name_tag,
            int min, int max, int* pvalue,
            const std::function<void(int*, int)>& callback={});
private:
    CollapseBox* box;
};

#endif // EDITWIDGET_H
