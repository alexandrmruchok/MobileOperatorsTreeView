#include "Country.h"
#include "MobileOperatorsModel.h"


Country::~Country()
{
    qDeleteAll(m_mobileOperators);
}

int Country::mcc() const
{
    return m_mcc;
}

void Country::setMcc(int mcc)
{
    m_mcc = mcc;
}

QString Country::code() const
{
    return m_code;
}

void Country::setCode(const QString &code)
{
    m_code = code;
}

void Country::addMobileOperator(MobileOperator &a_operator)
{
    m_mobileOperators.push_back(&a_operator);
}

int Country::mobileOperatorsCount() const
{
    return m_mobileOperators.count();
}

const MobileOperator *Country::mobileOperator(int a_index) const
{
    return m_mobileOperators.value(a_index);
}

const MobileOperator *Country::mobileOperatorByMnc(int a_mnc) const
{
    for (auto cur_operator : m_mobileOperators)
    {
        if (cur_operator->mnc() == a_mnc)
            return cur_operator;
    }

    return nullptr;
}

int Country::indexOf(const MobileOperator *a_mobileOperator) const
{
    return m_mobileOperators.indexOf(const_cast<MobileOperator*>(a_mobileOperator));
}

QString Country::iconPath() const
{
    if (this->code().isEmpty())
        return "";

    return QString(":/icons/Countries/%1.png").arg(this->code());
}

QVariant Country::data(int a_role)
{
    if (a_role == MobileOperatorsModel::Roles::Id)
        return this->mcc();
    else if (a_role == Qt::FontRole)
    {
        QFont font;
        font.setBold(true);
        return font;
    }

    return this->MobileOperatorModelItem::data(a_role);
}
