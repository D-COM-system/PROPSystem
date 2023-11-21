#ifndef ASHAREDIVIDENDDIFFERENTIATEDDECLARATION_H
#define ASHAREDIVIDENDDIFFERENTIATEDDECLARATION_H

#include <QTableWidget>
#include <QWidget>

namespace Ui {
class AShareDividendDifferentiatedDeclaration;
}

class AShareDividendDifferentiatedDeclaration : public QWidget
{
    Q_OBJECT

public:
    explicit AShareDividendDifferentiatedDeclaration(QWidget *parent = nullptr);
    ~AShareDividendDifferentiatedDeclaration();

public:
    Ui::AShareDividendDifferentiatedDeclaration *ui;
    int currentPage; // 当前页码
    int pageSize; // 每页显示的行数
    int totalRows; // 总行数
    int totalPages; // 总页数
    int currentPageLogList; // LogList当前页码
    int pageSizeLogList; // LogList每页显示的行数
    int totalRowsLogList; // LogList总行数
    int totalPagesLogList; // LogList总页数
    void updateTableDisplay();
    void updateLogListTableDisplay();
    void removeEmptyRows(QTableWidget *tableWidget);

private slots:
    void chooseFile();
    void importFile();
    void resetText();
    bool judgeFile();
    void previousPageButton_clicked();
    void nextPageButton_clicked();
    void topPageButton_clicked();
    void bottomPageButton_clicked();
    void previousLogListPageButton_clicked();
    void nextPageLogListButton_clicked();
    void topPageLogListButton_clicked();
    void bottomLogListPageButton_clicked();
    void submitData();
};

#endif // ASHAREDIVIDENDDIFFERENTIATEDDECLARATION_H
