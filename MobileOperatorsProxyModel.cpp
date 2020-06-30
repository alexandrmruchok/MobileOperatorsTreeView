#include "MobileOperatorsProxyModel.h"

#include "MobileOperatorsModel.h"

MobileOperatorsProxyModel::MobileOperatorsProxyModel(QObject *a_parent)
    :QSortFilterProxyModel(a_parent)
{
    setSourceModel(MobileOperatorsModel::instance());

    this->sort(0, Qt::AscendingOrder);
}

const MobileOperator *MobileOperatorsProxyModel::mobileOperatorAtIndex(QModelIndex a_index)
{
    if (!a_index.parent().isValid()) // Is country
        return nullptr;

    auto sourceIndex = this->mapToSource(a_index);
    return static_cast<MobileOperator*>(sourceIndex.internalPointer());
}

bool MobileOperatorsProxyModel::filterAcceptsRow(int a_source_row, const QModelIndex &a_source_parent) const
{
    if (!a_source_parent.isValid())
    {
        auto country = MobileOperatorsModel::instance()->country(a_source_row);
        if (country->mobileOperatorsCount() == 0)
            return false;
    }

    return this->QSortFilterProxyModel::filterAcceptsRow(a_source_row, a_source_parent);
}
