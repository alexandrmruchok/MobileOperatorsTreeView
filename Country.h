#ifndef COUNTRY_H
#define COUNTRY_H

#include <QString>
#include <QVector>
#include "MobileOperator.h"

class Country : public MobileOperatorModelItem
{
public:
    ~Country();

    int mcc() const;
    void setMcc(int mcc);

    QString code() const;
    void setCode(const QString &code);

    void addMobileOperator(MobileOperator &a_operator);
    int mobileOperatorsCount() const;
    const MobileOperator* mobileOperator(int a_index) const;
    const MobileOperator* mobileOperatorByMnc(int a_mnc) const;
    int indexOf(const MobileOperator *a_mobileOperator) const;

    virtual QString iconPath() const override;
    virtual QVariant data(int a_role) override;

private:
    int m_mcc;
    QString m_code;

    QVector<MobileOperator*> m_mobileOperators;
};

#endif // COUNTRY_H
