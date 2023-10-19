#include "deliveryManager.h"
#include "ui_deliverymanager.h"

#include <QAbstractItemView>

deliveryManager::deliveryManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::deliveryManager)
{
    ui->setupUi(this);

    navigation_model = new QStandardItemModel(this);
    ui->treeView->setModel(navigation_model);
    ui->treeView->setHeaderHidden(true);
    ui->treeView->setFocusPolicy(Qt::NoFocus);
    ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->treeView->setIconSize(QSize(18, 18));
    ui->treeView->setRootIsDecorated(false);    // 设置顶级节点无折叠/展开小图标
    ui->treeView->setIndentation(24);           // 父节点与子节点之间的水平距离

    setting_item = new QStandardItem(QIcon(":/image/delivery-manager.png"), tr("交收指令管理"));
    setting_item->appendRow(new QStandardItem("防误间隔配置"));
    setting_item->appendRow(new QStandardItem("防误间隔配置"));
    setting_item->appendRow(new QStandardItem("防误间隔配置"));
    setting_item->appendRow(new QStandardItem("防误间隔配置"));
    setting_item->appendRow(new QStandardItem("防误间隔配置"));
    navigation_model->appendRow(setting_item);

    setting_item1 = new QStandardItem(QIcon(":/image/delivery-query.png"), tr("交收结果查询"));
    setting_item1->appendRow(new QStandardItem("防误间隔配置"));
    setting_item1->appendRow(new QStandardItem("防误间隔配置"));
    setting_item1->appendRow(new QStandardItem("防误间隔配置"));
    setting_item1->appendRow(new QStandardItem("防误间隔配置"));
    navigation_model->appendRow(setting_item1);

    setting_item2 = new QStandardItem(QIcon(":/image/rent-unit.png"), tr("租用交易单元"));
    setting_item2->appendRow(new QStandardItem("防误间隔配置"));
    setting_item2->appendRow(new QStandardItem("防误间隔配置"));
    setting_item2->appendRow(new QStandardItem("防误间隔配置"));
    setting_item2->appendRow(new QStandardItem("防误间隔配置"));
    navigation_model->appendRow(setting_item2);

    setting_item3 = new QStandardItem(QIcon(":/image/liquidation-relation.png"), tr("清算路径关系"));
    setting_item3->appendRow(new QStandardItem("防误间隔配置"));
    setting_item3->appendRow(new QStandardItem("防误间隔配置"));
    setting_item3->appendRow(new QStandardItem("防误间隔配置"));
    setting_item3->appendRow(new QStandardItem("防误间隔配置"));
    navigation_model->appendRow(setting_item3);

    setting_item4 = new QStandardItem(QIcon(":/image/RTGS-manager.png"), tr("RTGS交收管理"));
    setting_item4->appendRow(new QStandardItem("RTGS勾单指令申报"));
    setting_item4->appendRow(new QStandardItem("RTGS参与人当日交收次查询"));
    setting_item4->appendRow(new QStandardItem("RTGS参与人历史交收查询"));
    setting_item4->appendRow(new QStandardItem("RTGS管理人当日交收查询"));
    setting_item4->appendRow(new QStandardItem("RTGS管理人历史交收查询"));
    setting_item4->appendRow(new QStandardItem("RTGS勾单模式设置"));
    setting_item4->appendRow(new QStandardItem("RTGS操作权限设置"));
    navigation_model->appendRow(setting_item4);

    //    打开菜单
    //    ui->treeView->expandAll();

    // signals and slots
    connect(navigation_model, SIGNAL(itemChanged(QStandardItem *)), this, SLOT(slot_item_changed(QStandardItem *)));




}

deliveryManager::~deliveryManager()
{
    delete ui;
}


void deliveryManager::on_treeView_clicked(const QModelIndex &index)
{
    if(navigation_model->itemFromIndex(ui->treeView->currentIndex())->hasChildren())
    {
        ui->treeView->isExpanded(index) ? ui->treeView->collapse(index) : ui->treeView->expand(index);
    }
    QStandardItem* clickedItem = navigation_model->itemFromIndex(index);
    if (clickedItem && !clickedItem->hasChildren()) {
        QString itemText = clickedItem->text().trimmed();
        qDebug() << "Clicked item text: " << itemText;
        if(itemText == "最高额度常规申报") {
            emit goToPage(itemText);
        }
    }
}
