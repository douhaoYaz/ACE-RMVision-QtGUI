#ifndef BUTTON_H
#define BUTTON_H

#include <QPushButton>
#include <QString>

class Button: public QPushButton {
    Q_OBJECT
public:
    explicit Button(QWidget* parent=nullptr);
    explicit Button(const QString& txt, QWidget* parent=nullptr);
};

class IconButton: public QPushButton {
    Q_OBJECT
public:
    //IconButton(QImage common, QImage hover, QWidget* parant=nullptr);
    IconButton(const QString& common, const QString& hover={},
            QWidget* parant=nullptr);

//这样画出来有锯齿
protected:  //@override

    //void enterEvent(QEvent* event) override;

    //void leaveEvent(QEvent* event) override;

    void paintEvent(QPaintEvent* event) override;


protected:
    //QImage img_common;
    //QImage img_hover;
    //bool is_hover;
};

class CheckableButton: public QPushButton {
    Q_OBJECT
public:
    explicit CheckableButton(const QString& txt, QWidget* parent=nullptr);

protected:
    using QPushButton::setCheckable;

};

#endif // BUTTON_H
