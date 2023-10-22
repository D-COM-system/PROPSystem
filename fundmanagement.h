#ifndef FUNDMANAGEMENT_H
#define FUNDMANAGEMENT_H

#include <QWidget>
#include <QPushButton>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QModelIndex>
#include <QPointer>
#include <QDebug>

namespace Ui {
class FundManagement;
}

class FundManagement : public QWidget
{
    Q_OBJECT

public:
    explicit FundManagement(QWidget *parent = nullptr);
    ~FundManagement();

private slots:
    void on_treeView_clicked(const QModelIndex &index);

public:
    Ui::FundManagement *ui;
    QPointer<QStandardItemModel> navigation_model;
    QStandardItem * setting_item;
    QStandardItem * setting_item1;
    QStandardItem * setting_item2;
    QStandardItem * setting_item3;
    QStandardItem * setting_item4;
    QStandardItem * setting_item5;
    QStandardItem * setting_item6;
    QStandardItem * setting_item7;
    QStandardItem * setting_item8;
    QStandardItem * setting_item9;
    QStandardItem * setting_item10;

signals:
    void goToPage(QString);

};

#endif // FUNDMANAGEMENT_H
