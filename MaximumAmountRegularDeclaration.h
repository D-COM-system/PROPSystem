#ifndef MAXIMUMAMOUNTREGULARDECLARATION_H
#define MAXIMUMAMOUNTREGULARDECLARATION_H

#include "globaldata.h"
#include "importSuccessEshade.h"
#include "ui_importSuccessEshade.h"

#include <QTableWidget>
#include <QWidget>

namespace Ui {
class MaximumAmountRegularDeclaration;
}

class MaximumAmountRegularDeclaration : public QWidget
{
    Q_OBJECT

public:
    explicit MaximumAmountRegularDeclaration(QWidget *parent = nullptr);
    ~MaximumAmountRegularDeclaration();

public:
    Ui::MaximumAmountRegularDeclaration *ui;
    int currentPage; // 当前页码
    int pageSize; // 每页显示的行数
    int totalRows; // 总行数
    int totalPages; // 总页数
    void updateTableDisplay();
    void removeEmptyRows(QTableWidget *tableWidget);

private slots:
    void chooseFile();
    void previousPageButton_clicked();
    void nextPageButton_clicked();
    void topPageButton_clicked();
    void bottomPageButton_clicked();
    bool judgeFile();
    void businessSubmissions();

public:
    void setImportNumber(const QString& data);
};

#endif // MAXIMUMAMOUNTREGULARDECLARATION_H
