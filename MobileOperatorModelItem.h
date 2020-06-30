#ifndef MOBILEOPERATOMODELITEM_H
#define MOBILEOPERATOMODELITEM_H

#include <QVariant>

class MobileOperatorModelItem
{
public:
    QString name() const;
    void setName(const QString &name);

    virtual QString iconPath() const = 0;

    virtual const MobileOperatorModelItem* parent();
    virtual QVariant data(int a_role = Qt::DisplayRole);
private:

    QString m_name;
};

#endif // MOBILEOPERATOMODELITEM_H
