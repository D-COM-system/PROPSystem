#include "importSuccessEshade.h"
#include "GlobalData.h"
#include "ui_importSuccessEshade.h"

#include <QVBoxLayout>

importSuccessEshade::importSuccessEshade(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::importSuccessEshade)
{
    ui->setupUi(this);

    resize(parent->size());
    // 获取父界面的大小
    QSize parentSize = parent->size();

    // 计算窗口的目标大小为父界面大小的百分之80
    int targetWidth = parentSize.width() * 0.8;
    int targetHeight = parentSize.height() * 0.8;

    // 设置窗口的最小大小和最大大小为目标大小
    setMinimumSize(targetWidth, targetHeight);
    setMaximumSize(targetWidth, targetHeight);
    int pWidth = targetWidth;
    int pHeight = targetHeight;
    int width = ui->widget_5->geometry().width();
    int height = ui->widget_5->geometry().height();
    QPointF topLeft = frameGeometry().topLeft();
    qDebug() << topLeft;
    qDebug() << "[" << topLeft.x() + (pWidth - width)/ 2  << "] [" << topLeft.y() + (pHeight - height)/2 << "]";
    ui->widget_5->move((pWidth - width)/ 2, (pHeight - height)/2);
    ui->lineEdit_12->setText("本次共导入" + efficientImportNumber + "条数据");
    initStyle();
}

importSuccessEshade::~importSuccessEshade()
{
    delete ui;
}

void importSuccessEshade::initStyle()
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setModal(true);
    setStyleSheet("background: rgba(0, 0, 0, 0.8);");
}

void importSuccessEshade::mousePressEvent(QMouseEvent *event)
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

void importSuccessEshade::mouseMoveEvent(QMouseEvent *event)
{
    if (_bDrag)
    {
        // 获得鼠标移动的距离
        QPoint distance = event->globalPos() - mouseStartPoint;
        // 改变窗口的位置
        this->move(windowTopLeftPoint + distance);
    }
}

void importSuccessEshade::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        _bDrag = false;
    }
}

void importSuccessEshade::on_close_clicked()
{
    // 关闭窗口前删除窗口实例
    deleteLater();
}
