#ifndef ASHAREDIVIDENDDIFFERENTIATEDSUBMITESHADE_H
#define ASHAREDIVIDENDDIFFERENTIATEDSUBMITESHADE_H

#include <QDialog>
#include <QMouseEvent>
#include <QDebug>
#include <QTableWidget>

namespace Ui {
class AShareDividendDifferentiatedSubmitEshade;
}

class AShareDividendDifferentiatedSubmitEshade : public QDialog
{
    Q_OBJECT

public:
    explicit AShareDividendDifferentiatedSubmitEshade(QWidget *parent = nullptr);
    ~AShareDividendDifferentiatedSubmitEshade();

private:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    Ui::AShareDividendDifferentiatedSubmitEshade *ui;
    bool _bDrag = false;
    QPoint      mouseStartPoint;
    QPoint      windowTopLeftPoint;
    QPoint      parentTopLeftPoint;
    int currentPage; // 当前页码
    int pageSize; // 每页显示的行数
    int totalRows; // 总行数
    int totalPages; // 总页数
    void updateTableDisplay();
    void removeEmptyRows(QTableWidget *tableWidget);

private slots:
    void initStyle();
    void previousPageButton_clicked();
    void nextPageButton_clicked();
    void topPageButton_clicked();
    void bottomPageButton_clicked();
    void on_close_clicked();
};

#endif // ASHAREDIVIDENDDIFFERENTIATEDSUBMITESHADE_H
