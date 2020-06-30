#ifndef MOBILEOPERATORSMODEL_H
#define MOBILEOPERATORSMODEL_H

#include <QMap>
#include <QAbstractItemModel>
#include <QtSql>


#include "Country.h"
#include "MobileOperator.h"

class MobileOperatorsModel : public QAbstractItemModel
{
    MobileOperatorsModel(QObject *a_parent = nullptr); ///Is private becouse MobileOperatorsModel is a singleton

public:
    ~MobileOperatorsModel();

    enum Roles{
        Id = Qt::UserRole,
        IconPath
    };
    static MobileOperatorsModel* instance();

    const Country* countryByMcc(int a_mcc) const;
    const MobileOperator *mobileOperatorByMccMnc(int a_mcc, int a_mnc) const;
    const Country* country(int a_index) const;

    int indexOf(const Country * const a_country) const;
    QModelIndex modelIndexOf(const Country * const a_country) const;
    QModelIndex modelIndexOf(const MobileOperator *a_mobileOperator) const;

    bool addMobileOperator(const QString& a_name, const Country &a_country, int a_mnc);
    bool changeMobileOperator(const MobileOperator &a_mobileOperator, const QString& a_name);

    // QAbstractItemModel interface:
public:
    virtual QModelIndex index(int a_row, int a_column, const QModelIndex &a_parent) const override;
    virtual QModelIndex parent(const QModelIndex &a_index) const override;
    virtual int rowCount(const QModelIndex &a_parent) const override;
    virtual int columnCount(const QModelIndex &a_parent) const override;
    virtual QVariant data(const QModelIndex &a_index, int a_role) const override;

private:
    void addMobileOperatorToModel(const QString& a_name, const Country &a_country, int a_mnc);

    void loadFromDb();
    QString sqlQuery();
    QVector<Country*> m_countries;

    const QString MCC_COLUMN {"mcc"};
    const QString COUNTRY_CODE_COLUMN {"countryCode"};
    const QString COUNTRY_NAME_COLUMN {"country"};
    const QString MNC_COLUMN {"mnc"};
    const QString OPERATOR_NAME_COLUMN {"operatorName"};
};


#endif // MOBILEOPERATORSMODEL_H
