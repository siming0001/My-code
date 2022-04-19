#include "modify.h"
#include "ui_modify.h"
#include "sqlite.h"

modify::modify(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::modify)
{
    ui->setupUi(this);

    //去窗口边框
            setWindowFlags(Qt::FramelessWindowHint | windowFlags());

    //把窗口背景设置为透明;
            setAttribute(Qt::WA_TranslucentBackground);
    img.load(":/img/triangles-1430105_640.png");
    ui->label->setStyleSheet("QLabel{"
            "color:white;}");
    ui->label_2->setStyleSheet("QLabel{"
            "color:white;}");
    ui->label_3->setStyleSheet("QLabel{"
            "color:white;}");
}

modify::~modify()
{
    delete ui;
}

void modify::on_cancelBtn_clicked()
{
    emit backSignal();
    this->hide();
}

void modify::on_confirmBtn_clicked()
{
    sqlite.selectUsersData(ui->lineEdit->text());
    if(ui->lineEdit_2->text() == sqlite.data){
        sqlite.updateUsers(ui->lineEdit->text(),ui->lineEdit_3->text());
        emit backSignal();
        this->close();
    }
    else{
        QMessageBox::information(
                    NULL,
                    "修改密码",
                    "与原密码不符");
    }
}

void modify::paintEvent(QPaintEvent *)
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
