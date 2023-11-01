#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
#include "xlsxdocument.h"

void createRTGS_instructions(QString excelFilePath, QString dirPath)
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
    // Load the Excel file
    QXlsx::Document excelFile(excelFilePath);

    // Assuming the data is in the first sheet
    QXlsx::Worksheet* sheet = excelFile.currentWorksheet();

    QSqlQuery query;
    QString createQuery = "CREATE TABLE RTGS_instructions (id VARCHAR(20) PRIMARY KEY,"
                          "check_status VARCHAR(10)," //勾单状态
                          "delivery_number VARCHAR(50),"      //交收编号
                          "securities_account VARCHAR(20),"   //证券账户
                          "fund_account VARCHAR(10),"         //资金账号
                          "actual_payment DECIMAL(17, 2),"    //实际收付
                          "delivery_status VARCHAR(10))";//交收状态
    if (!query.exec(createQuery)) {
        qDebug() << "创建表失败：" << query.lastError().text();
        return;
    }

    // Assuming the data starts from row 2 (skip header row)
    bool createTableSuccess = true;
    for (int row = 2; row <= sheet->dimension().lastRow(); ++row) {
        QString check_status = sheet->read(row, 2).toString();
        QString delivery_number = "";
        QString securities_account = sheet->read(row, 3).toString();
        QString fund_account = sheet->read(row, 4).toString();
        QString actual_payment = sheet->read(row, 5).toString();
        QString delivery_status = sheet->read(row, 6).toString();

        // 检查所有元素是否都为空
        if (check_status.isEmpty() && delivery_number.isEmpty() && securities_account.isEmpty() && fund_account.isEmpty() && actual_payment.isEmpty() && delivery_status.isEmpty()) {
            continue;  // 跳过当前行，不执行插入操作
        }

        QString insertQuery = "INSERT INTO RTGS_instructions (id, check_status, delivery_number, securities_account,fund_account,actual_payment,delivery_status) "
                              "VALUES (? , ? , ? , ?, ? , ? , ?)";
        query.prepare(insertQuery);
        query.addBindValue(QString::number(row - 1));
        query.addBindValue(check_status);
        query.addBindValue(delivery_number);
        query.addBindValue(securities_account);
        query.addBindValue(fund_account);
        query.addBindValue(actual_payment);
        query.addBindValue(delivery_status);

        if (createTableSuccess && !query.exec()) {
            qDebug() << "插入数据失败：" << query.lastError().text();
            createTableSuccess = false;
        }

        if (!createTableSuccess) {
            // 在插入数据失败后的处理逻辑
            // 可以选择终止循环或进行其他处理
            break;
        }
    }

    // Close database connection
    db.close();

}

