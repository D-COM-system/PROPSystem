#ifndef DASHBOARD_H
#define DASHBOARD_H

#include "riskManagement.h"
#include "ui_riskManagement.h"
#include "MaximumAmountRegularDeclaration.h"
#include "ui_MaximumAmountRegularDeclaration.h"
#include "deliveryManager.h"
#include "ui_deliveryManager.h"
#include "SecuritiesRegistrationtop.h"
#include "ui_SecuritiesRegistrationtop.h"
#include "AShareDividendDifferentiatedDeclaration.h"
#include "ui_AShareDividendDifferentiatedDeclaration.h"

#include <QWidget>
#include <QPushButton>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QModelIndex>
#include <QPointer>
#include <QMouseEvent>
#include <QDebug>

namespace Ui {
class dashboard;
}

class dashboard : public QWidget
{
    Q_OBJECT

public:
    explicit dashboard(QWidget *parent = nullptr);
    ~dashboard();

private slots:
    void toggleDropDown();
    void on_treeView_clicked(const QModelIndex &index);
    void mousePressEvent(QMouseEvent *event);
    void on_toolButton_12_clicked();
    void on_toolButton_8_clicked();

    void on_toolButton_11_clicked();

public slots:
    void handleGoToPage(QString);

private:
    Ui::dashboard *ui;
    riskManagement *riskmanagement;
    MaximumAmountRegularDeclaration *maximumamountregulardeclaration;
    deliveryManager *deliverymanager;
    SecuritiesRegistrationtop *securitiesregistrationtop;
    AShareDividendDifferentiatedDeclaration *asharedividenddifferentiateddeclaration;
    QPointer<QStandardItemModel> navigation_model;
    QStandardItem * setting_item;
    QStandardItem * setting_item1;
    QStandardItem * setting_item2;
};

#endif // DASHBOARD_H
