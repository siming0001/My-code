#ifndef SQLITE_H
#define SQLITE_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QTime>
#include <QDateTime>    //2021-7-15 星期四 10：00:06
#include <QTimer>       //定时器 1s

namespace Ui {
class Sqlite;
}

class Sqlite : public QWidget
{
    Q_OBJECT

public:
    explicit Sqlite(QWidget *parent = nullptr);
    ~Sqlite();

    void connDB();      //连接数据库

    void createTableUsers(); //创建用户表存储账号密码

    void createTableData();//创建数据表存放下位机数据

    void insertUsersData(QString account,QString password);  //插入用户数据

    int insertData(QString CardId,QString name,int status);  //插入传感器数据

    void selectUsersData(QString account);  //查找用户账号数据

    void selectData(QString CardId);//查找传感器数据

//    void deleteData();  //删除数据

    void updateUsers(QString account,QString newpassword);  //更新数据

    int updateData(QString CardId,int status);

    void selectStatus(int status);

    void selectToatlCount();

    QString data;

    int status;

    QString cardid;

    QString name;

    QString count;

    QString totalCount;


private:
    Ui::Sqlite *ui;

    int OK=1;

    int ERROR=0;
};

#endif // SQLITE_H
