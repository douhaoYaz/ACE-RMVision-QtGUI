#include "include/image_widget.h"
#include "interface.h"
#include <QVBoxLayout>
#include <opencv2/imgproc.hpp>
#include <iostream>
QHash<QString, QPair<cv::Mat*, int>> ImageWidget::imgs = { };

void ImageWidget::registerImage(const QString& name, cv::Mat* img, int format) {
    imgs.insert(name, { img, format });
}

ImageWidget::ImageWidget(QWidget* parent): QWidget(parent) {
    QVBoxLayout* l = new QVBoxLayout();
    img = new QLabel(this);
    img->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    l->addWidget(img, 0, Qt::AlignCenter);
    selector = new QComboBox(this);
    selector->setFixedWidth(200);
    for (auto iter=imgs.begin(); iter != imgs.end(); ++iter)
        selector->addItem(iter.key());
    selector->setCurrentIndex(-1);
    selector->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    l->addWidget(selector, 0, Qt::AlignCenter);
    this->setLayout(l);

    connect(selector, SIGNAL(currentIndexChanged(const QString&)),
            this, SLOT(onImageChanged(const QString&)));
}

void ImageWidget::closeEvent(QCloseEvent* event) {
    emit closed();
    QWidget::closeEvent(event);
}

void ImageWidget::refresh() {
    if (img_src && img_src->empty() == false) {
        cv::Mat cvimg;
        switch (format) {
        case UserInterface::UI_8U|UserInterface::UI_BGR:
            cv::cvtColor(*img_src, cvimg, cv::COLOR_BGR2RGB);
            img->setPixmap(QPixmap::fromImage(QImage(cvimg.data, cvimg.cols,
                    cvimg.rows, QImage::Format_RGB888)));
            break;
        case UserInterface::UI_8U|UserInterface::UI_HSV:
            cv::cvtColor(*img_src, cvimg, cv::COLOR_HSV2RGB);
            img->setPixmap(QPixmap::fromImage(QImage(cvimg.data, cvimg.cols,
                    cvimg.rows, QImage::Format_RGB888)));
            break;
        case UserInterface::UI_8U|UserInterface::UI_RBG:
            img->setPixmap(QPixmap::fromImage(QImage(img_src->data, img_src->cols,
                    img_src->rows, QImage::Format_RGB888)));
            break;
        case UserInterface::UI_8U|UserInterface::UI_GRAY:
            img->setPixmap(QPixmap::fromImage(QImage(img_src->data, img_src->cols,
                    img_src->rows, QImage::Format_Grayscale8)));
            break;
        default:
            return;
        };

        img->adjustSize();
        img->show();
    }
}

void ImageWidget::onImageChanged(const QString& txt) {
    auto iter = imgs.find(txt);
    if (iter == imgs.end())
        return;
    img_src = iter.value().first;
    format = iter.value().second;
}
