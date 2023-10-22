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
    setting_item->appendRow(new QStandardItem("防误间隔配置1"));
    setting_item->appendRow(new QStandardItem("防误间隔配置2"));
    setting_item->appendRow(new QStandardItem("防误间隔配置3"));
    setting_item->appendRow(new QStandardItem("防误间隔配置4"));
    navigation_model->appendRow(setting_item);

    setting_item1 = new QStandardItem(QIcon(":/image/systemfix.png"), tr("系统维护"));
    setting_item1->appendRow(new QStandardItem("防误间隔配置5"));
    setting_item1->appendRow(new QStandardItem("防误间隔配置6"));
    setting_item1->appendRow(new QStandardItem("防误间隔配置7"));
    setting_item1->appendRow(new QStandardItem("防误间隔配置8"));
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
    //分支页面跳转
    connect(ui->toolButton_12, &QPushButton::clicked, this, &dashboard::on_toolButton_12_clicked);
    connect(ui->toolButton_8, &QPushButton::clicked, this, &dashboard::on_toolButton_8_clicked);
}

dashboard::~dashboard()
{
    delete ui;
}

void dashboard::toggleDropDown()
{
    ui->widget_2->setVisible(!(ui->widget_2->isVisible()));
}

void dashboard::mousePressEvent(QMouseEvent *event)
{
    if (!ui->widget_2->geometry().contains(event->pos()))
    {
        // 点击widget_2以外的区域，关闭widget_2的显示
        ui->widget_2->setVisible(false);
    }
    QWidget::mousePressEvent(event);
}


void dashboard::on_treeView_clicked(const QModelIndex &index)
{
    if(navigation_model->itemFromIndex(ui->treeView->currentIndex())->hasChildren())
    {
        ui->treeView->isExpanded(index) ? ui->treeView->collapse(index) : ui->treeView->expand(index);
    }
    QStandardItem* clickedItem = navigation_model->itemFromIndex(index);
    if (clickedItem && !clickedItem->hasChildren()) {
        QString itemText = clickedItem->text();
        qDebug() << "Clicked item text: " << itemText;
    }
}


void dashboard::on_toolButton_12_clicked()
{
    // 处理点击事件
    riskmanagement = new riskManagement(this);
    QWidget *riskmanagementpage = riskmanagement->ui->page;
    QWidget *riskmanagementpage_3 = riskmanagement->ui->page_3;
    ui->stackedWidget->addWidget(riskmanagementpage);
    ui->stackedWidget->setCurrentWidget(riskmanagementpage);
    ui->stackedWidget_2->addWidget(riskmanagementpage_3);
    ui->stackedWidget_2->setCurrentWidget(riskmanagementpage_3);
    connect(riskmanagement, &riskManagement::goToPage, this, &dashboard::handleGoToPage);
}

//分支子页面跳转
void dashboard::handleGoToPage(QString data) {
    qDebug() << data;
    if(data == "最高额度常规申报") {
        maximumamountregulardeclaration = new MaximumAmountRegularDeclaration(this);
        QWidget *maximumamountregulardeclarationpage_3 = maximumamountregulardeclaration->ui->page_3;
        ui->stackedWidget_2->addWidget(maximumamountregulardeclarationpage_3);
        ui->stackedWidget_2->setCurrentWidget(maximumamountregulardeclarationpage_3);
    }
    if(data == "A股红利税差别化申报") {
        asharedividenddifferentiateddeclaration = new AShareDividendDifferentiatedDeclaration(this);
        QWidget *asharedividenddifferentiateddeclarationpage_3 = asharedividenddifferentiateddeclaration->ui->page_3;
        ui->stackedWidget_2->addWidget(asharedividenddifferentiateddeclarationpage_3);
        ui->stackedWidget_2->setCurrentWidget(asharedividenddifferentiateddeclarationpage_3);
    }
    if(data == "RTGS勾单指令申报") {
        rtgsdeclaration = new RTGSdeclaration(this);
        QWidget *rtgsdeclarationpage_3 = rtgsdeclaration->ui->page_3;
        ui->stackedWidget_2->addWidget(rtgsdeclarationpage_3);
        ui->stackedWidget_2->setCurrentWidget(rtgsdeclarationpage_3);
    }
}

void dashboard::on_toolButton_8_clicked()
{
    // 处理点击事件
    deliverymanager = new deliveryManager(this);
    QWidget *deliverymanagerpage = deliverymanager->ui->page;
    QWidget *deliverymanagerpage_3 = deliverymanager->ui->page_3;
    ui->stackedWidget->addWidget(deliverymanagerpage);
    ui->stackedWidget->setCurrentWidget(deliverymanagerpage);
    ui->stackedWidget_2->addWidget(deliverymanagerpage_3);
    ui->stackedWidget_2->setCurrentWidget(deliverymanagerpage_3);
    connect(deliverymanager, &deliveryManager::goToPage, this, &dashboard::handleGoToPage);
}

void dashboard::on_toolButton_9_clicked()
{
    // 处理点击事件
    fundmanagement = new FundManagement(this);
    QWidget *fundmanagementpage = fundmanagement->ui->page;
    QWidget *fundmanagementpage_3 = fundmanagement->ui->page_3;
    ui->stackedWidget->addWidget(fundmanagementpage);
    ui->stackedWidget->setCurrentWidget(fundmanagementpage);
    ui->stackedWidget_2->addWidget(fundmanagementpage_3);
    ui->stackedWidget_2->setCurrentWidget(fundmanagementpage_3);
    connect(fundmanagement, &FundManagement::goToPage, this, &dashboard::handleGoToPage);
}

void dashboard::on_toolButton_11_clicked()
{
    // 处理点击事件
    securitiesregistrationtop = new SecuritiesRegistrationtop(this);
    QWidget *securitiesregistrationtoppage = securitiesregistrationtop->ui->page;
    QWidget *securitiesregistrationtoppage_3 = securitiesregistrationtop->ui->page_3;
    ui->stackedWidget->addWidget(securitiesregistrationtoppage);
    ui->stackedWidget->setCurrentWidget(securitiesregistrationtoppage);
    ui->stackedWidget_2->addWidget(securitiesregistrationtoppage_3);
    ui->stackedWidget_2->setCurrentWidget(securitiesregistrationtoppage_3);
    connect(securitiesregistrationtop, &SecuritiesRegistrationtop::goToPage, this, &dashboard::handleGoToPage);
}

