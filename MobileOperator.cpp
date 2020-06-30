#include "MobileOperator.h"

#include <QDebug>
#include "Country.h"
#include "MobileOperatorsModel.h"

MobileOperator::MobileOperator(Country &a_country)
    : MobileOperatorModelItem()
    , m_country(&a_country)
{
    a_country.addMobileOperator(*this);
}

int MobileOperator::mnc() const
{
    return m_mnc;
}

void MobileOperator::setMnc(int mnc)
{
    m_mnc = mnc;
}

const Country *MobileOperator::country() const
{
    return m_country;
}

void MobileOperator::setCountry(const Country *a_country)
{
    m_country = a_country;
}

QString MobileOperator::iconPath() const
{
    return QString(":/icons/Operators/%1_%2.png").arg(this->country()->mcc()).arg(this->mnc());
}

const MobileOperatorModelItem *MobileOperator::parent()
{
    return this->country();
}

QVariant MobileOperator::data(int a_role)
{
    if (a_role == MobileOperatorsModel::Roles::Id)
        return this->mnc();

    else if (a_role == Qt::DisplayRole)
        return QString("%1 (%2, %3) ").arg(this->name()).arg(this->country()->mcc()).arg(this->mnc());

    return this->MobileOperatorModelItem::data(a_role);
}
