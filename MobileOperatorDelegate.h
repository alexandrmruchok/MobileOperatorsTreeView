#ifndef MOBILEOPERATORDELEGATE_H
#define MOBILEOPERATORDELEGATE_H

#include <QStyledItemDelegate>

class MobileOperatorDelegate : public QStyledItemDelegate
{
public:
    MobileOperatorDelegate(QObject *a_parent = nullptr);

    // QAbstractItemDelegate interface
public:
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};





#endif // MOBILEOPERATORDELEGATE_H
