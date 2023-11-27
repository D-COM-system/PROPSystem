#include "fundbalancequery.h"
#include "ui_fundbalancequery.h"

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


FundBalanceQuery::~FundBalanceQuery()
{
    delete ui;
}

FundBalanceQuery::FundBalanceQuery(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FundBalanceQuery)
{
    ui->setupUi(this);

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

    ui->comboBox_2->setStyleSheet(comboBoxStyle);

    ui->widget_2->setVisible(false);
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
    query.prepare("SELECT COUNT(*) FROM BalanceQuiry");
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


    ui->tableWidget_2->clear(); // 清空表格内容
    QStringList headerLabels;
    headerLabels << "序号" << "业务日期" << "资金账户" << "资金账户名称" << "币种"
                 << "账户余额" << "可用余额" << "尚未支付金额" << "透支金额"
                 << "最低备付" << "冻结金额" << "余额积数" << "结息利率"
                 << "最低备付积数" << "结果代码" << "结果说明";

    ui->tableWidget_2->setColumnCount(17);
    ui->tableWidget_2->setHorizontalHeaderLabels(headerLabels);
    ui->tableWidget_2->resizeColumnsToContents();
    ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_2->setRowCount(pageSize);
    //让tableWidget内容中的每个元素居中
    ui->tableWidget_2->setSelectionBehavior(QAbstractItemView::SelectRows);//设置整行选中
    //ui->tableWidget_2->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter);//表头字体居中
    ui->tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers);//单元格不可编辑
    // 设置表头的底色和文字加粗
    QString styleSheet = "QHeaderView::section { background-color: #f5f5f5; font-weight: bold; }";
    // 将样式表应用于表格的表头
    //ui->tableWidget_2->horizontalHeader()->setStyleSheet(styleSheet);
    //初始化勾选
    selectedRows.resize(totalRows);
    for (int row = 0; row < totalRows; ++row) {
        selectedRows[row] = 0;
    }
    updateTableDisplay();
//    qDebug() << totalRows;
    // 隐藏左侧的行号框
    //ui->tableWidget_2->verticalHeader()->setVisible(false);
    connect(ui->pushButton_5, &QPushButton::clicked, this, &FundBalanceQuery::siftToData);


}

void FundBalanceQuery::updateTableDisplay()
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
    query.prepare("SELECT * FROM BalanceQuiry LIMIT :startRow, :numRows");
    query.bindValue(":startRow", startRow);
    query.bindValue(":numRows", numRows);
    if (query.exec()) {
        int rowIndex = 0; // 当前页内的行索引

        while (query.next()) {
            QCheckBox *checkBox = new QCheckBox();
            if(selectedRows[startRow + rowIndex] == 1) {
                checkBox->setChecked(true);
            } else {
                checkBox->setChecked(false);
            }
            ui->tableWidget_2->setCellWidget(rowIndex, 0, checkBox);

        }

        // 删除空行
        removeEmptyRows(ui->tableWidget_2);
    } else {
        QMessageBox::critical(nullptr, "错误", "查询失败：" + query.lastError().text());
    }



    // 关闭数据库连接
    database.close();
}

void FundBalanceQuery::removeEmptyRows(QTableWidget *tableWidget)
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

void FundBalanceQuery::selectRows()
{
    for (int row = 0; row < ui->tableWidget_2->rowCount(); ++row) {
        int rowIndex = currentPage * pageSize + row; // 计算行索引
        QCheckBox *checkBox = qobject_cast<QCheckBox *>(ui->tableWidget_2->cellWidget(row, 0));
        if (checkBox->isChecked()) {
            selectedRows[rowIndex] = 1;
        } else {
            selectedRows[rowIndex] = 0;
        }
    }
}

void FundBalanceQuery::AllCheckbox(bool checked)
{
    QCheckBox *checkBox;
    int startRow = currentPage * pageSize; // 当前页的起始行
    int endRow = qMin(startRow + pageSize, totalRows); // 当前页的结束行
    int numRows = endRow - startRow; // 当前页的行数
    for (int row = 0; row < totalRows; ++row) {
        if(row < numRows) {
            checkBox = qobject_cast<QCheckBox *>(ui->tableWidget_2->cellWidget(row, 0));
        }
        if(checked) {
            selectedRows[row] = 1;
        } else {
            selectedRows[row] = 0;
        }
        checkBox->setChecked(checked);
    }
}

void FundBalanceQuery::Balancequery(){

    QString dbName = "database.db";
    QString dbPath = QCoreApplication::applicationDirPath() + "./" + dbName;  // Use a relative path
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(dbPath);

    QString account = ui->comboBox_2->currentText().trimmed();//资金账户
    qDebug() << account;
    if (!database.open()) {
        QMessageBox::critical(nullptr, "错误", "无法打开数据库：" + database.lastError().text());
        return;
    }
    QSqlQuery query;
    query.prepare("SELECT AccountName, Balance, AvailableBalance, BalanceAccumulation FROM BalanceQuiry WHERE Account = :account");
    query.bindValue(":account", account);
    qDebug() << "account1";
    if (query.exec()) {
        if (query.next()) {
            qDebug() << "account2";
            totalRows = query.value(0).toInt();
            QString accountname = query.value(0).toString();//资金账户名称
            QString balance = query.value(1).toString();//账户余额
            QString availablebalance = query.value(2).toString();//可用余额
            QString balanceaccumulation = query.value(3).toString();//余额积数

            ui->widget_2->setVisible(true);
            ui->lineEdit_11->setText(account);
            ui->lineEdit_13->setText(accountname);
            ui->lineEdit_30->setText(balance);
            ui->lineEdit_33->setText(availablebalance);
            ui->lineEdit_37->setText(balanceaccumulation);
        }
    } else {
        qDebug() << "account3";
        QMessageBox::critical(nullptr, "错误", "查询执行失败：" + query.lastError().text());
        return;
    }
    connect(ui->pushButton_2, &QPushButton::clicked, this,[&](){
        ui->widget_2->setVisible(false);
    } );

    // 关闭数据库连接
    database.close();
}

void FundBalanceQuery::siftToData() {  //查询

    QString account = ui->comboBox_2->currentText();
    if( account != NULL)  {
        QString dbName = "database.db";
        QString dbPath = QCoreApplication::applicationDirPath() + "./" + dbName;  // Use a relative path
        QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName(dbPath);

        if (!database.open()) {
            QMessageBox::critical(nullptr, "错误", "无法打开数据库：" + database.lastError().text());
            return;
        }

        QSqlQuery query;
        query.prepare("SELECT COUNT(*) FROM BalanceQuiry");
        if (query.exec()) {
            if (query.next()) {
                totalRows = query.value(0).toInt();
            }
        } else {
            QMessageBox::critical(nullptr, "错误", "查询执行失败：" + query.lastError().text());
            return;
        }
        currentPage = 0,
        pageSize = 50,
        totalPages = ((totalRows + pageSize - 1) / pageSize);
        int startRow = 0 * pageSize; // 当前页的起始行
        int endRow = qMin(startRow + pageSize, totalRows); // 当前页的结束行
        int numRows = endRow - startRow; // 当前页的行数
        int judgeAccount = 0;

        ui->tableWidget_2->clearContents(); // 清空表格内容
        ui->tableWidget_2->setRowCount(numRows);

        QString queryString = "SELECT * FROM BalanceQuiry WHERE 1=1";
        if (account != NULL) {
            queryString += " AND account = :account";
            judgeAccount = 1;
        }

        queryString += " LIMIT :startRow, :numRows";
        query.prepare(queryString);
        if(judgeAccount) {
            query.bindValue(":account", account);
        }

        query.bindValue(":startRow", startRow);
        query.bindValue(":numRows", numRows);
        qDebug() << queryString;

        if (query.exec()) {
            int rowIndex = 0; // 当前页内的行索引

            while (query.next()) {

                QCheckBox *checkBox = new QCheckBox();
                QHBoxLayout *layoutCheckBox = new QHBoxLayout();
                QWidget *widget = new QWidget(ui->tableWidget_2);
                layoutCheckBox->addWidget(checkBox);
                layoutCheckBox->setMargin(0);
                layoutCheckBox->setAlignment(checkBox, Qt::AlignCenter);
                widget->setLayout(layoutCheckBox);
                ui->tableWidget_2->setCellWidget(rowIndex, 0, widget);
                connect(checkBox, &QCheckBox::clicked, this, &FundBalanceQuery::selectRows);
                connect(checkBox, &QCheckBox::toggled, this, &FundBalanceQuery::AllCheckbox);

                QTableWidgetItem *item1 = new QTableWidgetItem(query.value(1).toString());//结束代码
                QTableWidgetItem *item2 = new QTableWidgetItem(query.value(2).toString());//币种
                QTableWidgetItem *item3 = new QTableWidgetItem(query.value(3).toString());//资金账户
                QTableWidgetItem *item4 = new QTableWidgetItem(query.value(4).toString());//资金账户名称
                QTableWidgetItem *item5 = new QTableWidgetItem(query.value(5).toString());//账户余额
                QTableWidgetItem *item6 = new QTableWidgetItem(query.value(6).toString());//可用余额
                QTableWidgetItem *item7 = new QTableWidgetItem("0.00");//尚未支付
                QTableWidgetItem *item8 = new QTableWidgetItem("0.00");//透支金额
                QTableWidgetItem *item9 = new QTableWidgetItem(query.value(9).toString());//最低备付
                QTableWidgetItem *item10 = new QTableWidgetItem("0.00");//冻结金额
                QTableWidgetItem *item11 = new QTableWidgetItem(query.value(11).toString());//余额积数
                QTableWidgetItem *item12 = new QTableWidgetItem(query.value(12).toString());//结息利率
                QTableWidgetItem *item13 = new QTableWidgetItem(query.value(13).toString());//业务日期
                QTableWidgetItem *item14 = new QTableWidgetItem(query.value(14).toString());//最低备付积数
                QTableWidgetItem *item15 = new QTableWidgetItem(query.value(15).toString());//结果代码
                QTableWidgetItem *item16 = new QTableWidgetItem(query.value(16).toString());//结果说明

                QPushButton *balancequery = new QPushButton("余额查询>>");
                balancequery->setStyleSheet("QPushButton{color:green;background-color:rgba(0,0,0,0);}"
                                            "QPushButton:pressed{background-color:rgb(51,129,172)}");
                ui->comboBox_4->setCurrentText(query.value(4).toString());//?没实现
                ui->tableWidget_2->setItem(rowIndex, 1, item13);
                ui->tableWidget_2->setItem(rowIndex, 2, item3);
                ui->tableWidget_2->setItem(rowIndex, 3, item4);
                ui->tableWidget_2->setItem(rowIndex, 4, item2);
                ui->tableWidget_2->setItem(rowIndex, 5, item5);
                ui->tableWidget_2->setItem(rowIndex, 6, item6);
                ui->tableWidget_2->setItem(rowIndex, 7, item7);
                ui->tableWidget_2->setItem(rowIndex, 8, item8);
                ui->tableWidget_2->setItem(rowIndex, 9, item9);
                ui->tableWidget_2->setItem(rowIndex, 10, item10);
                ui->tableWidget_2->setItem(rowIndex, 11, item11);
                ui->tableWidget_2->setItem(rowIndex, 12, item12);
                ui->tableWidget_2->setItem(rowIndex, 13, item14);
                ui->tableWidget_2->setItem(rowIndex, 14, item1);
                ui->tableWidget_2->setItem(rowIndex, 15, item16);
                ui->tableWidget_2->setCellWidget(rowIndex, 16, balancequery);

                item1->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                item2->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);              
                item3->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                item4->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                item5->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                item6->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                item7->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                item8->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                item9->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                item10->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                item11->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                item12->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                item13->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                item14->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                item15->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                item16->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

                ++rowIndex;
                connect(balancequery,&QPushButton::clicked,this,&FundBalanceQuery::Balancequery);//余额查询
            }
            // 删除空行
            removeEmptyRows(ui->tableWidget_2);
            QString text1 = "当前页记录数:";
            QString text2 = "当前页码:";
            QString text3 = "共计:";
            QString styledText1 = "<font color='black'>" + text1 + "</font>" + "<font color='red'>" + QString::number(numRows) + "</font>";
            QString styledText2 = "<font color='black'>" + text2 + "</font>" + "<font color='red'>" + QString::number(currentPage + 1) + "</font>" + "<font color='black'>" + "/" + "</font>" + "<font color='red'>" + QString::number(totalPages) + "</font>";
            QString styledText3 = "<font color='black'>" + text3 + "</font>" + "<font color='red'>" + QString::number(totalRows) + "</font>" + "<font color='black'>" + "条记录" + "</font>";


        } else {
            QMessageBox::critical(nullptr, "错误", "查询执行失败：" + query.lastError().text());
            return;
        }

        // 关闭数据库连接
        database.close();
    }else {
        currentPage = 0,
        pageSize = 50,
        totalPages = ((totalRows + pageSize - 1) / pageSize);
        updateTableDisplay();
    }
}
void createfundbalancequery(QString excelFilePath, QString dirPath){
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
    QString createQuery = "CREATE TABLE BalanceQuiry ("
                          "ID VARCHAR(64),"
                          "BankCode VARCHAR(10) DEFAULT '0100',"
                          "Currency VARCHAR(10) DEFAULT 'CNY',"
                          "Account VARCHAR(40),"
                          "AccountName VARCHAR(40),"
                          "Balance DECIMAL(17, 2),"
                          "AvailableBalance DECIMAL(17, 2),"
                          "OverdraftAmount DECIMAL(17, 2),"
                          "NotPaidAmount DECIMAL(17, 2),"
                          "MinReserve DECIMAL(17, 2),"
                          "FreezeAmount DECIMAL(17, 2),"
                          "BalanceAccumulation DECIMAL(17, 2),"
                          "InterestRate DECIMAL(17, 8),"
                          "BusinessDate DATE,"
                          "MinReserveAccumulation DECIMAL(17, 2),"
                          "ResultCode VARCHAR(10),"
                          "ResultDescription VARCHAR(10)"
                                      ")";
    if (!query.exec(createQuery)) {
        qDebug() << "创建表失败：" << query.lastError().text();
        return;
    }
    for (int row = 2; row <= sheet->dimension().lastRow(); ++row) {
        QString PROPID = "";
        QString BankCode = sheet->read(row, 2).toString();
        QString Currency = sheet->read(row, 3).toString();
        QString Account = sheet->read(row, 4).toString();
        QString AccountName = sheet->read(row, 5).toString();
        QString Balance = sheet->read(row, 6).toString();
        QString AvailableBalance = sheet->read(row, 7).toString();
        QString OverdraftAmount = sheet->read(row, 8).toString();
        QString NotPaidAmount = sheet->read(row, 9).toString();
        QString MinReserve = sheet->read(row, 10).toString();
        QString FreezeAmount = sheet->read(row, 11).toString();
        QString BalanceAccumulation = sheet->read(row, 12).toString();
        QString InterestRate = sheet->read(row, 13).toString();
        QString BusinessDate = sheet->read(row, 14).toString();
        QString MinReserveAccumulation = sheet->read(row, 15).toString();
        QString ResultCode = sheet->read(row, 16).toString();
        QString ResultDescription = sheet->read(row, 17).toString();

        // 检查所有元素是否都为空
        if (PROPID.isEmpty() && BankCode.isEmpty() && Currency.isEmpty() && Account.isEmpty() && Balance.isEmpty() && AvailableBalance.isEmpty() && OverdraftAmount.isEmpty() && NotPaidAmount.isEmpty() &&
                MinReserve.isEmpty() && FreezeAmount.isEmpty() && BalanceAccumulation.isEmpty() && InterestRate.isEmpty() && BusinessDate.isEmpty() && MinReserveAccumulation.isEmpty() &&
                ResultCode.isEmpty() && ResultDescription.isEmpty()) {
            continue;  // 跳过当前行，不执行插入操作
        }

        QString insertQuery = "INSERT INTO BalanceQuiry (ID,BankCode, Currency, Account,AccountName,Balance,AvailableBalance,OverdraftAmount,NotPaidAmount,MinReserve,FreezeAmount,BalanceAccumulation,InterestRate,BusinessDate,MinReserveAccumulation,ResultCode,ResultDescription) "
                              "VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)";
        query.prepare(insertQuery);
        query.addBindValue(PROPID);
        query.addBindValue(BankCode);
        query.addBindValue(Currency);
        query.addBindValue(Account);
        query.addBindValue(AccountName);
        query.addBindValue(Balance);
        query.addBindValue(AvailableBalance);
        query.addBindValue(OverdraftAmount);
        query.addBindValue(NotPaidAmount);
        query.addBindValue(MinReserve);
        query.addBindValue(FreezeAmount);
        query.addBindValue(BalanceAccumulation);
        query.addBindValue(InterestRate);
        query.addBindValue(BusinessDate);
        query.addBindValue(MinReserveAccumulation);
        query.addBindValue(ResultCode);
        query.addBindValue(ResultDescription);

        if (!query.exec()) {
            qDebug() << "插入数据失败：" << query.lastError().text();
        }
    }


    db.close();
}
