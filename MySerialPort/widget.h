#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QDebug>
#include <QTime>
#include "sqlite.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    QString getTime();  //获取时间

private slots:

    void readyReadSlot();       //接受数据槽函数

    void on_clearBtn_clicked();

    void on_confirmBtn_clicked();

    void on_selectBtn_clicked();

private:
    Ui::Widget *ui;
    QSerialPort *serialPort;    //串口
    Sqlite sqlite;
    int putInFlag=0;
    bool mode=0;
};
#endif // WIDGET_H
