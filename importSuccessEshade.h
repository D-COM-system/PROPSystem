#ifndef IMPORTSUCCESSESHADE_H
#define IMPORTSUCCESSESHADE_H

#include <QDialog>
#include <QMouseEvent>
#include <QDebug>

namespace Ui {
class importSuccessEshade;
}

class importSuccessEshade : public QDialog
{
    Q_OBJECT

public:
    explicit importSuccessEshade(QWidget *parent = nullptr);
    ~importSuccessEshade();

private slots:
    void on_close_clicked();

private:
    void initStyle();
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    Ui::importSuccessEshade *ui;
    bool _bDrag = false;
    QPoint      mouseStartPoint;
    QPoint      windowTopLeftPoint;
    QPoint      parentTopLeftPoint;
};

#endif // IMPORTSUCCESSESHADE_H
