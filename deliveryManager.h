#ifndef DELIVERYMANAGER_H
#define DELIVERYMANAGER_H

#include <QWidget>
#include <QPushButton>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QModelIndex>
#include <QPointer>
#include <QDebug>

namespace Ui {
class deliveryManager;
}

class deliveryManager : public QWidget
{
    Q_OBJECT

public:
    explicit deliveryManager(QWidget *parent = nullptr);
    ~deliveryManager();

private slots:
    void on_treeView_clicked(const QModelIndex &index);

public:
    Ui::deliveryManager *ui;
    QPointer<QStandardItemModel> navigation_model;
    QStandardItem * setting_item;
    QStandardItem * setting_item1;
    QStandardItem * setting_item2;
    QStandardItem * setting_item3;
    QStandardItem * setting_item4;

signals:
    void goToPage(QString);

};

#endif // DELIVERYMANAGER_H
