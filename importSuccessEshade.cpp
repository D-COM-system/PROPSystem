#include "importSuccessEshade.h"
#include "ui_importSuccessEshade.h"

importSuccessEshade::importSuccessEshade(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::importSuccessEshade)
{
    ui->setupUi(this);

    // 将父窗口设置为主窗口
     setParent(parent);

    // 禁用窗口的调整大小和移动
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog | Qt::WindowStaysOnTopHint);

    // 设置窗口样式和背景
    setStyleSheet("background: rgba(0, 0, 0, 0.8);");

    // 将窗口居中显示在主窗口内
     int pWidth = parent->width();
     int pHeight = parent->height();
     int width = ui->tableWidget->width();
     int height = ui->tableWidget->height();
     move((pWidth - width)/3, (pHeight - height)/3);

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
