#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QWidget>
#include <QPushButton>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QModelIndex>
#include <QPointer>


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

private:
    Ui::dashboard *ui;
    QPointer<QStandardItemModel> navigation_model;
    QStandardItem * setting_item;
    QStandardItem * setting_item1;
    QStandardItem * setting_item2;
};

#endif // DASHBOARD_H
