#include "fundmanagement.h"
#include "ui_fundmanagement.h"

FundManagement::FundManagement(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FundManagement)
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

    setting_item = new QStandardItem(QIcon(":/image/fundin.png"), tr("资金入账"));
    setting_item->appendRow(new QStandardItem("防误间隔配置"));
    navigation_model->appendRow(setting_item);

    setting_item1 = new QStandardItem(QIcon(":/image/fundtransfer.png"), tr("资金划拨"));
    setting_item1->appendRow(new QStandardItem("防误间隔配置"));
    navigation_model->appendRow(setting_item1);

    setting_item2 = new QStandardItem(QIcon(":/image/pre-transfer.png"), tr("预约划款"));
    setting_item2->appendRow(new QStandardItem("防误间隔配置"));
    navigation_model->appendRow(setting_item2);

    setting_item3 = new QStandardItem(QIcon(":/image/messagequery.png"), tr("信息查询"));
    setting_item3->appendRow(new QStandardItem("资金余额查询"));
    setting_item3->appendRow(new QStandardItem("资金变动查询"));
    setting_item3->appendRow(new QStandardItem("预留账户查询"));
    setting_item3->appendRow(new QStandardItem("A股结息凭证查询"));
    setting_item3->appendRow(new QStandardItem("网下配售对象资金到账查询"));
    setting_item3->appendRow(new QStandardItem("A股资金账户列表查询"));
    navigation_model->appendRow(setting_item3);

    setting_item4 = new QStandardItem(QIcon(":/image/fundconfirm.png"), tr("资金询证"));
    setting_item4->appendRow(new QStandardItem("防误间隔配置"));
    navigation_model->appendRow(setting_item4);

    setting_item5 = new QStandardItem(QIcon(":/image/B-fund.png"), tr("B股资金"));
    setting_item5->appendRow(new QStandardItem("防误间隔配置"));
    navigation_model->appendRow(setting_item5);

    setting_item6 = new QStandardItem(QIcon(":/image/refinancing.png"), tr("转融通资金代收付"));
    setting_item6->appendRow(new QStandardItem("防误间隔配置"));
    navigation_model->appendRow(setting_item6);

    setting_item7 = new QStandardItem(QIcon(":/image/quotationsys.png"), tr("报价系统资金代收付"));
    setting_item7->appendRow(new QStandardItem("防误间隔配置"));
    navigation_model->appendRow(setting_item7);

    setting_item8 = new QStandardItem(QIcon(":/image/open-upfund.png"), tr("开放式基金资金代收付"));
    setting_item8->appendRow(new QStandardItem("防误间隔配置"));
    navigation_model->appendRow(setting_item8);

    setting_item9 = new QStandardItem(QIcon(":/image/redemption.png"), tr("申赎及其他代收付"));
    setting_item9->appendRow(new QStandardItem("防误间隔配置"));
    navigation_model->appendRow(setting_item9);

    setting_item10 = new QStandardItem(QIcon(":/image/accessibility.png"), tr("辅助功能"));
    setting_item10->appendRow(new QStandardItem("防误间隔配置"));
    navigation_model->appendRow(setting_item10);

    //    打开菜单
    //    ui->treeView->expandAll();

    // signals and slots
    connect(navigation_model, SIGNAL(itemChanged(QStandardItem *)), this, SLOT(slot_item_changed(QStandardItem *)));

}

FundManagement::~FundManagement()
{
    delete ui;
}

void FundManagement::on_treeView_clicked(const QModelIndex &index)
{
    if(navigation_model->itemFromIndex(ui->treeView->currentIndex())->hasChildren())
    {
        ui->treeView->isExpanded(index) ? ui->treeView->collapse(index) : ui->treeView->expand(index);
    }
    QStandardItem* clickedItem = navigation_model->itemFromIndex(index);
    if (clickedItem && !clickedItem->hasChildren()) {
        QString itemText = clickedItem->text().trimmed();
        qDebug() << "Clicked item text: " << itemText;
        if(itemText == "资金余额查询") {
            emit goToPage(itemText);
        }
    }
}
