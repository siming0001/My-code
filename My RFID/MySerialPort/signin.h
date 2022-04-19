#ifndef SIGNIN_H
#define SIGNIN_H

#include <QWidget>

#include <QPaintEvent>
#include <QPainter>
#include <QImage>
#include "sqlite.h"

namespace Ui {
class signin;
}

class signin : public QWidget
{
    Q_OBJECT

public:
    explicit signin(QWidget *parent = 0);
    ~signin();

private slots:
    void on_cancelBtn_clicked();

    void on_confirmBtn_clicked();

protected:
    void paintEvent(QPaintEvent *);//绘画事件

signals:
    void backSignal();

private:
    Ui::signin *ui;

     QImage img; //存放背景图片

     Sqlite sqlite;
};

#endif // SIGNIN_H
