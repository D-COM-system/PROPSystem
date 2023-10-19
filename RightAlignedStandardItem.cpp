// 自定义的带有右对齐图标的 QStandardItem 子类 RightAlignedStandardItem.h

#include <QStandardItem>

class RightAlignedStandardItem : public QStandardItem
{
public:
    explicit RightAlignedStandardItem(const QIcon& icon, const QString& text)
        : QStandardItem(icon, text)
    {
    }

    QVariant data(int role) const override
    {
        if (role == Qt::DecorationRole) {
            // 返回右对齐的图标
            return QVariant::fromValue(Qt::AlignRight | Qt::AlignVCenter);
        }

        return QStandardItem::data(role);
    }
};
