#include "MaximumAmountRegularDeclaration.h"
#include "ui_MaximumAmountRegularDeclaration.h"
#include "ui_importSuccessEshade.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>
#include <QFile>
#include <QCoreApplication>
#include <QTextStream>
#include <xlsxdocument.h>
#include <QFileDialog>

QString nowFilePath = "";

MaximumAmountRegularDeclaration::MaximumAmountRegularDeclaration(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MaximumAmountRegularDeclaration)
{
    ui->setupUi(this);

    QString dbName = "database.db";
    QString dbPath = QCoreApplication::applicationDirPath() + "./" + dbName;  // Use a relative path
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(dbPath);

    if (!database.open()) {
        QMessageBox::critical(nullptr, "错误", "无法打开数据库：" + database.lastError().text());
        return;
    }
    // 执行查询
    QSqlQuery query;
    QString createQuery = "CREATE TABLE historyHighQueryRecord (recordPeopleId varchar(64),"
                          "institutionCodeResult varchar(20),"
                          "institutionNameResult varchar(50),"
                          "controlResult varchar(10),"
                          "TotalAssetsResult int(13),"
                          "MaxAccountResult int(13))";
    if (!query.exec(createQuery)) {
        qDebug() << "创建表失败：" << query.lastError().text();
    }
    query.prepare("SELECT COUNT(*) FROM historyHighQueryRecord WHERE recordPeopleId = :recordPeopleId");
    query.bindValue(":recordPeopleId", globalUserId);
    if (query.exec()) {
        if (query.next()) {
            totalRows = query.value(0).toInt();
        }
        // 删除空行
    } else {
        QMessageBox::critical(nullptr, "错误", "查询执行失败：" + query.lastError().text());
        return;
    }
    // 关闭数据库连接
    database.close();
    currentPage = 0,
    pageSize = 50,
    totalPages = ((totalRows + pageSize - 1) / pageSize);
    QStringList headerLabels;
    headerLabels << "机构代码" << "机构名称" << "控制类别" << "净资本/合计资产总额（百万）" << "最高额度（百万）";
    ui->tableWidget_2->clear(); // 清空表格内容
    ui->tableWidget_2->setColumnCount(headerLabels.size());
    ui->tableWidget_2->setHorizontalHeaderLabels(headerLabels);
    ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //让tableWidget内容中的每个元素居中
    ui->tableWidget_2->setSelectionBehavior(QAbstractItemView::SelectRows);//设置整行选中
    ui->tableWidget_2->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter);//表头字体居中
    ui->tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers);//单元格不可编辑
    // 设置表头的底色和文字加粗
    QString styleSheet = "QHeaderView::section { background-color: #f5f5f5; font-weight: bold; }";
    // 将样式表应用于表格的表头
    ui->tableWidget_2->horizontalHeader()->setStyleSheet(styleSheet);
    if(totalRows != 0) {
        ui->tableWidget_2->setRowCount(pageSize);
    }
    ui->tableWidget_2->verticalHeader()->hide();
    updateTableDisplay();
    connect(ui->pushButton_7, &QPushButton::clicked, this, &MaximumAmountRegularDeclaration::topPageButton_clicked);
    connect(ui->pushButton_8, &QPushButton::clicked, this, &MaximumAmountRegularDeclaration::previousPageButton_clicked);
    connect(ui->pushButton_9, &QPushButton::clicked, this, &MaximumAmountRegularDeclaration::nextPageButton_clicked);
    connect(ui->pushButton_10, &QPushButton::clicked, this, &MaximumAmountRegularDeclaration::bottomPageButton_clicked);
    connect(ui->pushButton_21, &QPushButton::clicked, this, &MaximumAmountRegularDeclaration::chooseFile);//导入按钮
    connect(ui->pushButton_23, &QPushButton::clicked, this, &MaximumAmountRegularDeclaration::businessSubmissions);//提交按钮
}

MaximumAmountRegularDeclaration::~MaximumAmountRegularDeclaration()
{
    delete ui;
}

//包括选择和导入功能
void  MaximumAmountRegularDeclaration::chooseFile()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Select File", "", "CSV Files (*.csv);;Excel Files (*.xlsx)");
    if (filePath.isEmpty()) {
        qDebug() << "Selected File: isEmpty";
        // 这里可以处理所选文件的路径
    }
    QString nativePath = QDir::toNativeSeparators(filePath);

    // 将路径分割为各个部分
    QStringList pathParts = nativePath.split(QDir::separator(), QString::SkipEmptyParts);

    // 移除盘符（如Macintosh HD）
    if (pathParts.size() > 0) {
        pathParts.removeFirst();
    }

    // 按照指定格式拼接路径
    QString convertedPath = pathParts.join(" ▶ ");

    // 将路径分隔符转换回原生格式
    convertedPath = QDir::fromNativeSeparators(convertedPath);

//    ui->lineEdit_6->setText(convertedPath);
    nowFilePath = filePath;
    int successInsert = 0;
    //判断文件是否符合导入需求
    if(!judgeFile()) {
        return ;
    }
    QString dbName = "database.db";
    QString dbPath = QCoreApplication::applicationDirPath() + "./" + dbName;  // Use a relative path
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(dbPath);

    if (!database.open()) {
        QMessageBox::critical(nullptr, "错误", "无法打开数据库：" + database.lastError().text());
        return;
    }
    qDebug() << nowFilePath;
    // Load the Excel file
    QXlsx::Document excelFile(nowFilePath);
    // Assuming the data is in the first sheet
    QXlsx::Worksheet* sheet = excelFile.currentWorksheet();

    QSqlQuery query;
    // Assuming the data starts from row 2 (skip header row)
    for (int row = 2; row <= sheet->dimension().lastRow(); ++row) {
        QString recordPeopleId = globalUserId;
        QString institutionCodeResult = sheet->read(row, 1).toString();
        QString institutionNameResult = sheet->read(row, 2).toString();
        QString controlResult = sheet->read(row, 3).toString();
        int TotalAssetsResult = sheet->read(row, 4).toInt();
        int MaxAccountResult = sheet->read(row, 5).toInt();

        // 检查所有元素是否都为空
        if (recordPeopleId.isEmpty() && institutionCodeResult.isEmpty() && institutionNameResult.isEmpty() && controlResult.isEmpty() &&
            TotalAssetsResult == 0 && MaxAccountResult == 0) {
            continue;  // 跳过当前行，不执行插入操作
        }

        QString insertQuery = "INSERT INTO historyHighQueryRecord (recordPeopleId,institutionCodeResult,institutionNameResult,controlResult,TotalAssetsResult,MaxAccountResult)"
                              "VALUES (?, ?, ?, ?, ?, ?)";
        query.prepare(insertQuery);
        query.addBindValue(recordPeopleId);
        query.addBindValue(institutionCodeResult);
        query.addBindValue(institutionNameResult);
        query.addBindValue(controlResult);
        query.addBindValue(TotalAssetsResult);
        query.addBindValue(MaxAccountResult);

        if (!query.exec()) {
            qDebug() << "插入数据失败：" << query.lastError().text();
        } else {
            successInsert++;
        }
    }
    query.prepare("SELECT COUNT(*) FROM historyHighQueryRecord WHERE recordPeopleId = :recordPeopleId");
    query.bindValue(":recordPeopleId", globalUserId);
    if (query.exec()) {
        if (query.next()) {
            totalRows = query.value(0).toInt();
        }
        // 删除空行
    } else {
        QMessageBox::critical(nullptr, "错误", "查询执行失败：" + query.lastError().text());
        return;
    }
    // 关闭数据库连接
    currentPage = 0,
    pageSize = 50,
    totalPages = ((totalRows + pageSize - 1) / pageSize);
    // Close the database connection
    database.close();
    setImportNumber(QString::number(successInsert));
    importSuccessEshade *shade = new importSuccessEshade(this);
    shade->exec();
    updateTableDisplay();
}

void MaximumAmountRegularDeclaration::setImportNumber(const QString& data) {
    efficientImportNumber = data;
}

bool MaximumAmountRegularDeclaration::judgeFile() {
//    // Load the Excel file
//    QXlsx::Document excelFile(nowFilePath);
//    // Assuming the data is in the first sheet
//    QXlsx::Worksheet* sheet = excelFile.currentWorksheet();
//    if(sheet->dimension().lastColumn() != 9) {
//            return false;
//    }
//    if(sheet->dimension().lastRow() == 9) {
//            return false;
//    }
//    //    for (int row = 0; row <= 1; ++row) {
//    //        QString guid = sheet->read(row, 1).toString();
//    //        QString state = sheet->read(row, 2).toString();
//    //        QString institutionCode = sheet->read(row, 3).toString();
//    //        QString institutionName = sheet->read(row, 4).toString();
//    //        QString TotalAssets = sheet->read(row, 5).toString();
//    //        QString MaxAccount = sheet->read(row, 6).toString();
//    //        QString ControlClass = sheet->read(row, 7).toString();
//    //        QString dataEntryClerk = sheet->read(row, 8).toString();
//    //        qDebug() << guid;
//    //    }

    return true;
}

void MaximumAmountRegularDeclaration::updateTableDisplay()
{
    if(totalRows == 0) {
        ui->label_3->setText("无数据显示");
    } else {
        QString dbName = "database.db";
        QString dbPath = QCoreApplication::applicationDirPath() + "./" + dbName;  // Use a relative path
        QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName(dbPath);
        if (!database.open()) {
            QMessageBox::critical(nullptr, "错误", "无法打开数据库：" + database.lastError().text());
            return;
        }
        QSqlQuery query;
        QString queryString = "SELECT COUNT(*) FROM historyHighQueryRecord WHERE recordPeopleId = :recordPeopleId";

        query.prepare(queryString);
        query.bindValue(":recordPeopleId", globalUserId);
        if (query.exec()) {
            if (query.next()) {
                totalRows = query.value(0).toInt();
            }
            // 删除空行
        } else {
            QMessageBox::critical(nullptr, "错误", "查询执行失败：" + query.lastError().text());
            return;
        }

        int startRow = currentPage * pageSize; // 当前页的起始行
        int endRow = qMin(startRow + pageSize, totalRows); // 当前页的结束行
        int numRows = endRow - startRow; // 当前页的行数

        ui->tableWidget_2->clearContents(); // 清空表格内容
        ui->tableWidget_2->setRowCount(numRows);
        // 执行查询
        query.prepare("SELECT * FROM historyHighQueryRecord WHERE recordPeopleId = :recordPeopleId LIMIT :startRow, :numRows");
        query.bindValue(":recordPeopleId", globalUserId);
        query.bindValue(":startRow", startRow);
        query.bindValue(":numRows", numRows);
        if (query.exec()) {
            int rowIndex = 0; // 当前页内的行索引

            while (query.next()) {
                QString stateResult = query.value(1).toString();
                QString institutionCodeResult = query.value(2).toString();
                QString institutionNameResult = query.value(3).toString();
                int TotalAssetsResult = query.value(4).toInt();
                int MaxAccountResult = query.value(5).toInt();
                QTableWidgetItem *item1 = new QTableWidgetItem(stateResult);
                QTableWidgetItem *item2 = new QTableWidgetItem(institutionCodeResult);
                QTableWidgetItem *item3 = new QTableWidgetItem(institutionNameResult);
                QTableWidgetItem *item4 = new QTableWidgetItem(QString::number(TotalAssetsResult));
                QTableWidgetItem *item5 = new QTableWidgetItem(QString::number(MaxAccountResult));

                ui->tableWidget_2->setItem(rowIndex, 0, item1);
                ui->tableWidget_2->setItem(rowIndex, 1, item2);
                ui->tableWidget_2->setItem(rowIndex, 2, item3);
                ui->tableWidget_2->setItem(rowIndex, 3, item4);
                ui->tableWidget_2->setItem(rowIndex, 4, item5);
                item1->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                item2->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                item3->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                item4->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                item5->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

                ++rowIndex;
            }
            // 删除空行
            removeEmptyRows(ui->tableWidget_2);
        } else {
            QMessageBox::critical(nullptr, "错误", "查询执行失败：" + query.lastError().text());
            return;
        }

        // 关闭数据库连接
        database.close();
        QString styledText1 ="<font color='red'>" + QString::number(currentPage + 1) + "</font>";
        QString styledText2 ="<font color='black'>/</font>""<font color='red'>" + QString::number(totalPages) + "</font>";
        ui->textEdit_5->setHtml(styledText1);
        ui->textEdit_4->setHtml(styledText2);
        ui->label_3->setText(QString::number(startRow) + "-" + QString::number(endRow) + "条，共" + QString::number(numRows) + "条");
    }
}

void MaximumAmountRegularDeclaration::removeEmptyRows(QTableWidget *tableWidget) {
    for (int row = tableWidget->rowCount() - 1; row >= 0; --row) {
        bool isEmptyRow = true;
        for (int column = 0; column < tableWidget->columnCount(); ++column) {
            QTableWidgetItem *item = tableWidget->item(row, column);
            if (item && !item->text().isEmpty()) {
                isEmptyRow = false;
                break;
            }
        }
        if (isEmptyRow) {
            tableWidget->removeRow(row);
//            totalRows--;
        }
    }
}

//业务提交
void MaximumAmountRegularDeclaration::businessSubmissions() {
    businessSubmissionsList.clear();
    QString dbName = "database.db";
    QString dbPath = QCoreApplication::applicationDirPath() + "./" + dbName;  // Use a relative path
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(dbPath);
    if (!database.open()) {
        QMessageBox::critical(nullptr, "错误", "无法打开数据库：" + database.lastError().text());
        return;
    }
    QSqlQuery query;
    query.prepare("SELECT ROWID,* FROM historyHighQueryRecord WHERE recordPeopleId = :recordPeopleId");
    query.bindValue(":recordPeopleId", globalUserId);
    if (query.exec()) {
        // 查询成功，将记录存储到全局变量中
        while (query.next()) {
            QStringList record;
            record << query.value(0).toString()
                   << query.value(1).toString()
                   << query.value(2).toString()
                   << query.value(3).toString()
                   << query.value(4).toString()
                   << query.value(5).toString()
                   << query.value(6).toString();
            businessSubmissionsList.append(record);
        }
    } else {
        return ;
    }

//    qDebug() << businessSubmissionsList;
    if (businessSubmissionsList.isEmpty()) {
        QMessageBox::information(nullptr, "提示", "没有找到相关记录。");
        // 关闭数据库连接
        database.close();
        return;
    }
    for (int row = 0; row < businessSubmissionsList.length(); ++row) {
        QStringList record = businessSubmissionsList.at(row);
        QString GUID = "";
        QString importResultCode = "0000";
        QString importResultState = "申报成功";
        QString institutionCodeResult = record.at(2);
        QString institutionNameResult = record.at(3);
        QString controlResult = record.at(4);
        int TotalAssetsResult = record.at(5).toInt();
        int MaxAccountResult = record.at(6).toInt();

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
        if (!query.exec()) {
            qDebug() << "插入数据失败：" << query.lastError().text();
        }

        QString deleteQuery = "DELETE FROM historyHighQueryRecord WHERE recordPeopleId = :recordPeopleId AND ROWID = :ROWID";
        query.prepare(deleteQuery);
        query.bindValue(":recordPeopleId", globalUserId);
        query.bindValue(":ROWID", record.at(0));
        if (!query.exec()) {
            qDebug() << "删除数据失败：" << query.lastError().text();
        }
    }
    // 关闭数据库连接
    database.close();
    totalRows = 0;
    ui->tableWidget_2->clearContents(); // 清空表格内容
    ui->tableWidget_2->setRowCount(totalRows);
    updateTableDisplay();
    // 创建并显示businessSubmissionsEshade对象
    businessSubmissionsEshade *businessSubmissionsshade = new businessSubmissionsEshade(this);
    businessSubmissionsshade->exec();
}

void MaximumAmountRegularDeclaration::topPageButton_clicked()
{
    if (currentPage != 0) {
        currentPage = 0;
        updateTableDisplay();
    }
    ui->pushButton_9->setEnabled(true);
    ui->pushButton_10->setEnabled(true);
    ui->pushButton_7->setEnabled(false);
    ui->pushButton_8->setEnabled(false);
}

void MaximumAmountRegularDeclaration::previousPageButton_clicked()
{
    if (currentPage > 0) {
        currentPage--;
        updateTableDisplay();
    }
    if(currentPage != 0) {
        ui->pushButton_7->setEnabled(true);
        ui->pushButton_8->setEnabled(true);
        ui->pushButton_9->setEnabled(true);
        ui->pushButton_10->setEnabled(true);
    } else {
        ui->pushButton_9->setEnabled(true);
        ui->pushButton_10->setEnabled(true);
        ui->pushButton_7->setEnabled(false);
        ui->pushButton_8->setEnabled(false);
    }
}

void MaximumAmountRegularDeclaration::nextPageButton_clicked()
{
    if (currentPage < totalPages - 1) {
        currentPage++;
        updateTableDisplay();
    }
    if (currentPage != totalPages - 1) {
        ui->pushButton_7->setEnabled(true);
        ui->pushButton_8->setEnabled(true);
        ui->pushButton_9->setEnabled(true);
        ui->pushButton_10->setEnabled(true);
    } else {
        ui->pushButton_9->setEnabled(false);
        ui->pushButton_10->setEnabled(false);
        ui->pushButton_7->setEnabled(true);
        ui->pushButton_8->setEnabled(true);
    }
}

void MaximumAmountRegularDeclaration::bottomPageButton_clicked()
{
    if (currentPage != totalPages - 1) {
        currentPage = totalPages - 1;
        updateTableDisplay();
    }
    ui->pushButton_9->setEnabled(false);
    ui->pushButton_10->setEnabled(false);
    ui->pushButton_7->setEnabled(true);
    ui->pushButton_8->setEnabled(true);
}
