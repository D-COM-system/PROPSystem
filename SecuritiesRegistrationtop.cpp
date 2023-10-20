#include "SecuritiesRegistrationtop.h"
#include "ui_SecuritiesRegistrationtop.h"

SecuritiesRegistrationtop::SecuritiesRegistrationtop(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SecuritiesRegistrationtop)
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

    setting_item = new QStandardItem(QIcon(":/image/dividends.png"), tr("红利税差别化缴纳"));
    setting_item->appendRow(new QStandardItem("A股红利税差别化申报"));
    setting_item->appendRow(new QStandardItem("A股红利税差别化查询"));
    setting_item->appendRow(new QStandardItem("B股红利税差别化申报"));
    setting_item->appendRow(new QStandardItem("B股红利税差别化查询"));
    navigation_model->appendRow(setting_item);

    setting_item1 = new QStandardItem(QIcon(":/image/accessibility.png"), tr("辅助功能"));
    navigation_model->appendRow(setting_item1);
    //    打开菜单
    //    ui->treeView->expandAll();

    // signals and slots
    connect(navigation_model, SIGNAL(itemChanged(QStandardItem *)), this, SLOT(slot_item_changed(QStandardItem *)));
}

SecuritiesRegistrationtop::~SecuritiesRegistrationtop()
{
    delete ui;
}

void SecuritiesRegistrationtop::on_treeView_clicked(const QModelIndex &index)
{
    if(navigation_model->itemFromIndex(ui->treeView->currentIndex())->hasChildren())
    {
        ui->treeView->isExpanded(index) ? ui->treeView->collapse(index) : ui->treeView->expand(index);
    }
    QStandardItem* clickedItem = navigation_model->itemFromIndex(index);
    if (clickedItem && !clickedItem->hasChildren()) {
        QString itemText = clickedItem->text().trimmed();
        qDebug() << "Clicked item text: " << itemText;
        if(itemText == "A股红利税差别化申报") {
            emit goToPage(itemText);
        }
    }
}
