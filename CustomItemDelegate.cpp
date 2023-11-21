#include <QStyledItemDelegate>
#include <QPainter>
#include <QStandardItemModel>
#include <QTreeView>

class CustomItemDelegate : public QStyledItemDelegate
{
public:
    explicit CustomItemDelegate(QObject* parent = nullptr)
        : QStyledItemDelegate(parent)
    {
    }

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override
    {
        QStyledItemDelegate::paint(painter, option, index);

        if (index.column() == 0)
        {
            // 获取数据
            QString text = index.data(Qt::DisplayRole).toString();
            QIcon icon1 = index.data(Qt::DecorationRole).value<QIcon>();
            QIcon icon2 = index.data(Qt::UserRole + 1).value<QIcon>();

            // 绘制图标和文本
            QRect icon1Rect = option.rect.adjusted(0, 0, -option.rect.width() / 2, 0);
            QRect icon2Rect = option.rect.adjusted(option.rect.width() / 2, 0, 0, 0);
            QRect textRect = option.rect.adjusted(option.rect.width() / 2, 0, 0, 0);

            if (!icon1.isNull())
            {
                icon1.paint(painter, icon1Rect, Qt::AlignLeft | Qt::AlignVCenter, QIcon::Normal, QIcon::On);
            }

            if (!icon2.isNull())
            {
                icon2.paint(painter, icon2Rect, Qt::AlignLeft | Qt::AlignVCenter, QIcon::Normal, QIcon::On);
            }

            painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, text);
        }
    }

    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override
    {
        QSize size = QStyledItemDelegate::sizeHint(option, index);
        size.setWidth(size.width() * 2);
        return size;
    }
};
