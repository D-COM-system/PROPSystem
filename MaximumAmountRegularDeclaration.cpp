#include "MaximumAmountRegularDeclaration.h"
#include "ui_MaximumAmountRegularDeclaration.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>

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
    query.prepare("SELECT COUNT(*) FROM record");
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
    headerLabels << "机构代码" << "机构名称" << "控制类别" << "净资本/合计资产总额（百万）" << "最高额度（百万）";
    ui->tableWidget_2->clear(); // 清空表格内容
    ui->tableWidget_2->setColumnCount(headerLabels.size());
    ui->tableWidget_2->setHorizontalHeaderLabels(headerLabels);
    ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_2->setRowCount(pageSize);
    ui->tableWidget_2->verticalHeader()->hide();
    updateTableDisplay();
    connect(ui->pushButton_7, &QPushButton::clicked, this, &MaximumAmountRegularDeclaration::topPageButton_clicked);
    connect(ui->pushButton_8, &QPushButton::clicked, this, &MaximumAmountRegularDeclaration::previousPageButton_clicked);
    connect(ui->pushButton_9, &QPushButton::clicked, this, &MaximumAmountRegularDeclaration::nextPageButton_clicked);
    connect(ui->pushButton_10, &QPushButton::clicked, this, &MaximumAmountRegularDeclaration::bottomPageButton_clicked);
}

MaximumAmountRegularDeclaration::~MaximumAmountRegularDeclaration()
{
    delete ui;
}

void MaximumAmountRegularDeclaration::updateTableDisplay()
{
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

    ui->tableWidget_2->clearContents(); // 清空表格内容
    ui->tableWidget_2->setRowCount(numRows);
    // 执行查询
    QSqlQuery query;
    query.prepare("SELECT * FROM record LIMIT :startRow, :numRows");
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

            ui->tableWidget_2->setItem(rowIndex, 0, item1);
            ui->tableWidget_2->setItem(rowIndex, 1, item2);
            ui->tableWidget_2->setItem(rowIndex, 2, item3);
            ui->tableWidget_2->setItem(rowIndex, 3, item4);
            ui->tableWidget_2->setItem(rowIndex, 4, item5);

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
    ui->lineEdit_7->setText(QString::number(currentPage + 1));
    ui->lineEdit_13->setText(QString::number(totalPages));
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

void MaximumAmountRegularDeclaration::topPageButton_clicked()
{
    if (currentPage != 0) {
        currentPage = 0;
        updateTableDisplay();
    }
}

void MaximumAmountRegularDeclaration::previousPageButton_clicked()
{
    if (currentPage > 0) {
        currentPage--;
        updateTableDisplay();
    }
}

void MaximumAmountRegularDeclaration::nextPageButton_clicked()
{
    if (currentPage < totalPages - 1) {
        currentPage++;
        updateTableDisplay();
    }
}

void MaximumAmountRegularDeclaration::bottomPageButton_clicked()
{
    if (currentPage != totalPages - 1) {
        currentPage = totalPages - 1;
        updateTableDisplay();
    }
}
