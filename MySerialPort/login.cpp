#include "login.h"
#include "ui_login.h"
#include "signin.h"
#include "modify.h"
#include "widget.h"

Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);

    Sqlite sqlite;
    sqlite.connDB();
    sqlite.createTableData();
    sqlite.createTableUsers();

    //去窗口边框
            setWindowFlags(Qt::FramelessWindowHint | windowFlags());

    //把窗口背景设置为透明;
            setAttribute(Qt::WA_TranslucentBackground);

    //输入密码的时候就显示圆点
             ui->passwordEdit->setEchoMode(QLineEdit::Password);
}

Login::~Login()
{
    delete ui;
}

void Login::on_toolButton_clicked()
{
    showMinimized();
}

void Login::on_toolButton_2_clicked()
{
    close();
}

void Login::on_signBtn_clicked()
{
    signin *s = new signin;
    connect(s,SIGNAL(backSignal()),
          this,SLOT(showOwn()));
    s->show();
    this->hide();
}


void Login::on_loginBtn_clicked()
{
    sqlite.selectUsersData(ui->usernameEdit->text());
    if(ui->usernameEdit->text() !="")
    {
        if(ui->passwordEdit->text() == sqlite.data){
//        livingRoom *livingroom = new livingRoom;
//        connect(livingroom,SIGNAL(backSignal()),this,SLOT(showOwn()));
//        livingroom->show();
             Widget *widget=new Widget;
//            connect(widget,SIGNAL(backSignal()),this,SLOT(showOwn()));
            widget->show();
            this->close();
        }else{
            qDebug()<<"登录失败"<<sqlite.data;
            QMessageBox::information(NULL,
                                    "登录",
                                    "密码错误");
        }
    }else{
        QMessageBox::information(NULL,
                                 "登录",
                                 "账号不能不为空");
    }
}

void Login::showOwn()
{
    this->show();
}

void Login::on_modifyBtn_clicked()
{
    modify *m = new modify;
    connect(m,SIGNAL(backSignal()),
          this,SLOT(showOwn()));
    m->show();
    this->hide();
}

