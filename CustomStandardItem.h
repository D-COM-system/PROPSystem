#ifndef CUSTOMSTANDARDITEM_H
#define CUSTOMSTANDARDITEM_H

#include <QStandardItem>

class CustomStandardItem : public QStandardItem
{
public:
    CustomStandardItem(const QIcon &leftIcon, const QString &text, const QIcon &rightIcon);

    QIcon rightIcon() const;

    QVariant data(int role) const override;
};

#endif // CUSTOMSTANDARDITEM_H
