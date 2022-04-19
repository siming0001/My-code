#include "sqlite.h"
#include "ui_sqlite.h"
#include <QDebug>

Sqlite::Sqlite(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Sqlite)
{
    ui->setupUi(this);
}

Sqlite::~Sqlite()
{
    delete ui;
}

void Sqlite::connDB()
{
    QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("user.db");
    if(db.open())
    {
        qDebug()<<"数据库打开成功!";
    }
    else {
        qDebug()<<"数据库打开失败:"<<db.lastError().text();
    }
}

void Sqlite::createTableUsers()
{
    QString cmd="create table users("
                "account int primary key,"
                "password varchar(50) not null);";
    QSqlQuery query;
    if(query.exec(cmd)){
        qDebug()<<"建表成功！";
    }else {
        qDebug()<<"建表失败："<<query.lastError().text();
    }
}

void Sqlite::createTableData()
{
    QString cmd="create table data("
                "date datetime not null,"
                "CardId char(10) primary key,"
                "name char(10) not null,"
                "status int not null);";
    QSqlQuery query;
    if(query.exec(cmd)){
        qDebug()<<"建表成功！";
    }else {
        qDebug()<<"建表失败："<<query.lastError().text();
    }
}

void Sqlite::insertUsersData(QString account,QString password)
{
    QString cmd="insert into users values(""'%1','%2');";//%x用来占位
    //cmd=cmd.arg(1).arg("hello kitty").arg(2);//替换数据
    cmd=cmd.arg(account).arg(password);//替换数据
//    cmd=cmd.arg(3).arg("hello kitty").arg(2);//替换数据
    QSqlQuery query;
    if(query.exec(cmd)){
        qDebug()<<"数据插入成功";
        QMessageBox::information(
                    NULL,
                    "注册账号",
                    "注册成功");
    }else {
        qDebug()<<"数据插入失败："<<query.lastError().text();
        QMessageBox::information(NULL,
                                 "注册账号",
                                 "已存在该用户名");
    }
}

int Sqlite::insertData(QString CardId,QString name,int status)
{
    QString datetime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString cmd="insert into data values(""'%1','%2','%3','%4');";//%x用来占位
    cmd=cmd.arg(datetime).arg(CardId).arg(name).arg(status);//替换数据
    QSqlQuery query;
    if(query.exec(cmd)){
        qDebug()<<"数据插入成功";
        return OK;
    }else {
        qDebug()<<"数据插入失败："<<query.lastError().text();
        return ERROR;
    }
}

void Sqlite::selectUsersData(QString account)
{
    QString cmd = "select password from users where account = '%2';";
    cmd=cmd.arg(account);
    QSqlQuery query;
    if(query.exec(cmd)){
        qDebug()<<"查询成功！";
    }else{
        qDebug()<<"查询失败："<<
                  query.lastError().text();
    }

    while(query.next()){
        //id-0 name-1 age-2
//        qDebug()<<"account:"<<query.value(0).toString();
        qDebug()<<"password:"<<query.value(0).toString();
        data = query.value(0).toString();
//        qDebug()<<"age:"<<query.value(2).toString();
//        if(query.value(0).toString() != password)
//                qDebug()<<"登录失败！";
    }
}

void Sqlite::selectData(QString CardId)
{
    QString cmd = "select * from data where CardId = '%1';";
    cmd = cmd.arg(CardId);
    QSqlQuery query;
    if(query.exec(cmd)){
        qDebug()<<"查询成功！";
    }else{
        qDebug()<<"查询失败："<<
                  query.lastError().text();
    }

    while(query.next()){
        //id-0 name-1 age-2
//        qDebug()<<"datetime:"<<query.value(0).toString();
//        qDebug()<<"dataname:"<<query.value(1).toString();
//        qDebug()<<"data:"<<query.value(2).toString();
        cardid=query.value(1).toString();
        name=query.value(2).toString();
        status=query.value(3).toInt();
//        qDebug()<<"age:"<<query.value(2).toString();
    }
}

void Sqlite::updateUsers(QString account,QString newpassword)
{
    QString cmd = "update users set password = '%1' where account = '%2'";
    cmd = cmd.arg(newpassword).arg(account);
    QSqlQuery query;
    if(query.exec(cmd))
    {
        QMessageBox::information(
                    NULL,
                    "修改密码",
                    "修改成功");
        return;
    }else{
        qDebug()<<"修改失败："<<
                  query.lastError().text();
    }
}

int Sqlite::updateData(QString CardId,int status)
{
    QString cmd = "update data set status = '%1' where CardId = '%2'";
    cmd = cmd.arg(status).arg(CardId);
    QSqlQuery query;
    if(query.exec(cmd))
    {
        qDebug()<<"状态位修改成功！";
        return OK;
    }else{
        qDebug()<<"状态位修改失败："<<
                  query.lastError().text();
        return ERROR;
    }
}

void Sqlite::selectStatus(int status)
{
     QString cmd = "select count(status) from data where status = '%1';";
     cmd = cmd.arg(status);
     QSqlQuery query;
     if(query.exec(cmd))
     {
         qDebug()<<"查询成功";
     }else{
         qDebug()<<"查询失败"<<
                   query.lastError().text();
     }
     while(query.next()){
         count=query.value(0).toString();
         qDebug()<<"查询成功"<<count;
     }
}

void Sqlite::selectToatlCount()
{
    QString cmd = "select count(CardId) from data;";
    QSqlQuery query;
    if(query.exec(cmd))
    {
        qDebug()<<"查询成功";
    }else{
        qDebug()<<"查询失败"<<
                  query.lastError().text();
    }
    while(query.next()){
        totalCount=query.value(0).toString();
        qDebug()<<"查询成功"<<totalCount;
    }
}
