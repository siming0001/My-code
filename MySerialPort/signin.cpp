#include "signin.h"
#include "ui_signin.h"
#include "login.h"

signin::signin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::signin)
{
    ui->setupUi(this);
    //去窗口边框
            setWindowFlags(Qt::FramelessWindowHint | windowFlags());

    //把窗口背景设置为透明;
            setAttribute(Qt::WA_TranslucentBackground);
    img.load(":/img/water-drops-6373296_640.jpg");

    ui->label->setStyleSheet("QLabel{"
            "color:white;}");
    ui->label_2->setStyleSheet("QLabel{"
            "color:white;}");
    ui->label_3->setStyleSheet("QLabel{"
            "color:white;}");



}

signin::~signin()
{
    delete ui;
}

void signin::on_cancelBtn_clicked()
{
    emit backSignal();
    this->hide();
}

void signin::on_confirmBtn_clicked()
{
    if(ui->lineEdit_2->text() == ui->lineEdit_3->text())
    {
        sqlite.insertUsersData(ui->lineEdit->text(),ui->lineEdit_2->text());
        Login *login = new Login;
        connect(login,SIGNAL(backSignal()),this,SLOT(showOwn()));
        login->show();
        this->close();
    }else{
        QMessageBox::information(
                    NULL,
                    "注册账号",
                    "两次输入的密码不同！");
    }

}

void signin::paintEvent(QPaintEvent *)
{
    //绘画人->画在哪
    QPainter painter(this);
    //图片的大小
    QRect source(0,0,img.width(),img.height());
    //目标区域的大小
    QRect target(0,0,this->width(),this->height());
    //绘画
    painter.drawImage(target,img,source);

}
