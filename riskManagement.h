#ifndef RISKMANAGEMENT_H
#define RISKMANAGEMENT_H

#include <QWidget>
#include <QPushButton>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QModelIndex>
#include <QPointer>
#include <QDebug>

namespace Ui {
class riskManagement;
}

class riskManagement : public QWidget
{
    Q_OBJECT

public:
    explicit riskManagement(QWidget *parent = nullptr);
    ~riskManagement();



private slots:
    void on_treeView_clicked(const QModelIndex &index);

public:
    Ui::riskManagement *ui;
    QPointer<QStandardItemModel> navigation_model;
    QStandardItem * setting_item;
    QStandardItem * setting_item1;
    QStandardItem * setting_item2;
    QStandardItem * setting_item3;
    QStandardItem * setting_item4;
    QStandardItem * setting_item5;

signals:
    void goToPage(QString);
};

#endif // RISKMANAGEMENT_H
