#ifndef FUNDBALANCEQUERY_H
#define FUNDBALANCEQUERY_H

#include "globaldata.h"

#include <QWidget>
#include <QTableWidget>
#include <QWidget>
#include <QVector>
#include <QMap>
#include <QTableWidget>
#include <QHeaderView>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>
#include "balancequery.h"
#include "ui_balancequery.h"

namespace Ui {
class FundBalanceQuery;
}

class FundBalanceQuery : public QWidget
{
    Q_OBJECT

public:
    explicit FundBalanceQuery(QWidget *parent = nullptr);
    ~FundBalanceQuery();

public:
    Ui::FundBalanceQuery *ui;
    int currentPage; // 当前页码
    int pageSize; // 每页显示的行数
    int totalRows; // 总行数
    int totalPages; // 总页数
    QVector<int> selectedRows; // 存储选中的行索引
    void updateTableDisplay();
    void removeEmptyRows(QTableWidget *tableWidget);

private slots:
    void selectRows();
    void Balancequery();
    //void nextPageButton_clicked();
    void AllCheckbox(bool checked);
    void siftToData();
};

#endif // FUNDBALANCEQUERY_H
