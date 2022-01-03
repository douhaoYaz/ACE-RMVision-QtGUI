#ifndef MAIN_WINODW_H
#define MAIN_WINDOW_H

#include "defines.h"
#include "interface.h"
#include "ui.h"
#include "edit_widget.h"
#include "show_widget.h"
#include "image_widget.h"
#include "setting.h"
#include <QButtonGroup>
#include <QRadioButton>
#include <QMainWindow>
#include <QTimer>
#include <memory>

class MainWindow;

class ControlArea : public QWidget {
    Q_OBJECT
public:
    explicit ControlArea(QWidget* parent=nullptr);

    void paintEvent(QPaintEvent* event) override;

signals:
    void changeMode(int mode);
    void save(bool is_save);

private:
    QButtonGroup* radios_mode;
    QButtonGroup* radios_color;
    QRadioButton* radio_armor;
    QRadioButton* radio_buff;
    QRadioButton* radio_top;
    QRadioButton* radio_red;
    QRadioButton* radio_blue;
    CheckableButton* btn_save;

    friend class MainWindow;
};

class MainWindow: public QMainWindow {
    Q_OBJECT
public:

    static MainWindow* getMainWindow();

    void log(const QString& str);

    void addImageWidget();

    void registerTable(const QString& name_table,
            const QVector<QPair<QString, Pointer>>& kvs);

    void setCurrentTable(const QString& name);

    void createEditItem(const QString& name);

    void registerTrackBar(const QString& name_bar,
            const QString& key, int min, int max, int* pvalue);

    void setSetting(const Setting& setting);

    Setting& getSettingReference();

    void setTasks(std::shared_ptr<ThreadRunnerBase> task);

    void closeEvent(QCloseEvent* event) override;

private:
    static MainWindow* win;
    Setting setting;

    std::shared_ptr<ThreadRunnerBase> task;
    QDockWidget* dock_edit;
    QDockWidget* dock_show;
    QDockWidget* dock_ctrl;
    QDockWidget* dock_log;
    QDockWidget* dock_main;

    explicit MainWindow(QWidget* parent=nullptr);

    void initializeMenuBar();

    void initializeMainImage();

    void initializeEditWidget();

    void initializeShowWidget();

    void initializeControlWidget();

    void initializeLogWidget();

    void initializeImageWidget();

private slots:
    void onInitializeSetting(int mode, const QVariant& param1,
            const QVariant& param2, int type);

    void onModeChanged(int mode);

    void onSave(bool clicked);

    void onColorChanged(int color);

};

#endif // MAIN_WIDGET_H
