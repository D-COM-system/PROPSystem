#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
#include "xlsxdocument.h"

void createMaximumQuotaRoutineDeclarationDataTable(QString excelFilePath, QString dirPath)
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
    QString createQuery = "CREATE TABLE MaximumQuotaRoutineDeclarationDataTable (GUID varchar(64),"
                          "importResultCode varchar(10),"
                          "importResultState varchar(10),"
                          "institutionCodeResult varchar(20),"
                          "institutionNameResult varchar(50),"
                          "controlResult varchar(10),"
                          "TotalAssetsResult int(13),"
                          "MaxAccountResult int(13))";
    if (!query.exec(createQuery)) {
        qDebug() << "创建表失败：" << query.lastError().text();
        return;
    }

    // Assuming the data starts from row 2 (skip header row)
    bool createTableSuccess = true;
    for (int row = 2; row <= sheet->dimension().lastRow(); ++row) {
        QString GUID = "";
        QString importResultCode = sheet->read(row, 2).toString();
        QString importResultState = sheet->read(row, 3).toString();
        QString institutionCodeResult = sheet->read(row, 4).toString();
        QString institutionNameResult = sheet->read(row, 5).toString();
        QString controlResult = sheet->read(row, 6).toString();
        int TotalAssetsResult = sheet->read(row, 7).toInt();
        int MaxAccountResult = sheet->read(row, 8).toInt();

        // 检查所有元素是否都为空
        if (GUID.isEmpty() && importResultCode.isEmpty() && importResultState.isEmpty() && institutionCodeResult.isEmpty() && institutionNameResult.isEmpty() && controlResult.isEmpty() && TotalAssetsResult == 0 && MaxAccountResult == 0) {
            continue;  // 跳过当前行，不执行插入操作
        }

        QString insertQuery = "INSERT INTO MaximumQuotaRoutineDeclarationDataTable (GUID,importResultCode, importResultState, institutionCodeResult,institutionNameResult,controlResult,TotalAssetsResult,MaxAccountResult) "
                              "VALUES (? , ? , ? , ? , ? , ? , ?, ?)";
        query.prepare(insertQuery);
        query.addBindValue(GUID);
        query.addBindValue(importResultCode);
        query.addBindValue(importResultState);
        query.addBindValue(institutionCodeResult);
        query.addBindValue(institutionNameResult);
        query.addBindValue(controlResult);
        query.addBindValue(TotalAssetsResult);
        query.addBindValue(MaxAccountResult);

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

