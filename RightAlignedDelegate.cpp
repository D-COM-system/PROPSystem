// 自定义代理类 RightAlignedDelegate.h

#include <QStyledItemDelegate>
#include <QPainter>
#include <QStyleOptionViewItem>

class RightAlignedDelegate : public QStyledItemDelegate
{
public:
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override
    {
        QStyleOptionViewItem opt = option;
        initStyleOption(&opt, index);

        // 绘制文本
        QStyledItemDelegate::paint(painter, opt, index);

        // 绘制展开/收起的小图标
        if (index.parent().isValid() && opt.state & QStyle::State_Open) {
            QRect decorationRect = opt.rect;
            decorationRect.setLeft(decorationRect.right() - opt.decorationSize.width());

            QIcon::Mode mode = opt.state & QStyle::State_Selected ? QIcon::Selected : QIcon::Normal;
            QIcon::State state = QIcon::On;

            opt.icon.paint(painter, decorationRect, opt.decorationAlignment, mode, state);
        }
    }
};
