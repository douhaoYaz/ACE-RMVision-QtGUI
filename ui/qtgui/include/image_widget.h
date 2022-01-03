#ifndef IMAGE_WIDGET_H
#define IMAGE_WIDGET_H

#include <QWidget>
#include <opencv2/core.hpp>
#include <QComboBox>
#include <QLabel>

class ImageWidget: public QWidget {
    Q_OBJECT
public:
    static void registerImage(const QString& name, cv::Mat* img,
            int format);

public:
    explicit ImageWidget(QWidget* widget=nullptr);

    void closeEvent(QCloseEvent* event) override;
signals:
    void closed();

public slots:
    void refresh();

protected:
    static QHash<QString, QPair<cv::Mat*, int>> imgs;

protected:
    QLabel* img;
    QComboBox* selector;
    cv::Mat* img_src;
    int format;

protected slots:

    void onImageChanged(const QString& txt);
};

#endif // IMAGE_WIDGET_H
