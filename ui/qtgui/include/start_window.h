#ifndef START_WINDOW_H
#define START_WINDOW_H

#include "button.h"
#include "title_bar.h"
#include "tab_widget.h"
#include <QDialog>
#include <QComboBox>

class StartWindow;

class CapturePage: public QWidget {
    Q_OBJECT
public:
    CapturePage(QWidget* parent=nullptr);

    void getParameter(QVariant& cap1, QVariant& cap2, int& type);

private:
    QLabel* lbl_cap1;
    QLabel* lbl_cap2;
    QLabel* lbl_type;
    QComboBox* selector_cap1;
    QComboBox* selector_cap2;
    QComboBox* selector_type;
    friend class StartWindow;
};

class VideoPage: public QWidget {
    Q_OBJECT
public:
    VideoPage(QWidget* parent=nullptr);

    void getParameter(QVariant& video);

private:
    QLabel* lbl_video;
    QLabel* lbl_fps;
    QComboBox* selector;

    friend class StartWindow;
};

class StartWindow: public QDialog {
    Q_OBJECT
public:
    explicit StartWindow(QWidget* parent=nullptr);

    void closeEvent(QCloseEvent* event) override;

private:
    TitleBar* titlebar;
    QLabel* icon;

    TabWidget* widget_tab;
    CapturePage* page_cap;
    VideoPage* page_video;

    IconButton* btn_start;

private slots:
    void onStart();

signals:
    void openByMode(int mode, const QVariant& param1, const QVariant& param2, int type);
};

#endif // START_WINODW_H
