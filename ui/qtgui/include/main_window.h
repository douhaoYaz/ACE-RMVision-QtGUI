#ifndef MAIN_WINODW_H
#define MAIN_WINDOW_H

#include "defines.h"
#include "interface.h"
#include "ui.h"
#include "edit_widget.h"
#include "show_widget.h"
#include "setting.h"
#include <QButtonGroup>
#include <QRadioButton>
#include <QMainWindow>
#include <QTimer>
#include <memory>

class MainWindow: public QMainWindow {
    Q_OBJECT
public:

    static MainWindow* getMainWindow();

    void log(const QString& str);

    void registerTable(const QString& name_table,
            const QVector<QPair<QString, Pointer>>& kvs);

    void setCurrentTable(const QString& name);

    void createEditItem(const QString& name);

    void registerTrackBar(const QString& name_bar,
            const QString& key, int min, int max, int* pvalue);

    void closeEvent(QCloseEvent* event) override;

private:
    static MainWindow* win;

    explicit MainWindow(QWidget* parent=nullptr);

};

#endif // MAIN_WIDGET_H
