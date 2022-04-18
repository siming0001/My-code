#include "widget.h"
#include "sqlite.h"
#include "login.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Login b;
    b.show();

    return a.exec();
}
