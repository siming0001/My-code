#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include "sqlite.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setFixedSize(800,600);
    this->setWindowTitle("考勤系统");
    serialPort = new QSerialPort(this);

    serialPort->setPortName("COM7");
    if(!serialPort->open(QIODevice::ReadWrite)){
        QMessageBox::information(
                    this,
                    "串口打开失败！",
                    "由于未知原因，串口打开失败！");
        return;
    }
    serialPort->setBaudRate(QSerialPort::Baud9600);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);
    connect(serialPort,SIGNAL(readyRead()),this,SLOT(readyReadSlot()));
    ui->lineEdit->setEnabled(false);
}

Widget::~Widget()
{
    delete ui;
}

QString Widget::getTime()
{
    QString dt = QTime::currentTime().toString("hh:mm:ss");
    dt = "[" + dt +"]";
    return dt;
}

void Widget::readyReadSlot()
{
        if(serialPort->bytesAvailable()>4){
        QByteArray Flag = serialPort->read(1);//读取模式位
        QByteArray data = serialPort->read(4);//读取卡号

        sqlite.selectData(QString(data.toHex()));//查询卡号信息

        if(Flag.toHex()=="ff")
            mode=!mode;//考勤查询状态位

        if(Flag.toHex()=="00")//00：签到模式
        {
            ui->lineEdit->clear();
            ui->lineEdit->setEnabled(false);
            ui->selectBtn->setEnabled(true);
            if(sqlite.cardid==QString(data.toHex()))//查询是否已经入库
            {
                if(sqlite.status==1)//已签退
                {
                    ui->textEdit->append(getTime() + "用户名："+sqlite.name+"(卡号：" + data.toHex() + ")的用户，您好！您已成功签到！");
                    sqlite.updateData(QString(data.toHex()),0);
                }else
                    ui->textEdit->append(getTime() + "用户名："+sqlite.name+"(卡号：" + data.toHex() + ")的用户，您已处于签到状态，请勿重复签到！");
            }
            else
            {
                ui->textEdit->append(getTime() + "卡号：" + data.toHex() + "的用户信息尚未录入数据库！");
            }
//            qDebug()<<"cardid"<<sqlite.cardid;
//            qDebug()<<"data"<<QString(data.toHex());
        }
        else if(Flag.toHex()=="01")//01：签退模式
        {
            ui->lineEdit->clear();
            ui->lineEdit->setEnabled(false);
            ui->selectBtn->setEnabled(true);
            if(sqlite.cardid==QString(data.toHex()))//查询是否已经入库
            {
                if(sqlite.status==0)//已签到
                {
                    ui->textEdit->append(getTime() + "用户名："+sqlite.name+"(卡号：" + data.toHex() + ")的用户，您好！您已成功签退！");
                    sqlite.updateData(QString(data.toHex()),1);
                }else
                    ui->textEdit->append(getTime() + "用户名："+sqlite.name+"(卡号：" + data.toHex() + ")的用户，您已处于签退状态，请勿重复签退！");
            }
            else
            {
                ui->textEdit->append(getTime() + "卡号：" + data.toHex() + "的用户信息尚未录入数据库！");
            }
        //ui->textEdit->append(getTime() +  QString(data));
        qDebug()<<"flag"<<Flag.toHex();
        }
        else if(Flag.toHex()=="02")//录卡模式
        {
            ui->lineEdit->setEnabled(true);
            ui->selectBtn->setEnabled(false);
            while(1)
            {
                if(putInFlag)
                {
                    if(sqlite.insertData(QString(data.toHex()),QString(sqlite.name),1))//状态位置1，插入时默认表示已签退,未签到。
                        ui->textEdit->append(getTime() + "用户名："+sqlite.name+"(卡号：" + data.toHex() + ")的用户， 您好！您已成功录入信息！");
                    else
                        ui->textEdit->append(getTime() + "卡号：" + data.toHex() + "的用户信息已录入数据库！请勿重复录入！");
                    ui->lineEdit->clear();
                    putInFlag=0;
                }
                break;
            }
        }
    }
}

void Widget::on_clearBtn_clicked()
{
    ui->textEdit->clear();
}


void Widget::on_confirmBtn_clicked()
{
    if(ui->lineEdit->text()!="")
    {
        sqlite.name = ui->lineEdit->text();
        putInFlag=1;
        ui->lineEdit->setEnabled(false);
    }else
        ui->textEdit->append(getTime() + "输入的用户名不能为空!");
}


void Widget::on_selectBtn_clicked()
{
    sqlite.selectToatlCount();
    QString count0,count1;
    if(mode)
    {
        sqlite.selectStatus(0);
        count0=sqlite.count;
        sqlite.selectStatus(1);
        count1=sqlite.count;
        ui->textEdit->append(getTime() + "已签退的人数为："+count1+ "人，未签退的人数为："+count0+"人,总人数为："+sqlite.totalCount+"人");
    }
    else
    {
        sqlite.selectStatus(1);
        count1=sqlite.count;
        sqlite.selectStatus(0);
        count0=sqlite.count;
        ui->textEdit->append(getTime() + "已签到的人数为："+count0+ "人，未签到的人数为："+count1+"人,总人数为："+sqlite.totalCount+"人");
    }
}

