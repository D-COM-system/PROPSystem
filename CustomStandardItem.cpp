#include "CustomStandardItem.h"

CustomStandardItem::CustomStandardItem(const QIcon &leftIcon, const QString &text, const QIcon &rightIcon)
    : QStandardItem(text)
{
    setIcon(leftIcon);
    setData(rightIcon, Qt::UserRole + 1);
}

QIcon CustomStandardItem::rightIcon() const
{
    return data(Qt::UserRole + 1).value<QIcon>();
}

QVariant CustomStandardItem::data(int role) const
{
    if (role == Qt::DecorationRole && !rightIcon().isNull())
    {
        return rightIcon().pixmap(QSize(16, 16)); // 设置右侧图标的大小
    }
    return QStandardItem::data(role);
}
