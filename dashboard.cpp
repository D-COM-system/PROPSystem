#include "dashboard.h"
#include "ui_dashboard.h"
#include <QPropertyAnimation>
#include <QDebug>

dashboard::dashboard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::dashboard),
    animationRunning(false) // 添加标志变量并初始化为false
{
    ui->setupUi(this);
    ui->widget_2->setVisible(false);
    connect(ui->pushButton, &QPushButton::clicked, this, &dashboard::toggleDropDown);
}

dashboard::~dashboard()
{
    delete ui;
}

void dashboard::toggleDropDown()
{
    ui->widget_2->setVisible(!(ui->widget_2->isVisible()));
}
