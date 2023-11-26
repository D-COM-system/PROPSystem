#include "AShareDividendDifferentiatedDeclaration.h"
#include "AShareDividendDifferentiatedSubmitEshade.h"
#include "GlobalData.h"
#include "qsqlerror.h"
#include "ui_AShareDividendDifferentiatedDeclaration.h"

#include <QFileDialog>
#include <QDebug>
#include <xlsxdocument.h>
#include <QFile>
#include <QDate>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QProcess>
#include <QTextCodec>

QString nowAShareDividendDifferentiatedDeclarationFilePath = "";

AShareDividendDifferentiatedDeclaration::AShareDividendDifferentiatedDeclaration(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AShareDividendDifferentiatedDeclaration)
{
    ui->setupUi(this);

    totalRows = 0;
    currentPage = 0,
    pageSize = 50,
    totalPages = ((totalRows + pageSize - 1) / pageSize);

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
    query.prepare("SELECT COUNT(*) FROM AShareDividendDifferentiatedLogList");
    if (query.exec()) {
        if (query.next()) {
            totalRowsLogList = query.value(0).toInt();
        }
        // 删除空行
    } else {
        QMessageBox::critical(nullptr, "错误", "查询执行失败：" + query.lastError().text());
        return;
    }
    // 关闭数据库连接
    database.close();
    currentPageLogList = 0,
    pageSizeLogList = 50,
    totalPagesLogList = ((totalRowsLogList + pageSizeLogList - 1) / pageSizeLogList);

    QStringList headerLabels;
    headerLabels << "SBBH" << "ZLLX" << "RQ" << "YWBH" << "ZQZH" << "BZ" << "BY";
    ui->tableWidget_2->clear(); // 清空表格内容
    ui->tableWidget_2->setColumnCount(headerLabels.size());
    ui->tableWidget_2->setHorizontalHeaderLabels(headerLabels);
    ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //让tableWidget内容中的每个元素居中
    ui->tableWidget_2->setSelectionBehavior(QAbstractItemView::SelectRows);//设置整行选中
    ui->tableWidget_2->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter);//表头字体居中
    ui->tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers);//单元格不可编辑
    // 设置表头的底色和文字加粗
    QString styleSheet = "QHeaderView::section { background-color: rgb(219,241,253); font-weight: bold; border: 1px solid white; }";
    // 将样式表应用于表格的表头
    ui->tableWidget_2->horizontalHeader()->setStyleSheet(styleSheet);
    ui->tableWidget_2->verticalHeader()->hide();
    QString styledText1 ="<font color='red'>" + QString::number(0) + "</font>";
    QString styledText2 ="<font color='black'>/</font>""<font color='red'>" + QString::number(0) + "</font>";
    ui->textEdit_5->setHtml(styledText1);
    ui->textEdit_4->setHtml(styledText2);
    ui->label_5->setText("无数据显示");

    headerLabels.clear();
    headerLabels << "交易序号" << "操作员" << "日期" << "时间" << "业务域名" << "业务名称" << "业务代码" << "返回代码" << "返回说明";
    ui->tableWidget_3->clear(); // 清空表格内容
    ui->tableWidget_3->setColumnCount(headerLabels.size());
    ui->tableWidget_3->setHorizontalHeaderLabels(headerLabels);
    ui->tableWidget_3->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //让tableWidget内容中的每个元素居中
    ui->tableWidget_3->setSelectionBehavior(QAbstractItemView::SelectRows);//设置整行选中
    ui->tableWidget_3->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter);//表头字体居中
    ui->tableWidget_3->setEditTriggers(QAbstractItemView::NoEditTriggers);//单元格不可编辑
    // 设置表头的底色和文字加粗
    styleSheet = "QHeaderView::section { background-color: rgb(219,241,253); font-weight: bold; border: 1px solid white; }";
    // 将样式表应用于表格的表头
    ui->tableWidget_3->horizontalHeader()->setStyleSheet(styleSheet);
    ui->tableWidget_3->verticalHeader()->hide();

    updateLogListTableDisplay();

    connect(ui->pushButton_5, &QPushButton::clicked, this,&AShareDividendDifferentiatedDeclaration::chooseFile);

    connect(ui->pushButton_8, &QPushButton::clicked, this, &AShareDividendDifferentiatedDeclaration::topPageButton_clicked);
    connect(ui->pushButton_11, &QPushButton::clicked, this, &AShareDividendDifferentiatedDeclaration::previousPageButton_clicked);
    connect(ui->pushButton_9, &QPushButton::clicked, this, &AShareDividendDifferentiatedDeclaration::nextPageButton_clicked);
    connect(ui->pushButton_10, &QPushButton::clicked, this, &AShareDividendDifferentiatedDeclaration::bottomPageButton_clicked);

    connect(ui->pushButton_14, &QPushButton::clicked, this, &AShareDividendDifferentiatedDeclaration::topPageLogListButton_clicked);
    connect(ui->pushButton_15, &QPushButton::clicked, this, &AShareDividendDifferentiatedDeclaration::previousLogListPageButton_clicked);
    connect(ui->pushButton_12, &QPushButton::clicked, this, &AShareDividendDifferentiatedDeclaration::nextPageLogListButton_clicked);
    connect(ui->pushButton_13, &QPushButton::clicked, this, &AShareDividendDifferentiatedDeclaration::bottomLogListPageButton_clicked);

    connect(ui->pushButton_7, &QPushButton::clicked, this, &AShareDividendDifferentiatedDeclaration::submitData);
}

AShareDividendDifferentiatedDeclaration::~AShareDividendDifferentiatedDeclaration()
{
    delete ui;
}

void  AShareDividendDifferentiatedDeclaration::chooseFile()
{
    resetText();
    QString filePath = QFileDialog::getOpenFileName(this, "Select File", "", "DBF Files (*.dbf)");
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

    ui->lineEdit_7->setText(convertedPath);
    nowAShareDividendDifferentiatedDeclarationFilePath = filePath;
    qDebug() << nowAShareDividendDifferentiatedDeclarationFilePath;
    importFile();
}

void AShareDividendDifferentiatedDeclaration::importFile()
{
    if (judgeFile()) {
        // 获取用户输入的DBF文件路径
        QString dbfFilePath = nowAShareDividendDifferentiatedDeclarationFilePath;
        QProcess pythonProcess;
        QString filePath = QCoreApplication::applicationDirPath() + "//test.py";
        pythonProcess.start("python", QStringList() << filePath << dbfFilePath);
        pythonProcess.waitForFinished();

        QByteArray output = pythonProcess.readAllStandardOutput();

        // 将以GBK格式输出的QByteArray转换为Unicode字符串
        QTextCodec *codec = QTextCodec::codecForName("GBK");
        QString decodedOutput = codec->toUnicode(output);
        // 使用分隔符'\r\n'提取记录内容
        QStringList lines = decodedOutput.split("\r\n", QString::SkipEmptyParts);
//        businessSubmissionsList.clear();
        for(int i = 0;i < lines.size();i = i + 6){
            QStringList record;
            record << lines[i] << lines[i + 1] << lines[i + 2]<< lines[i + 3] << lines[i + 4] << lines[i + 5];
            businessSubmissionsList.append(record);
        }
        totalRows += lines.size() / 6;
        currentPage = 0,
        pageSize = 5,
        totalPages = ((totalRows + pageSize - 1) / pageSize);
        updateTableDisplay();
    }
}

void AShareDividendDifferentiatedDeclaration::updateTableDisplay() {
    if(totalRows == 0) {
        ui->label_5->setText("无数据显示");
    } else {
        // 获取全局变量中的数据
        QList<QStringList> databaseRecords = businessSubmissionsList;
        int startRow = currentPage * pageSize; // 当前页的起始行
        int endRow = qMin(startRow + pageSize, totalRows); // 当前页的结束行
        int numRows = endRow - startRow; // 当前页的行数
        // 清空表格内容
        ui->tableWidget_2->clearContents();
        // 设置行数
        ui->tableWidget_2->setRowCount(numRows);

        // 遍历数据并设置单元格内容
        for (int row = 0; row < numRows; ++row) {
            QStringList record = businessSubmissionsList.at(row + startRow);
            int columnCount = record.size();
            for (int column = 0; column < columnCount; ++column) {
                if (column == 1) {
                    QTableWidgetItem* item = new QTableWidgetItem("SB");
                    ui->tableWidget_2->setItem(row, column, item);
                    item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                } else {
                    QTableWidgetItem* item = new QTableWidgetItem(record.at(column));
                    ui->tableWidget_2->setItem(row, column, item);
                    item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                }
            }
        }
        removeEmptyRows(ui->tableWidget_2);
        QString styledText1 ="<font color='red'>" + QString::number(currentPage + 1) + "</font>";
        QString styledText2 ="<font color='black'>/</font>""<font color='red'>" + QString::number(totalPages) + "</font>";
        ui->textEdit_5->setHtml(styledText1);
        ui->textEdit_4->setHtml(styledText2);
        ui->label_5->setText(QString::number(startRow) + "-" + QString::number(endRow) + "条，共" + QString::number(numRows) + "条");
    }
}

void AShareDividendDifferentiatedDeclaration::removeEmptyRows(QTableWidget *tableWidget) {
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

void AShareDividendDifferentiatedDeclaration::resetText() {
    ui->lineEdit_7->clear();
}

void AShareDividendDifferentiatedDeclaration::topPageButton_clicked()
{
    if (currentPage != 0) {
        currentPage = 0;
        updateTableDisplay();
    }
    ui->pushButton_9->setEnabled(true);
    ui->pushButton_10->setEnabled(true);
    ui->pushButton_11->setEnabled(false);
    ui->pushButton_8->setEnabled(false);
}

void AShareDividendDifferentiatedDeclaration::previousPageButton_clicked()
{
    if (currentPage > 0) {
        currentPage--;
        updateTableDisplay();
    }
    if(currentPage != 0) {
        ui->pushButton_11->setEnabled(true);
        ui->pushButton_8->setEnabled(true);
        ui->pushButton_9->setEnabled(true);
        ui->pushButton_10->setEnabled(true);
    } else {
        ui->pushButton_9->setEnabled(true);
        ui->pushButton_10->setEnabled(true);
        ui->pushButton_11->setEnabled(false);
        ui->pushButton_8->setEnabled(false);
    }
}

void AShareDividendDifferentiatedDeclaration::nextPageButton_clicked()
{
    if (currentPage < totalPages - 1) {
        currentPage++;
        updateTableDisplay();
    }
    if (currentPage != totalPages - 1) {
        ui->pushButton_11->setEnabled(true);
        ui->pushButton_8->setEnabled(true);
        ui->pushButton_9->setEnabled(true);
        ui->pushButton_10->setEnabled(true);
    } else {
        ui->pushButton_9->setEnabled(false);
        ui->pushButton_10->setEnabled(false);
        ui->pushButton_11->setEnabled(true);
        ui->pushButton_8->setEnabled(true);
    }
}

void AShareDividendDifferentiatedDeclaration::bottomPageButton_clicked()
{
    if (currentPage != totalPages - 1) {
        currentPage = totalPages - 1;
        updateTableDisplay();
    }
    ui->pushButton_9->setEnabled(false);
    ui->pushButton_10->setEnabled(false);
    ui->pushButton_11->setEnabled(true);
    ui->pushButton_8->setEnabled(true);
}

bool AShareDividendDifferentiatedDeclaration::judgeFile() {
    return true;
}

void AShareDividendDifferentiatedDeclaration::submitData() {
    if(totalRows == 0) {
        QMessageBox::critical(nullptr, "错误", "无数据提交");
        return;
    }
    QString dbName = "database.db";
    QString dbPath = QCoreApplication::applicationDirPath() + "./" + dbName;  // Use a relative path
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(dbPath);

    if (!database.open()) {
        QMessageBox::critical(nullptr, "错误", "无法打开数据库：" + database.lastError().text());
        return;
    }
    // 获取当前时间
    QDateTime currentTime = QDateTime::currentDateTime();

    // 格式化时间为"YYYY-MM-DD HH:MM:SS"的字符串
    QString formattedTime = currentTime.toString("yyyy-MM-dd hh:mm:ss");
    // 将当前时间加一秒，并设置为resultTime
    QDateTime nextTime = currentTime.addSecs(1);
    QString resultTime = nextTime.toString("yyyy-MM-dd hh:mm:ss");
    // 拆分日期和时间
    QStringList dateTimeParts = resultTime.split(" ");
    QString datePart;
    QString timePart;
    if (dateTimeParts.size() == 2) {
        datePart = dateTimeParts[0];  // 日期部分
        timePart = dateTimeParts[1];  // 时间部分

        qDebug() << "Date: " << datePart;
        qDebug() << "Time: " << timePart;
    } else {
        // 时间字符串格式不正确
        qDebug() << "Invalid time string format!";
    }
    // 遍历数据并设置单元格内容
    QSqlQuery query;
    for (int row = 0; row < totalRows; ++row) {
        QStringList record = businessSubmissionsList.at(row);
        QString insertQuery = "INSERT INTO AShareDividendDifferentiated (AShareDividendDifferentiatedID,SBBH,ZLLX,RQ,YWBH,ZQZH)"
                              "VALUES (?, ?, ?, ?, ?, ?)";
        query.prepare(insertQuery);
        query.addBindValue(" ");
        query.addBindValue(record.at(0));
        query.addBindValue("SB");
        query.addBindValue(record.at(4));
        query.addBindValue(record.at(1));
        query.addBindValue(record.at(2));

        if (!query.exec()) {
            qDebug() << "插入数据失败：" << query.lastError().text();
        }

        QString TransactionSerialNumber;
        QString selectQuery = "SELECT TransactionSerialNumber FROM AShareDividendDifferentiatedEshade ORDER BY TransactionSerialNumber DESC LIMIT 1";
        query.prepare(selectQuery);
        if (query.exec()) {
            if (query.next()) {
                if(query.value(0).toString() == 0) {
                    TransactionSerialNumber = "100000000001";
//                    qDebug() << AShareDividendDifferentiatedEshadeID;
                } else {
                    TransactionSerialNumber = query.value(0).toString();
                    bool conversionOk;
                    qint64 numericValue = TransactionSerialNumber.toLongLong(&conversionOk);
                    if (conversionOk) {
                        // 执行加一操作
                        numericValue += 1;

                        // 将结果转换回字符串类型
                        TransactionSerialNumber = QString::number(numericValue);

                        // 输出结果字符串
//                        qDebug() << AShareDividendDifferentiatedEshadeID;
                    } else {
                        // 转换失败，处理错误
                        qDebug() << "Conversion error!";
                    }

                }
            } else {
                TransactionSerialNumber = "100000000001";
//                qDebug() << AShareDividendDifferentiatedEshadeID;
            }
              // 删除空行
        } else {
            qDebug() << "查询数据失败：" << query.lastError().text();
        }
        insertQuery = "INSERT INTO AShareDividendDifferentiatedEshade (AShareDividendDifferentiatedEshadeID,TransactionSerialNumber,CLJG,SBBH,FZDM,JGDM,JGSM)"
                      "VALUES (?, ?, ?, ?, ?, ?, ?)";
        query.prepare(insertQuery);
        query.addBindValue(" ");
        query.addBindValue(TransactionSerialNumber);
        query.addBindValue("P");
        query.addBindValue(record.at(0));
        query.addBindValue("HL");
        query.addBindValue("0000");
        query.addBindValue("成功");

        if (!query.exec()) {
            qDebug() << "插入数据失败：" << query.lastError().text();
        }

        insertQuery = "INSERT INTO AShareDividendDifferentiatedLogList (AShareDividendDifferentiatedLogListID,ActionPeople,AShareDividendDifferentiatedLogList,LogDate,LogTime,BusinessDomainName,BusinessName,BusinessCode,BackCode,BackResult)"
                      "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
        query.prepare(insertQuery);
        query.addBindValue(" ");
        query.addBindValue(globalUserName);
        query.addBindValue(TransactionSerialNumber);
        query.addBindValue(datePart);
        query.addBindValue(timePart);
        query.addBindValue("SSCCRC");
        query.addBindValue("CGXT");
        query.addBindValue("56");
        query.addBindValue("0000");
        query.addBindValue("成功");

        if (!query.exec()) {
            qDebug() << "插入数据失败：" << query.lastError().text();
        }
    }
    // Close the database connection
    database.close();
    AShareDividendDifferentiatedSubmitEshade *shade = new AShareDividendDifferentiatedSubmitEshade(this);
    shade->exec();
    // 清空表格内容
    ui->tableWidget_2->clearContents();
    businessSubmissionsList.clear();
    // 设置行数
    ui->tableWidget_2->setRowCount(0);
    int startRow = currentPage * pageSize; // 当前页的起始行
    int endRow = qMin(startRow + pageSize, totalRows); // 当前页的结束行
    int numRows = endRow - startRow; // 当前页的行数
    totalRows = 0;
    currentPage = 0,
    pageSize = 50,
    totalPages = ((totalRows + pageSize - 1) / pageSize);
    ui->textEdit_5->setHtml(" ");
    ui->textEdit_4->setHtml("");
    ui->label_5->setText("无数据显示");
    if(totalRowsLogList == 0) {
        totalRowsLogList = 1;
    }
    updateLogListTableDisplay();
}

void AShareDividendDifferentiatedDeclaration::updateLogListTableDisplay() {
    if(totalRowsLogList == 0) {
        ui->label_6->setText("无数据显示");
    } else {
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
        query.prepare("SELECT COUNT(*) FROM AShareDividendDifferentiatedLogList");
        if (query.exec()) {
            if (query.next()) {
                totalRowsLogList = query.value(0).toInt();
            }
            // 删除空行
        } else {
            QMessageBox::critical(nullptr, "错误", "查询执行失败：" + query.lastError().text());
            return;
        }

//        currentPageLogList = 0,
//        pageSizeLogList = 5,
        totalPagesLogList = ((totalRowsLogList + pageSizeLogList - 1) / pageSizeLogList);
        int startRow = currentPageLogList * pageSizeLogList; // 当前页的起始行
        int endRow = qMin(startRow + pageSizeLogList, totalRowsLogList); // 当前页的结束行
        int numRows = endRow - startRow; // 当前页的行数

        ui->tableWidget_3->clearContents(); // 清空表格内容
        ui->tableWidget_3->setRowCount(numRows);
        // 执行查询
//        query.prepare("SELECT * FROM AShareDividendDifferentiatedLogList LIMIT :startRow, :numRows");
        query.prepare("SELECT * FROM AShareDividendDifferentiatedLogList ORDER BY AShareDividendDifferentiatedLogList DESC LIMIT :startRow, :numRows");
        query.bindValue(":startRow", startRow);
        query.bindValue(":numRows", numRows);
        if (query.exec()) {
            int rowIndex = 0; // 当前页内的行索引

            while (query.next()) {
//                QString AShareDividendDifferentiatedLogListID = query.value(0).toString();
                QString ActionPeople = query.value(1).toString();
                QString AShareDividendDifferentiatedLogList = query.value(2).toString();
                QString LogDate = query.value(3).toString();
                QString LogTime = query.value(4).toString();
                QString BusinessDomainName = query.value(5).toString();
                QString BusinessName = query.value(6).toString();
                QString BusinessCode = query.value(7).toString();
                QString BackCode = query.value(8).toString();
                QString BackResult = query.value(9).toString();


                QTableWidgetItem *item1 = new QTableWidgetItem(AShareDividendDifferentiatedLogList);
                QTableWidgetItem *item2 = new QTableWidgetItem(ActionPeople);
                QTableWidgetItem *item3 = new QTableWidgetItem(LogDate);
                QTableWidgetItem *item4 = new QTableWidgetItem(LogTime);
                QTableWidgetItem *item5 = new QTableWidgetItem(BusinessDomainName);
                QTableWidgetItem *item6 = new QTableWidgetItem(BusinessName);
                QTableWidgetItem *item7 = new QTableWidgetItem(BusinessCode);
                QTableWidgetItem *item8 = new QTableWidgetItem(BackCode);
                QTableWidgetItem *item9 = new QTableWidgetItem(BackResult);

                ui->tableWidget_3->setItem(rowIndex, 0, item1);
                ui->tableWidget_3->setItem(rowIndex, 1, item2);
                ui->tableWidget_3->setItem(rowIndex, 2, item3);
                ui->tableWidget_3->setItem(rowIndex, 3, item4);
                ui->tableWidget_3->setItem(rowIndex, 4, item5);
                ui->tableWidget_3->setItem(rowIndex, 5, item6);
                ui->tableWidget_3->setItem(rowIndex, 6, item7);
                ui->tableWidget_3->setItem(rowIndex, 7, item8);
                ui->tableWidget_3->setItem(rowIndex, 8, item9);
                item1->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                item2->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                item3->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                item4->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                item5->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                item6->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                item7->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                item8->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                item9->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

                ++rowIndex;
            }
            // 删除空行
            removeEmptyRows(ui->tableWidget_3);
        } else {
            QMessageBox::critical(nullptr, "错误", "查询执行失败：" + query.lastError().text());
            return;
        }

        // 关闭数据库连接
        database.close();
        QString styledText1 ="<font color='red'>" + QString::number(currentPageLogList + 1) + "</font>";
        QString styledText2 ="<font color='black'>/</font>""<font color='red'>" + QString::number(totalPagesLogList) + "</font>";
        ui->textEdit_6->setHtml(styledText1);
        ui->textEdit_7->setHtml(styledText2);
        ui->label_6->setText(QString::number(startRow) + "-" + QString::number(endRow) + "条，共" + QString::number(numRows) + "条");
    }
}

void AShareDividendDifferentiatedDeclaration::topPageLogListButton_clicked()
{
    if (currentPageLogList != 0) {
        currentPageLogList = 0;
        updateLogListTableDisplay();
    }
    ui->pushButton_12->setEnabled(true);
    ui->pushButton_13->setEnabled(true);
    ui->pushButton_15->setEnabled(false);
    ui->pushButton_14->setEnabled(false);
}

void AShareDividendDifferentiatedDeclaration::previousLogListPageButton_clicked()
{
    if (currentPageLogList > 0) {
        currentPageLogList--;
        updateLogListTableDisplay();
    }
    if(currentPageLogList != 0) {
        ui->pushButton_15->setEnabled(true);
        ui->pushButton_14->setEnabled(true);
        ui->pushButton_12->setEnabled(true);
        ui->pushButton_13->setEnabled(true);
    } else {
        ui->pushButton_12->setEnabled(true);
        ui->pushButton_13->setEnabled(true);
        ui->pushButton_15->setEnabled(false);
        ui->pushButton_14->setEnabled(false);
    }
}

void AShareDividendDifferentiatedDeclaration::nextPageLogListButton_clicked()
{
    if (currentPageLogList < totalPagesLogList - 1) {
        currentPageLogList++;
        updateLogListTableDisplay();
    }
    if (currentPageLogList != totalPagesLogList - 1) {
        ui->pushButton_15->setEnabled(true);
        ui->pushButton_14->setEnabled(true);
        ui->pushButton_12->setEnabled(true);
        ui->pushButton_13->setEnabled(true);
    } else {
        ui->pushButton_9->setEnabled(false);
        ui->pushButton_13->setEnabled(false);
        ui->pushButton_15->setEnabled(true);
        ui->pushButton_14->setEnabled(true);
    }
}

void AShareDividendDifferentiatedDeclaration::bottomLogListPageButton_clicked()
{
    if (currentPageLogList != totalPagesLogList - 1) {
        currentPageLogList = totalPagesLogList - 1;
        updateLogListTableDisplay();
    }
    ui->pushButton_12->setEnabled(false);
    ui->pushButton_13->setEnabled(false);
    ui->pushButton_15->setEnabled(true);
    ui->pushButton_14->setEnabled(true);
}
