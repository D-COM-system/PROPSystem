#include "businessSubmissionsEshade.h"
#include "GlobalData.h"
#include "ui_businessSubmissionsEshade.h"

businessSubmissionsEshade::businessSubmissionsEshade(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::businessSubmissionsEshade)
{
    ui->setupUi(this);

    totalRows = businessSubmissionsList.length();
    currentPage = 0,
    pageSize = 50,
    totalPages = ((totalRows + pageSize - 1) / pageSize);

    QStringList headerLabels;
    headerLabels << "结果代码" << "结果说明" << "机构代码" << "机构名称" << "控制类别" << "净资本/合计资产总额（百万）" << "最高额度（百万）";
    ui->tableWidget_2->clear(); // 清空表格内容
    ui->tableWidget_2->setColumnCount(headerLabels.size());
    ui->tableWidget_2->setHorizontalHeaderLabels(headerLabels);
    ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_2->setRowCount(pageSize);
    ui->tableWidget_2->verticalHeader()->hide();

    initStyle();
    connect(ui->pushButton_7, &QPushButton::clicked, this, &businessSubmissionsEshade::topPageButton_clicked);
    connect(ui->pushButton_8, &QPushButton::clicked, this, &businessSubmissionsEshade::previousPageButton_clicked);
    connect(ui->pushButton_9, &QPushButton::clicked, this, &businessSubmissionsEshade::nextPageButton_clicked);
    connect(ui->pushButton_10, &QPushButton::clicked, this, &businessSubmissionsEshade::bottomPageButton_clicked);
}

businessSubmissionsEshade::~businessSubmissionsEshade()
{
    delete ui;
}

void businessSubmissionsEshade::initStyle()
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setModal(true);
    setStyleSheet("background: rgba(212, 212, 212, 0.1);");
    updateTableDisplay();
}

void businessSubmissionsEshade::mousePressEvent(QMouseEvent *event)
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

void businessSubmissionsEshade::mouseMoveEvent(QMouseEvent *event)
{
    if (_bDrag)
    {
        // 获得鼠标移动的距离
        QPoint distance = event->globalPos() - mouseStartPoint;
        // 改变窗口的位置
        this->move(windowTopLeftPoint + distance);
    }
}

void businessSubmissionsEshade::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        _bDrag = false;
    }
}

void businessSubmissionsEshade::updateTableDisplay() {
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
            int columnCount = record.size();
            for (int column = 0; column < columnCount; ++column) {
                if(column == 0) {
                    QTableWidgetItem* item = new QTableWidgetItem("0000");
                    ui->tableWidget_2->setItem(row, column, item);
                } else if (column == 1) {
                    QTableWidgetItem* item = new QTableWidgetItem("申报成功");
                    ui->tableWidget_2->setItem(row, column, item);
                } else {
                    QTableWidgetItem* item = new QTableWidgetItem(record.at(column));
                    ui->tableWidget_2->setItem(row, column, item);
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

void businessSubmissionsEshade::removeEmptyRows(QTableWidget *tableWidget) {
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

void businessSubmissionsEshade::on_close_clicked()
{
    // 关闭窗口前删除窗口实例
    deleteLater();
}

void businessSubmissionsEshade::topPageButton_clicked()
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

void businessSubmissionsEshade::previousPageButton_clicked()
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

void businessSubmissionsEshade::nextPageButton_clicked()
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

void businessSubmissionsEshade::bottomPageButton_clicked()
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
