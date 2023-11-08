#ifndef BUSINESSSUBMISSIONSESHADE_H
#define BUSINESSSUBMISSIONSESHADE_H

#include <QDialog>
#include <QMouseEvent>
#include <QDebug>
#include <QTableWidget>

namespace Ui {
class businessSubmissionsEshade;
}

class businessSubmissionsEshade : public QDialog
{
    Q_OBJECT

public:
    explicit businessSubmissionsEshade(QWidget *parent = nullptr);
    ~businessSubmissionsEshade();

private slots:
    void on_close_clicked();

private:
    void initStyle();
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    Ui::businessSubmissionsEshade *ui;
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
};

#endif // BUSINESSSUBMISSIONSESHADE_H