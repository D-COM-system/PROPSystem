#include "widget.h"
#include "ui_widget.h"
#include "user.cpp"
#include "dashboard.h"

#include <QApplication>
#include <QCoreApplication>
#include <QFile>
#include <QVBoxLayout>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    createUser(":/Data/user.xlsx",QCoreApplication::applicationDirPath());
    QFile qssFile(":/qss/dashboard.qss");//1.
    if(qssFile.open(QFile::ReadOnly)){//2.
        a.setStyleSheet(qssFile.readAll());//3.
    }
    qssFile.close();
    Widget w;
    // 去掉窗体的默认标题栏

    w.show();
    dashboard w2;

    QObject::connect(&w, SIGNAL(showdashboard()), &w2, SLOT(show()));
    // 将信号连接到槽函数

    return a.exec();
}
