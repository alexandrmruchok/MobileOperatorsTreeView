#include "MobileOperatorDelegate.h"
#include "qapplication.h"
#include <QtDebug>
#include <QPainter>

#include "MobileOperatorsModel.h"

MobileOperatorDelegate::MobileOperatorDelegate(QObject *a_parent /*= nullptr*/)
    : QStyledItemDelegate(a_parent)
{
}

void MobileOperatorDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);
    QRect itemRect = opt.rect;

    QRect imageRect = itemRect;
    QVariant varImage = index.data(MobileOperatorsModel::Roles::IconPath);
    int imageWidth = 0;
    if (varImage.canConvert<QImage>())
    {
        QImage image = qvariant_cast<QImage>(varImage);
        imageWidth = image.size().width();
        imageRect.setSize(image.size());
        imageRect.translate(0, (itemRect.height() - imageRect.height())/2);
        painter->drawImage(imageRect, image, image.rect());
    }

    opt.rect.setX(opt.rect.x() + imageWidth);

    this->QStyledItemDelegate::paint(painter, opt, index);
}

QSize MobileOperatorDelegate::sizeHint(const QStyleOptionViewItem &a_option, const QModelIndex &a_index) const
{
    QSize size = this->QStyledItemDelegate::sizeHint(a_option, a_index);

    QVariant varImage = a_index.data(MobileOperatorsModel::Roles::IconPath);
    if (varImage.canConvert<QImage>())
    {
        QImage image = qvariant_cast<QImage>(varImage);
        size.setWidth(size.width() + image.width());
    }
    size.setHeight(size.height() * 1.05); // increase in height items

    return size;
}
