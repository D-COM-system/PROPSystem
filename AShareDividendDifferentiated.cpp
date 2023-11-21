#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
#include "xlsxdocument.h"

void createAShareDividendDifferentiated(QString dirPath)
{
    QString dbName = "database.db";
    QString dbPath = dirPath + "./" + dbName;  // Use a relative path
//    qDebug() << dbPath;

    // Create database connection
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath);

    if (!db.open()) {
        QMessageBox::critical(nullptr, "错误", "数据库打开失败：" + db.lastError().text());
        return;
    }
//    // Load the Excel file
//    QXlsx::Document excelFile(excelFilePath);

//    // Assuming the data is in the first sheet
//    QXlsx::Worksheet* sheet = excelFile.currentWorksheet();

    QSqlQuery query;
    QString createQuery = "CREATE TABLE AShareDividendDifferentiated ("
                          "AShareDividendDifferentiatedID VARCHAR(64),"
                          "SBBH VARCHAR(40),"
                          "ZLLX VARCHAR(10),"
                          "RQ DATE,"
                          "YWBH VARCHAR(20),"
                          "ZQZH VARCHAR(30)"
                          ")";
    if (!query.exec(createQuery)) {
        qDebug() << "创建表失败：" << query.lastError().text();
        return;
    }

    // Assuming the data starts from row 2 (skip header row)
//    bool createTableSuccess = true;
//    for (int row = 2; row <= sheet->dimension().lastRow(); ++row) {
//        QString GUID = QString::number(row - 1);
//        QString username = sheet->read(row, 1).toString();
//        QString password = sheet->read(row, 2).toString();
//        QString operatorAccount = sheet->read(row, 3).toString();

//        // 检查所有元素是否都为空
//        if (GUID.isEmpty() && username.isEmpty() && password.isEmpty() && operatorAccount.isEmpty()) {
//            continue;  // 跳过当前行，不执行插入操作
//        }

//        QString insertQuery = "INSERT INTO PROPUser (userID,username, password, operatorAccount) "
//                              "VALUES (?, ?, ?, ?)";
//        query.prepare(insertQuery);
//        query.addBindValue(GUID);
//        query.addBindValue(username);
//        query.addBindValue(password);
//        query.addBindValue(operatorAccount);

//        if (createTableSuccess && !query.exec()) {
//            qDebug() << "插入数据失败：" << query.lastError().text();
//            createTableSuccess = false;
//        }

//        if (!createTableSuccess) {
//            // 在插入数据失败后的处理逻辑
//            // 可以选择终止循环或进行其他处理
//            break;
//        }
//    }

    // Close database connection
    db.close();

}

