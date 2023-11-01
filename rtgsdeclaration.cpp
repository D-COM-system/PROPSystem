#include "CheckBoxHeaderView.h"
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
#include <QCheckBox>

RTGSdeclaration::RTGSdeclaration(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RTGSdeclaration)
{
    ui->setupUi(this);
    comboBoxAddItem();
    // 设置下拉框的颜色
    QString comboBoxStyle = "QComboBox {"
                            "background-color: #FFFFFF;"  // 设置背景色
                            "color: #000000;"  // 设置文字颜色
                            "selection-background-color: #C0C0C0;"  // 设置选中项的背景色
                            "selection-color: #000000;"  // 设置选中项的文字颜色
                            "}";

    // 设置子项之间的间距
    comboBoxStyle += "QComboBox QAbstractItemView {"
                     "spacing: 10px;"  // 设置子项之间的间距
                     "}";

    ui->comboBox->setStyleSheet(comboBoxStyle);
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
    query.prepare("SELECT COUNT(*) FROM RTGS_instructions");
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
    pageSize = 20,
    totalPages = ((totalRows + pageSize - 1) / pageSize);

    CheckBoxHeaderView *header = new CheckBoxHeaderView(0, QPoint(40, 5), QSize(20, 20), Qt::Horizontal, this);

    ui->tableWidget->setColumnCount(9);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "勾选状态" << "交收编号" << "证券账户" << "资金账号" << "实际收付"<< "交收状态" << "结果代码" << "结果说明" << "交收时间");
    ui->tableWidget->setHorizontalHeader(header);

    connect(header, &CheckBoxHeaderView::signalCheckStateChanged, [=](bool state)
            {
                AllCheckbox(state);
            });
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setRowCount(pageSize);
    ui->tableWidget->verticalHeader()->hide();

    //初始化勾选
    selectedRows.resize(totalRows);
    for (int row = 0; row < totalRows; ++row) {
        selectedRows[row] = 0;
    }
    updateTableDisplay();
    connect(ui->pushButton_14, &QPushButton::clicked, this, &RTGSdeclaration::topPageButton_clicked);
    connect(ui->pushButton_15, &QPushButton::clicked, this, &RTGSdeclaration::previousPageButton_clicked);
    connect(ui->pushButton_12, &QPushButton::clicked, this, &RTGSdeclaration::nextPageButton_clicked);
    connect(ui->pushButton_13, &QPushButton::clicked, this, &RTGSdeclaration::bottomPageButton_clicked);
    connect(ui->textEdit, &QTextEdit::textChanged, this, &RTGSdeclaration::onTextChanged);
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
        query.prepare("SELECT * FROM RTGS_instructions LIMIT :startRow, :numRows");
        query.bindValue(":startRow", startRow);
        query.bindValue(":numRows", numRows);
        if (query.exec()) {
            int rowIndex = 0; // 当前页内的行索引

            while (query.next()) {
                QString check_status = query.value(1).toString();
                QString delivery_number = query.value(2).toString();
                QString securities_account = query.value(3).toString();
                QString fund_account = query.value(4).toString();
                QString actual_payment = query.value(5).toString();
                QString delivery_status = query.value(6).toString();
                // 创建水平布局
                QHBoxLayout* layout = new QHBoxLayout();
                layout->setContentsMargins(0, 0, 0, 0);
                layout->setAlignment(Qt::AlignCenter); // 设置布局居中对齐
                // 创建勾选框和标签
                QCheckBox *checkBox = new QCheckBox();
                QLabel* label = new QLabel(check_status);

                // 将勾选框和标签添加到布局
                layout->addWidget(checkBox);
                layout->addWidget(label);

                // 设置布局为单元格的小部件
                QWidget* widget = new QWidget();
                widget->setLayout(layout);
                ui->tableWidget->setCellWidget(rowIndex, 0, widget);
                if(selectedRows[startRow + rowIndex] == 1) {
                    checkBox->setChecked(true);
                } else {
                    checkBox->setChecked(false);
                }

//                ui->tableWidget->setCellWidget(rowIndex, 0, checkBox);
                connect(checkBox, &QCheckBox::clicked, this, &RTGSdeclaration::selectRows);
                QTableWidgetItem *item2 = new QTableWidgetItem(delivery_number);
                QTableWidgetItem *item3 = new QTableWidgetItem(securities_account);
                QTableWidgetItem *item4 = new QTableWidgetItem(fund_account);
                QTableWidgetItem *item5 = new QTableWidgetItem(actual_payment);
                QTableWidgetItem *item6 = new QTableWidgetItem(delivery_status);

                ui->tableWidget->setItem(rowIndex, 1, item2);
                ui->tableWidget->setItem(rowIndex, 2, item3);
                ui->tableWidget->setItem(rowIndex, 3, item4);
                ui->tableWidget->setItem(rowIndex, 4, item5);
                ui->tableWidget->setItem(rowIndex, 5, item6);

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
        QString styledText1 ="<font color='red'>" + QString::number(currentPage + 1) + "</font>";
        QString styledText2 ="<font color='black'>/</font>""<font color='red'>" + QString::number(totalPages) + "</font>";
        ui->textEdit->setHtml(styledText1);
        ui->textEdit_2->setHtml(styledText2);
        ui->label_3->setText(QString::number(startRow) + "-" + QString::number(endRow) + "条，共" + QString::number(numRows) + "条");
    }
}

void RTGSdeclaration::removeEmptyRows(QTableWidget *tableWidget)
{
    for (int row = tableWidget->rowCount() - 1; row >= 0; --row) {
        bool isEmpty = true;
        for (int col = 0; col < tableWidget->columnCount(); ++col) {
            QTableWidgetItem *item = tableWidget->item(row, col);
            if (item && !item->text().isEmpty()) {
                isEmpty = false;
                break;
            }
        }
        if (isEmpty) {
            tableWidget->removeRow(row);
        }
    }
}

void RTGSdeclaration::selectRows()
{
    for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
        int rowIndex = currentPage * pageSize + row; // 计算行索引
        QWidget* cellWidget = ui->tableWidget->cellWidget(row, 0);
        if (cellWidget) {
            QCheckBox *checkBox = cellWidget->findChild<QCheckBox *>();
            if (checkBox && checkBox->isChecked()) {
                selectedRows[rowIndex] = 1;
            } else {
                selectedRows[rowIndex] = 0;
            }
        }
    }
}

void RTGSdeclaration::AllCheckbox(bool checked)
{
    QCheckBox *checkBox;
    int startRow = currentPage * pageSize; // 当前页的起始行
    int endRow = qMin(startRow + pageSize, totalRows); // 当前页的结束行
    int numRows = endRow - startRow; // 当前页的行数
    for (int row = 0; row < totalRows; ++row) {
        if(row < numRows) {
            QWidget* cellWidget = ui->tableWidget->cellWidget(row, 0);
            checkBox = cellWidget->findChild<QCheckBox *>();
        }
        if(checked) {
            selectedRows[row] = 1;
        } else {
            selectedRows[row] = 0;
        }
        checkBox->setChecked(checked);
    }
}

void RTGSdeclaration::comboBoxAddItem() {
    ui->comboBox->setEditable(false);
    // 连接数据库
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
    query.prepare("SELECT fund_account FROM RTGS_instructions");
    if (query.exec()) {
        while (query.next()) {
            QString fund_account = query.value(0).toString();
            ui->comboBox->addItem(fund_account);
        }
        ui->comboBox->setEditText("全部");
        ui->comboBox->setStyleSheet("QComboBox QAbstractItemView { background-color: yellow; color: blue; }");
    } else {
        QMessageBox::critical(nullptr, "错误", "查询执行失败：" + query.lastError().text());
        return;
    }

    // 关闭数据库连接
    database.close();
}

void RTGSdeclaration::topPageButton_clicked()
{
    if (currentPage != 0) {
        currentPage = 0;
        updateTableDisplay();
    }
    ui->pushButton_12->setEnabled(true);
    ui->pushButton_13->setEnabled(true);
    ui->pushButton_14->setEnabled(false);
    ui->pushButton_15->setEnabled(false);
}

void RTGSdeclaration::previousPageButton_clicked()
{
    if (currentPage > 0) {
        currentPage--;
        updateTableDisplay();
    }
    if(currentPage != 0) {
        ui->pushButton_12->setEnabled(true);
        ui->pushButton_13->setEnabled(true);
        ui->pushButton_14->setEnabled(true);
        ui->pushButton_15->setEnabled(true);
    } else {
        ui->pushButton_12->setEnabled(true);
        ui->pushButton_13->setEnabled(true);
        ui->pushButton_14->setEnabled(false);
        ui->pushButton_15->setEnabled(false);
    }
}

void RTGSdeclaration::nextPageButton_clicked()
{
    if (currentPage < totalPages - 1) {
        currentPage++;
        updateTableDisplay();
    }
    if (currentPage != totalPages - 1) {
        ui->pushButton_12->setEnabled(true);
        ui->pushButton_13->setEnabled(true);
        ui->pushButton_14->setEnabled(true);
        ui->pushButton_15->setEnabled(true);
    } else {
        ui->pushButton_12->setEnabled(false);
        ui->pushButton_13->setEnabled(false);
        ui->pushButton_14->setEnabled(true);
        ui->pushButton_15->setEnabled(true);
    }
}

void RTGSdeclaration::bottomPageButton_clicked()
{
    if (currentPage != totalPages - 1) {
        currentPage = totalPages - 1;
        updateTableDisplay();
    }
    ui->pushButton_12->setEnabled(false);
    ui->pushButton_13->setEnabled(false);
    ui->pushButton_14->setEnabled(true);
    ui->pushButton_15->setEnabled(true);
}

void RTGSdeclaration::onTextChanged() {

}

void RTGSdeclaration::on_pushButton_5_clicked()
{
    ui->comboBox->setCurrentText("全部");
    ui->comboBox_2->setCurrentText("未勾单");
    ui->comboBox_3->setCurrentText("全部");
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
    query.prepare("SELECT COUNT(*) FROM RTGS_instructions");
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
    pageSize = 20,
    totalPages = ((totalRows + pageSize - 1) / pageSize);
    //初始化勾选
    selectedRows.resize(totalRows);
    for (int row = 0; row < totalRows; ++row) {
        selectedRows[row] = 0;
    }
    updateTableDisplay();
}

//查询按钮，筛选数据
void RTGSdeclaration::on_pushButton_6_clicked()
{
    QString fund = ui->comboBox->currentText().trimmed();
    QString check = ui->comboBox_2->currentText().trimmed();
    QString delivery = ui->comboBox_3->currentText().trimmed();
    QString dbName = "database.db";
    QString dbPath = QCoreApplication::applicationDirPath() + "./" + dbName;  // Use a relative path
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(dbPath);

    if (!database.open()) {
        QMessageBox::critical(nullptr, "错误", "无法打开数据库：" + database.lastError().text());
        return;
    }

    QSqlQuery query;
    QString queryString = "SELECT COUNT(*) FROM RTGS_instructions WHERE 1=1";
    int fundCode = 0;
    int checkCode = 0;
    int deliveryCode = 0;
    if(fund != "全部") {
        queryString += " AND fund_account = :fund_account";
        fundCode = 1;
    }
    if(check != "全部") {
        queryString += " AND check_status = :check_status";
        checkCode = 1;
    }
    if(delivery != "全部") {
        queryString += " AND delivery_status = :delivery_status";
        deliveryCode = 1;
    }
    query.prepare(queryString);
    if(fundCode) {
        query.bindValue(":fund_account", fund);
    }
    if(checkCode) {
        query.bindValue(":check_status", check);
    }
    if(deliveryCode) {
        query.bindValue(":delivery_status", delivery);
    }
    if (query.exec()) {
        if (query.next()) {
            totalRows = query.value(0).toInt();
        }
        // 删除空行
    } else {
        QMessageBox::critical(nullptr, "错误", "查询执行失败：" + query.lastError().text());
        return;
    }
    currentPage = 0,
    pageSize = 50,
    totalPages = ((totalRows + pageSize - 1) / pageSize);
    //初始化勾选
    selectedRows.resize(totalRows);
    for (int row = 0; row < totalRows; ++row) {
        selectedRows[row] = 0;
    }
    int startRow = 0 * pageSize; // 当前页的起始行
    int endRow = qMin(startRow + pageSize, totalRows); // 当前页的结束行
    int numRows = endRow - startRow; // 当前页的行数
    if(totalRows == 0) {
        QString styledText1 ="<font color='red'>0</font>";
        QString styledText2 ="<font color='black'>/</font>""<font color='red'>0</font>";
        ui->textEdit->setHtml(styledText1);
        ui->textEdit_2->setHtml(styledText2);
        ui->label_3->setText("无数据显示");
        ui->tableWidget->clearContents(); // 清空表格内容
        ui->tableWidget->setRowCount(totalRows);
    } else {

        ui->tableWidget->clearContents(); // 清空表格内容
        ui->tableWidget->setRowCount(numRows);
        // 执行查询
        QSqlQuery query;
        QString queryStringInsert = "SELECT * FROM RTGS_instructions WHERE 1=1";
        fundCode = 0;
        checkCode = 0;
        deliveryCode = 0;
        if(fund != "全部") {
            queryStringInsert += " AND fund_account = :fund_account";
            fundCode = 1;
        }
        if(check != "全部") {
            queryStringInsert += " AND check_status = :check_status";
            checkCode = 1;
        }
        if(delivery != "全部") {
            queryStringInsert += " AND delivery_status = :delivery_status";
            deliveryCode = 1;
        }
        queryStringInsert += " LIMIT :startRow, :numRows";
        query.prepare(queryStringInsert);
        if(fundCode) {
            query.bindValue(":fund_account", fund);
        }
        if(checkCode) {
            query.bindValue(":check_status", check);
        }
        if(deliveryCode) {
            query.bindValue(":delivery_status", delivery);
        }
        query.bindValue(":startRow", startRow);
        query.bindValue(":numRows", numRows);

        if (query.exec()) {
            int rowIndex = 0; // 当前页内的行索引

            while (query.next()) {
                QString check_status = query.value(1).toString();
                QString delivery_number = query.value(2).toString();
                QString securities_account = query.value(3).toString();
                QString fund_account = query.value(4).toString();
                QString actual_payment = query.value(5).toString();
                QString delivery_status = query.value(6).toString();
                // 创建水平布局
                QHBoxLayout* layout = new QHBoxLayout();
                layout->setContentsMargins(0, 0, 0, 0);
                layout->setAlignment(Qt::AlignCenter); // 设置布局居中对齐
                // 创建勾选框和标签
                QCheckBox *checkBox = new QCheckBox();
                QLabel* label = new QLabel(check_status);

                // 将勾选框和标签添加到布局
                layout->addWidget(checkBox);
                layout->addWidget(label);

                // 设置布局为单元格的小部件
                QWidget* widget = new QWidget();
                widget->setLayout(layout);
                ui->tableWidget->setCellWidget(rowIndex, 0, widget);
                if(selectedRows[startRow + rowIndex] == 1) {
                    checkBox->setChecked(true);
                } else {
                    checkBox->setChecked(false);
                }

//                ui->tableWidget->setCellWidget(rowIndex, 0, checkBox);
                connect(checkBox, &QCheckBox::clicked, this, &RTGSdeclaration::selectRows);
                QTableWidgetItem *item2 = new QTableWidgetItem(delivery_number);
                QTableWidgetItem *item3 = new QTableWidgetItem(securities_account);
                QTableWidgetItem *item4 = new QTableWidgetItem(fund_account);
                QTableWidgetItem *item5 = new QTableWidgetItem(actual_payment);
                QTableWidgetItem *item6 = new QTableWidgetItem(delivery_status);

                ui->tableWidget->setItem(rowIndex, 1, item2);
                ui->tableWidget->setItem(rowIndex, 2, item3);
                ui->tableWidget->setItem(rowIndex, 3, item4);
                ui->tableWidget->setItem(rowIndex, 4, item5);
                ui->tableWidget->setItem(rowIndex, 5, item6);

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
        QString styledText1 ="<font color='red'>" + QString::number(currentPage + 1) + "</font>";
        QString styledText2 ="<font color='black'>/</font>""<font color='red'>" + QString::number(totalPages) + "</font>";
        ui->textEdit->setHtml(styledText1);
        ui->textEdit_2->setHtml(styledText2);
        ui->label_3->setText(QString::number(startRow) + "-" + QString::number(endRow) + "条，共" + QString::number(numRows) + "条");
    }
}

