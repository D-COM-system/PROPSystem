#include "riskManagement.h"
#include "ui_riskManagement.h"

riskManagement::riskManagement(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::riskManagement)
{
    ui->setupUi(this);
    //    ui->widget->setFixedSize(1000,500);

    navigation_model = new QStandardItemModel(this);
    ui->treeView->setModel(navigation_model);
    ui->treeView->setHeaderHidden(true);
    ui->treeView->setFocusPolicy(Qt::NoFocus);
    ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->treeView->setIconSize(QSize(18, 18));
    ui->treeView->setRootIsDecorated(false);    // 设置顶级节点无折叠/展开小图标
    ui->treeView->setIndentation(24);           // 父节点与子节点之间的水平距离

    setting_item = new QStandardItem(QIcon(":/image/usercontrol.png"), tr("资金前端控制"));
    setting_item->appendRow(new QStandardItem("最高额度常规申报"));
    setting_item->appendRow(new QStandardItem("盘中紧急调整申报"));
    setting_item->appendRow(new QStandardItem("盘中紧急调整撤销"));
    setting_item->appendRow(new QStandardItem("最高额度申报历史查询"));
    setting_item->appendRow(new QStandardItem("最高额度生效情况查询"));
    navigation_model->appendRow(setting_item);

    setting_item1 = new QStandardItem(QIcon(":/image/systemfix.png"), tr("交收违约处理"));
    setting_item1->appendRow(new QStandardItem("防误间隔配置"));
    setting_item1->appendRow(new QStandardItem("防误间隔配置"));
    setting_item1->appendRow(new QStandardItem("防误间隔配置"));
    setting_item1->appendRow(new QStandardItem("防误间隔配置"));
    navigation_model->appendRow(setting_item1);

    setting_item2 = new QStandardItem(QIcon(":/image/messageserver.png"), tr("货币基金净申赎额度"));
    setting_item2->appendRow(new QStandardItem("防误间隔配置"));
    setting_item2->appendRow(new QStandardItem("防误间隔配置"));
    setting_item2->appendRow(new QStandardItem("防误间隔配置"));
    setting_item2->appendRow(new QStandardItem("防误间隔配置"));
    navigation_model->appendRow(setting_item2);

    setting_item3 = new QStandardItem(QIcon(":/image/messageserver.png"), tr("货币基金净申赎额度"));
    setting_item3->appendRow(new QStandardItem("防误间隔配置"));
    setting_item3->appendRow(new QStandardItem("防误间隔配置"));
    setting_item3->appendRow(new QStandardItem("防误间隔配置"));
    setting_item3->appendRow(new QStandardItem("防误间隔配置"));
    navigation_model->appendRow(setting_item3);

    setting_item4 = new QStandardItem(QIcon(":/image/messageserver.png"), tr("货银对付申报管理"));
    setting_item4->appendRow(new QStandardItem("防误间隔配置"));
    setting_item4->appendRow(new QStandardItem("防误间隔配置"));
    setting_item4->appendRow(new QStandardItem("防误间隔配置"));
    setting_item4->appendRow(new QStandardItem("防误间隔配置"));
    navigation_model->appendRow(setting_item4);

    setting_item5 = new QStandardItem(QIcon(":/image/messageserver.png"), tr("债券质押式回购"));
    setting_item5->appendRow(new QStandardItem("防误间隔配置"));
    setting_item5->appendRow(new QStandardItem("防误间隔配置"));
    setting_item5->appendRow(new QStandardItem("防误间隔配置"));
    setting_item5->appendRow(new QStandardItem("防误间隔配置"));
    navigation_model->appendRow(setting_item5);
    //    打开菜单
    //    ui->treeView->expandAll();

    // signals and slots
    connect(navigation_model, SIGNAL(itemChanged(QStandardItem *)), this, SLOT(slot_item_changed(QStandardItem *)));
}

riskManagement::~riskManagement()
{
    delete ui;
}

void riskManagement::on_treeView_clicked(const QModelIndex &index)
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

