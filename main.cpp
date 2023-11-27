#include "widget.h"
#include "ui_widget.h"
#include "user.cpp"
#include "AShareDividendDifferentiated.cpp"
#include "AShareDividendDifferentiatedEshade.cpp"
#include "AShareDividendDifferentiatedLogList.cpp"
#include "fundbalancequery.cpp"
#include "RTGS_instructions.cpp"
#include "MaximumQuotaRoutineDeclarationDataTable.cpp"
#include "dashboard.h"
#include <QApplication>
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QProcess>
#include <QTextCodec>
#include <QVBoxLayout>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    createUser(":/Data/user.xlsx",QCoreApplication::applicationDirPath());
    createRTGS_instructions(":/Data/RTGS.xlsx",QCoreApplication::applicationDirPath());
    createMaximumQuotaRoutineDeclarationDataTable(":/Data/shanghaihighquery.xlsx",QCoreApplication::applicationDirPath());
    createfundbalancequery(":/Data/PROP.xlsx",QCoreApplication::applicationDirPath());
    createAShareDividendDifferentiated(QCoreApplication::applicationDirPath());
    createAShareDividendDifferentiatedEshade(QCoreApplication::applicationDirPath());
    createAShareDividendDifferentiatedLogList(QCoreApplication::applicationDirPath());
    QFile qssFile(":/qss/dashboard.qss");//1.
    if(qssFile.open(QFile::ReadOnly)){//2.
        a.setStyleSheet(qssFile.readAll());//3.
    }
    qssFile.close();
    Widget w;

    w.show();
    dashboard w2;

    QObject::connect(&w, SIGNAL(showdashboard()), &w2, SLOT(show()));
    // 将信号连接到槽函数

    return a.exec();
}
