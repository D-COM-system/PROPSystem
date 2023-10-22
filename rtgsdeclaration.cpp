#include "rtgsdeclaration.h"
#include "ui_rtgsdeclaration.h"

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

RTGSdeclaration::RTGSdeclaration(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RTGSdeclaration)
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
    QString createQuery = "CREATE TABLE RTGS_instructions (id VARCHAR(20) PRIMARY KEY,"
                          "check_status VARCHAR(10) NOT NULL," //勾单状态
                          "delivery_number VARCHAR(50),"      //交收编号
                          "securities_account VARCHAR(20),"   //证券账户
                          "fund_account VARCHAR(10),"         //资金账号
                          "actual_payment DECIMAL(17, 2),"    //实际收付
                          "delivery_status VARCHAR(10) NOT NULL)";//交收状态
    if (!query.exec(createQuery)) {
        qDebug() << "创建表失败：" << query.lastError().text();
    }
    query.prepare("SELECT COUNT(*) FROM RTGS_instructions WHERE id = : id");
    query.bindValue(":id", globalUserId);
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
    pageSize = 10,
    totalPages = ((totalRows + pageSize - 1) / pageSize);
    QStringList headerLabels;
    headerLabels << "勾选状态" << "交收编号" << "证券账户" << "资金账号" << "实际收付"<< "交收状态" << "结果代码" << "结果说明" << "交收时间";
    ui->tableWidget->clear(); // 清空表格内容
    ui->tableWidget->setColumnCount(headerLabels.size());
    ui->tableWidget->setHorizontalHeaderLabels(headerLabels);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setRowCount(pageSize);
    ui->tableWidget->verticalHeader()->hide();
    updateTableDisplay();
    connect(ui->pushButton_14, &QPushButton::clicked, this, &RTGSdeclaration::topPageButton_clicked);
    connect(ui->pushButton_15, &QPushButton::clicked, this, &RTGSdeclaration::previousPageButton_clicked);
    connect(ui->pushButton_12, &QPushButton::clicked, this, &RTGSdeclaration::nextPageButton_clicked);
    connect(ui->pushButton_13, &QPushButton::clicked, this, &RTGSdeclaration::bottomPageButton_clicked);
//    connect(ui->pushButton_21, &QPushButton::clicked, this, &RTGSdeclaration::on_pushButton_21_clicked);//导入按钮
}

RTGSdeclaration::~RTGSdeclaration()
{
    delete ui;
}

void RTGSdeclaration::updateTableDisplay()
{
    if(totalRows == 0) {
        ui->label_3->setText("无数据显示");
    } else {
        int startRow = currentPage * pageSize; // 当前页的起始行
        int endRow = qMin(startRow + pageSize, totalRows); // 当前页的结束行
        int numRows = endRow - startRow; // 当前页的行数

            QString dbName = "database.db";
            QString dbPath = QCoreApplication::applicationDirPath() + "./" + dbName;  // Use a relative path
            QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
            database.setDatabaseName(dbPath);

            if (!database.open()) {
                QMessageBox::critical(nullptr, "错误", "无法打开数据库：" + database.lastError().text());
                return;
            }

            ui->tableWidget->clearContents(); // 清空表格内容
            ui->tableWidget->setRowCount(numRows);
            // 执行查询
            QSqlQuery query;
            query.prepare("SELECT * FROM RTGS_instructions WHERE id = :id LIMIT :startRow, :numRows");
            query.bindValue(":id", globalUserId);
            query.bindValue(":startRow", startRow);
            query.bindValue(":numRows", numRows);
            if (query.exec()) {
                int rowIndex = 0; // 当前页内的行索引

                while (query.next()) {
                    QString stateResult = query.value(1).toString();
                    QString institutionCodeResult = query.value(2).toString();
                    QString institutionNameResult = query.value(3).toString();
                    QString TotalAssetsResult = query.value(4).toString();
                    QString MaxAccountResult = query.value(5).toString();
                    QTableWidgetItem *item1 = new QTableWidgetItem(stateResult);
                    QTableWidgetItem *item2 = new QTableWidgetItem(institutionCodeResult);
                    QTableWidgetItem *item3 = new QTableWidgetItem(institutionNameResult);
                    QTableWidgetItem *item4 = new QTableWidgetItem(TotalAssetsResult);
                    QTableWidgetItem *item5 = new QTableWidgetItem(MaxAccountResult);

                    ui->tableWidget->setItem(rowIndex, 0, item1);
                    ui->tableWidget->setItem(rowIndex, 1, item2);
                    ui->tableWidget->setItem(rowIndex, 2, item3);
                    ui->tableWidget->setItem(rowIndex, 3, item4);
                    ui->tableWidget->setItem(rowIndex, 4, item5);

                    ++rowIndex;
                }
                // 删除空行
                removeEmptyRows(ui->tableWidget);
            } else {
                QMessageBox::critical(nullptr, "错误", "查询执行失败：" + query.lastError().text());
                return;
            }

            // 关闭数据库连接
            database.close();

        ui->lineEdit_7->setText(QString::number(currentPage + 1));
        ui->lineEdit_13->setText(QString::number(totalPages));
        ui->label_3->setText(QString::number(startRow) + "-" + QString::number(endRow) + "条，共" + QString::number(numRows) + "条");
    }
}

void RTGSdeclaration::removeEmptyRows(QTableWidget *tableWidget) {
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

void RTGSdeclaration::topPageButton_clicked()
{
    if (currentPage != 0) {
        currentPage = 0;
        updateTableDisplay();
    }
}

void RTGSdeclaration::previousPageButton_clicked()
{
    if (currentPage > 0) {
        currentPage--;
        updateTableDisplay();
    }
}

void RTGSdeclaration::nextPageButton_clicked()
{
    if (currentPage < totalPages - 1) {
        currentPage++;
        updateTableDisplay();
    }
}

void RTGSdeclaration::bottomPageButton_clicked()
{
    if (currentPage != totalPages - 1) {
        currentPage = totalPages - 1;
        updateTableDisplay();
    }
}
