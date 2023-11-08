// GlobalData.h
#ifndef GLOBALDATA_H
#define GLOBALDATA_H

#include <QString>
#include <QList>

//用户Id
extern QString globalUserId;
//用户名
extern QString globalUserName;
//操作员号
extern QString globalOperatorAccount;
//有效导入条数
extern QString efficientImportNumber;
//业务提交数据
extern QList<QStringList> businessSubmissionsList;


#endif // GLOBALDATA_H
