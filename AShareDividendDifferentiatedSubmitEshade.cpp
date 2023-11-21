#include "AShareDividendDifferentiatedSubmitEshade.h"
#include "GlobalData.h"
#include "qdatetime.h"
#include "ui_AShareDividendDifferentiatedSubmitEshade.h"

AShareDividendDifferentiatedSubmitEshade::AShareDividendDifferentiatedSubmitEshade(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AShareDividendDifferentiatedSubmitEshade)
{
    ui->setupUi(this);
    totalRows = businessSubmissionsList.length();
    currentPage = 0,
    pageSize = 50,
    totalPages = ((totalRows + pageSize - 1) / pageSize);

    QStringList headerLabels;
    headerLabels << "SBBH" << "CLJG" << "FZDM" << "BY1" << "BY2" << "JGDM" << "JGSM" << "BZ";
    ui->tableWidget_2->clear(); // 清空表格内容
    ui->tableWidget_2->setColumnCount(headerLabels.size());
    ui->tableWidget_2->setHorizontalHeaderLabels(headerLabels);
    ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_2->setRowCount(pageSize);
    ui->tableWidget_2->verticalHeader()->hide();
    //让tableWidget内容中的每个元素居中
    ui->tableWidget_2->setSelectionBehavior(QAbstractItemView::SelectRows);//设置整行选中
    ui->tableWidget_2->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter);//表头字体居中
    ui->tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers);//单元格不可编辑
    // 设置表头的底色和文字加粗
    QString styleSheet = "QHeaderView::section { background-color: #f5f5f5; font-weight: bold; }";
    // 将样式表应用于表格的表头
    ui->tableWidget_2->horizontalHeader()->setStyleSheet(styleSheet);
    // 获取当前时间
    QDateTime currentTime = QDateTime::currentDateTime();

    // 格式化时间为"YYYY-MM-DD HH:MM:SS"的字符串
    QString formattedTime = currentTime.toString("yyyy-MM-dd hh:mm:ss");
    // 将当前时间加一秒，并设置为resultTime
    QDateTime nextTime = currentTime.addSecs(1);
    QString resultTime = nextTime.toString("yyyy-MM-dd hh:mm:ss");

    // 设置lineEdit_2的文本为当前时间
    ui->lineEdit->setText("请求时间：" + formattedTime);
    ui->lineEdit_2->setText("应答时间：" + resultTime);
    ui->lineEdit_3->setText("处理结果：0000-申报成功");


    initStyle();
    connect(ui->pushButton_7, &QPushButton::clicked, this, &AShareDividendDifferentiatedSubmitEshade::topPageButton_clicked);
    connect(ui->pushButton_8, &QPushButton::clicked, this, &AShareDividendDifferentiatedSubmitEshade::previousPageButton_clicked);
    connect(ui->pushButton_9, &QPushButton::clicked, this, &AShareDividendDifferentiatedSubmitEshade::nextPageButton_clicked);
    connect(ui->pushButton_10, &QPushButton::clicked, this, &AShareDividendDifferentiatedSubmitEshade::bottomPageButton_clicked);
}

AShareDividendDifferentiatedSubmitEshade::~AShareDividendDifferentiatedSubmitEshade()
{
    delete ui;
}

void AShareDividendDifferentiatedSubmitEshade::initStyle()
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setModal(true);
    setStyleSheet("background: rgba(212, 212, 212, 0.1);");
    updateTableDisplay();
}

void AShareDividendDifferentiatedSubmitEshade::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        _bDrag = true;
        // 获得鼠标的初始位置
        mouseStartPoint = event->globalPos();
        // 获得窗口的初始位置
        windowTopLeftPoint = this->geometry().topLeft();
        qDebug() << windowTopLeftPoint;
    }
}

void AShareDividendDifferentiatedSubmitEshade::mouseMoveEvent(QMouseEvent *event)
{
    if (_bDrag)
    {
        // 获得鼠标移动的距离
        QPoint distance = event->globalPos() - mouseStartPoint;
        // 改变窗口的位置
        this->move(windowTopLeftPoint + distance);
    }
}

void AShareDividendDifferentiatedSubmitEshade::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        _bDrag = false;
    }
}

void AShareDividendDifferentiatedSubmitEshade::updateTableDisplay() {
    if(totalRows == 0) {
        ui->label_3->setText("无数据显示");
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
            QStringList record = databaseRecords.at(row + startRow);
            int columnCount = ui->tableWidget_2->columnCount();
            for (int column = 0; column < columnCount; ++column) {
                if(column == 0){
                    QTableWidgetItem* item = new QTableWidgetItem(record.at(0));
                    ui->tableWidget_2->setItem(row, column, item);
                    item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                } else if(column == 1) {
                    QTableWidgetItem* item1 = new QTableWidgetItem("P");
                    ui->tableWidget_2->setItem(row, column, item1);
                    item1->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                } else if(column == 2) {
                    QTableWidgetItem* item2 = new QTableWidgetItem("HL");
                    ui->tableWidget_2->setItem(row, column, item2);
                    item2->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                } else if(column == 3 || column == 4 || column == 7) {
                    QTableWidgetItem* item3 = new QTableWidgetItem("");
                    ui->tableWidget_2->setItem(row, column, item3);
                    item3->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                }
                else if(column == 5){
                    QTableWidgetItem* item4 = new QTableWidgetItem("0000");
                    ui->tableWidget_2->setItem(row, column, item4);
                    item4->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                } else if(column == 6){
                    QTableWidgetItem* item5 = new QTableWidgetItem("申报成功");
                    ui->tableWidget_2->setItem(row, column, item5);
                    item5->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
                }
            }
        }
        removeEmptyRows(ui->tableWidget_2);
        QString styledText1 ="<font color='red'>" + QString::number(currentPage + 1) + "</font>";
        QString styledText2 ="<font color='black'>/</font>""<font color='red'>" + QString::number(totalPages) + "</font>";
        ui->textEdit_5->setHtml(styledText1);
        ui->textEdit_4->setHtml(styledText2);
        ui->label_3->setText(QString::number(startRow) + "-" + QString::number(endRow) + "条，共" + QString::number(numRows) + "条");
    }
}

void AShareDividendDifferentiatedSubmitEshade::removeEmptyRows(QTableWidget *tableWidget) {
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

void AShareDividendDifferentiatedSubmitEshade::on_close_clicked()
{
    // 关闭窗口前删除窗口实例
    deleteLater();
}

void AShareDividendDifferentiatedSubmitEshade::topPageButton_clicked()
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

void AShareDividendDifferentiatedSubmitEshade::previousPageButton_clicked()
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

void AShareDividendDifferentiatedSubmitEshade::nextPageButton_clicked()
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

void AShareDividendDifferentiatedSubmitEshade::bottomPageButton_clicked()
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

