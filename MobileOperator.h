#ifndef MOBILEOPERATOR_H
#define MOBILEOPERATOR_H

#include <QObject>
#include "MobileOperatorModelItem.h"

class Country;

class MobileOperator : public MobileOperatorModelItem
{
public:
    MobileOperator(Country &a_country);

    int mnc() const;
    void setMnc(int mnc);

    const Country *country() const;
    void setCountry(const Country *a_country);

    QString iconPath() const override;

private:
    int m_mnc;
    const Country* m_country;

    // MobileOperatorModelItem interface
public:
    virtual QVariant data(int a_role) override;

private:
    virtual const MobileOperatorModelItem *parent() override;
};


#endif // MOBILEOPERATOR_H
