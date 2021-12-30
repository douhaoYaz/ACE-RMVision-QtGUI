#ifndef TITLE_BAR_H
#define TITLE_BAR_H

#include "button.h"
#include <QLabel>


class TitleBar: public QWidget {
    Q_OBJECT
public:
    explicit TitleBar(QWidget* parent=nullptr);

protected:  //@override

    /**
     * @brief 鼠标按下事件处理函数
     * @param event，参数，事件
     */
    virtual void mousePressEvent(QMouseEvent* event) override;
    /**
     * @brief 鼠标移动事件处理函数
     * @param event，参数，事件
     */
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    /**
     * @brief 鼠标释放事件处理函数
     * @param event，参数，事件
     */
    virtual void mouseReleaseEvent(QMouseEvent* event) override;

    virtual void paintEvent(QPaintEvent *event) override;

    virtual bool eventFilter(QObject* watched, QEvent* event) override;

protected slots:

    void onMinimum();

    void onClose();

protected:
    bool is_pressing;
    QPoint pt_start;

    IconButton* btn_close;
    IconButton* btn_minimum;
    QLabel* title;
    QLabel* icon;
};

#endif //TITLE_BAR_H
