#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QToolButton>
#include "sqlite.h"
#include <QDebug>

namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    ~Login();

    Sqlite sqlite;

private slots:
    void on_toolButton_clicked();

    void on_toolButton_2_clicked();

    void on_signBtn_clicked();//注册

    void on_loginBtn_clicked();//登录

    void showOwn();//让自己显示出来

    void on_modifyBtn_clicked();// 修改密码


private:
    Ui::Login *ui;
};

#endif // LOGIN_H
