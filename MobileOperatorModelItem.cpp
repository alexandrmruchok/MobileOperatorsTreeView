#include "MobileOperatorModelItem.h"
#include <QPixmap>

#include "MobileOperatorsModel.h"

QString MobileOperatorModelItem::name() const
{
    return m_name;
}

void MobileOperatorModelItem::setName(const QString &name)
{
    m_name = name;
}

///Default realisation
const MobileOperatorModelItem *MobileOperatorModelItem::parent()
{
    return nullptr;
}

QVariant MobileOperatorModelItem::data(int a_role)
{
    if (a_role == Qt::DisplayRole)
        return this->name();

    else if (a_role == MobileOperatorsModel::IconPath)
        return QImage(this->iconPath());

    return {};
}
