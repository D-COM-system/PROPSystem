#include "dashboard.h"
#include "ui_dashboard.h"

#include <QPropertyAnimation>
#include <QDebug>
#include <QFile>



dashboard::dashboard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::dashboard)
{
    ui->setupUi(this);
//    ui->widget->setFixedSize(1000,500);
    ui->widget_2->setVisible(false);

    navigation_model = new QStandardItemModel(this);
    ui->treeView->setModel(navigation_model);
    ui->treeView->setHeaderHidden(true);
    ui->treeView->setFocusPolicy(Qt::NoFocus);
    ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->treeView->setIconSize(QSize(18, 18));
    ui->treeView->setRootIsDecorated(false);    // 设置顶级节点无折叠/展开小图标
    ui->treeView->setIndentation(24);           // 父节点与子节点之间的水平距离

    setting_item = new QStandardItem(QIcon(":/image/usercontrol.png"), tr("用户管理"));
    setting_item->appendRow(new QStandardItem("防误间隔配置"));
    setting_item->appendRow(new QStandardItem("防误间隔配置"));
    setting_item->appendRow(new QStandardItem("防误间隔配置"));
    setting_item->appendRow(new QStandardItem("防误间隔配置"));
    navigation_model->appendRow(setting_item);

    setting_item1 = new QStandardItem(QIcon(":/image/systemfix.png"), tr("系统维护"));
    setting_item1->appendRow(new QStandardItem("防误间隔配置"));
    setting_item1->appendRow(new QStandardItem("防误间隔配置"));
    setting_item1->appendRow(new QStandardItem("防误间隔配置"));
    setting_item1->appendRow(new QStandardItem("防误间隔配置"));
    navigation_model->appendRow(setting_item1);

    setting_item2 = new QStandardItem(QIcon(":/image/messageserver.png"), tr("短信订阅服务"));
    setting_item2->appendRow(new QStandardItem("防误间隔配置"));
    setting_item2->appendRow(new QStandardItem("防误间隔配置"));
    setting_item2->appendRow(new QStandardItem("防误间隔配置"));
    setting_item2->appendRow(new QStandardItem("防误间隔配置"));
    navigation_model->appendRow(setting_item2);
//    打开菜单
//    ui->treeView->expandAll();

    // signals and slots
    connect(navigation_model, SIGNAL(itemChanged(QStandardItem *)), this, SLOT(slot_item_changed(QStandardItem *)));

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


void dashboard::on_treeView_clicked(const QModelIndex &index)
{
    if(navigation_model->itemFromIndex(ui->treeView->currentIndex())->hasChildren())
    {
        ui->treeView->isExpanded(index) ? ui->treeView->collapse(index) : ui->treeView->expand(index);
    }
//    QModelIndex setting_item_index = navigation_model->indexFromItem(setting_item);
    QModelIndex setting_item1_index = navigation_model->indexFromItem(setting_item1);
    QModelIndex setting_item2_index = navigation_model->indexFromItem(setting_item2);
//    if (ui->treeView->isExpanded(setting_item_index)) {
//        // 处理处于打开状态的情况
//        setting_item->setIcon(QIcon(":/image/closemenu.png"));
//    } else {
//        // 处理未打开状态的情况
//        setting_item->setIcon(QIcon(":/image/openmenu.png"));
//    }
    if (ui->treeView->isExpanded(setting_item1_index)) {
        // 处理处于打开状态的情况
        setting_item1->setIcon(QIcon(":/image/closemenu.png"));
    } else {
        // 处理未打开状态的情况
        setting_item1->setIcon(QIcon(":/image/openmenu.png"));
    }
    if (ui->treeView->isExpanded(setting_item2_index)) {
        // 处理处于打开状态的情况
        setting_item2->setIcon(QIcon(":/image/closemenu.png"));
    } else {
        // 处理未打开状态的情况
        setting_item2->setIcon(QIcon(":/image/openmenu.png"));
    }
}

