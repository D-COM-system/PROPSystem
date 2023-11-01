#ifndef RTGSDECLARATION_H
#define RTGSDECLARATION_H

#include "globaldata.h"

#include <QWidget>
#include <QTableWidget>

namespace Ui {
class RTGSdeclaration;
}


class RTGSdeclaration : public QWidget
{
    Q_OBJECT

public:
    explicit RTGSdeclaration(QWidget *parent = nullptr);
    ~RTGSdeclaration();

public:
    Ui::RTGSdeclaration *ui;
    int currentPage;
    int pageSize;
    int totalRows;
    int totalPages;
    QVector<int> selectedRows; // 存储选中的行索引
    void updateTableDisplay();
    void removeEmptyRows(QTableWidget *tableWidget);

private slots:
    void previousPageButton_clicked();
    void nextPageButton_clicked();
    void topPageButton_clicked();
    void bottomPageButton_clicked();
    void selectRows();
    void AllCheckbox(bool checked);
    void onTextChanged();
    void comboBoxAddItem();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

signals:
    void goToPage(QString);
};

#endif // RTGSDECLARATION_H
