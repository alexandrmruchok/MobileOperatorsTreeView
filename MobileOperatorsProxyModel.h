#ifndef MOBILEOPERATORSPROXYMODEL_H
#define MOBILEOPERATORSPROXYMODEL_H

#include <QSortFilterProxyModel>

class MobileOperator;

//ProxyModel is needed for hiding countries without mobile operators
class MobileOperatorsProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    MobileOperatorsProxyModel(QObject *a_parent = nullptr);

    const MobileOperator *mobileOperatorAtIndex(QModelIndex a_index);

    // QSortFilterProxyModel interface
protected:
    virtual bool filterAcceptsRow(int a_source_row, const QModelIndex &a_source_parent) const override;
};



#endif // MOBILEOPERATORSPROXYMODEL_H
