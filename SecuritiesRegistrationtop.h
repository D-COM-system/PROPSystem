#ifndef SECURITIESREGISTRATIONTOP_H
#define SECURITIESREGISTRATIONTOP_H

#include <QWidget>
#include <QPushButton>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QModelIndex>
#include <QPointer>
#include <QDebug>

namespace Ui {
class SecuritiesRegistrationtop;
}

class SecuritiesRegistrationtop : public QWidget
{
    Q_OBJECT

public:
    explicit SecuritiesRegistrationtop(QWidget *parent = nullptr);
    ~SecuritiesRegistrationtop();

private slots:
    void on_treeView_clicked(const QModelIndex &index);

public:
    Ui::SecuritiesRegistrationtop *ui;
    QPointer<QStandardItemModel> navigation_model;
    QStandardItem * setting_item;
    QStandardItem * setting_item1;

signals:
    void goToPage(QString);
};

#endif // SECURITIESREGISTRATIONTOP_H
