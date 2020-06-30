#include "MobileOperatorsModel.h"


MobileOperatorsModel::MobileOperatorsModel(QObject *a_parent)
    :QAbstractItemModel(a_parent)
{
    this->loadFromDb();
}

void MobileOperatorsModel::loadFromDb()
{
    QSqlQuery query;
    Q_ASSERT_X(query.exec(this->sqlQuery()), "MobileOperatorsModel::loadFromDb()", "Error while loading from database");

    QVariant lastMcc;
    Country* lastCountry;
    while (query.next())
    {
        if (lastMcc != query.value(MCC_COLUMN))
        {
            lastCountry = new Country;
            lastMcc = query.value(MCC_COLUMN);
            lastCountry->setCode(query.value(COUNTRY_CODE_COLUMN).toString());
            lastCountry->setMcc (lastMcc.toInt());
            lastCountry->setName(query.value(COUNTRY_NAME_COLUMN).toString());
            m_countries.append(lastCountry);
        }

        if (query.isNull(MNC_COLUMN))
            continue;

        //TODO: move this part to MobileOperatorsModel::addMobileOperator if you need updating from SQL:
        MobileOperator* newOperator = new MobileOperator(*lastCountry);
        newOperator->setMnc(query.value(MNC_COLUMN).toInt());
        newOperator->setName(query.value(OPERATOR_NAME_COLUMN).toString());

    }
}

QString MobileOperatorsModel::sqlQuery()
{
    return QString("SELECT "
                 "countries.mcc AS %1, "
                 "countries.code AS %2, "
                 "countries.name AS %3, "
                 "operators.mnc AS %4, "
                 "operators.name  AS %5 "
             "FROM countries LEFT OUTER JOIN operators "
             "ON operators.mcc = countries.mcc "
             "ORDER BY countries.mcc;")
                .arg(MCC_COLUMN)
                .arg(COUNTRY_CODE_COLUMN)
                .arg(COUNTRY_NAME_COLUMN)
                .arg(MNC_COLUMN)
                .arg(OPERATOR_NAME_COLUMN);
}

MobileOperatorsModel::~MobileOperatorsModel()
{
    qDeleteAll(this->m_countries);
}

MobileOperatorsModel *MobileOperatorsModel::instance()
{
    static MobileOperatorsModel s_model;

    return &s_model;
}

const Country *MobileOperatorsModel::countryByMcc(int a_mcc) const
{
    for (Country* curCountry: m_countries)
    {
        if (curCountry->mcc() == a_mcc)
            return curCountry;
    }

    return nullptr;
}

const MobileOperator *MobileOperatorsModel::mobileOperatorByMccMnc(int a_mcc, int a_mnc) const
{
    const Country* operatorCountry = this->countryByMcc(a_mcc);
    if (!operatorCountry)
        return nullptr;

    return operatorCountry->mobileOperatorByMnc(a_mnc);
}

const Country *MobileOperatorsModel::country(int a_index) const
{
    return m_countries.value(a_index);
}

int MobileOperatorsModel::indexOf(const Country * const a_country) const
{
    for (int i =0; i < m_countries.count(); i++)
    {
        if (m_countries.value(i)->mcc() == a_country->mcc())
            return i;
    }
    return -1;
}

QModelIndex MobileOperatorsModel::modelIndexOf(const Country *a_country) const
{
    int indexInContainer = this->indexOf(a_country);

    if (indexInContainer == -1)
        return {};

    return this->index(indexInContainer, 0, {});
}

QModelIndex MobileOperatorsModel::modelIndexOf(const MobileOperator *a_mobileOperator) const
{
    QModelIndex parentIndex = modelIndexOf(a_mobileOperator->country());
    return this->index(a_mobileOperator->country()->indexOf(a_mobileOperator), 0 , parentIndex);
}

bool MobileOperatorsModel::addMobileOperator(const QString &a_name, const Country& a_country, int a_mnc)
{
    if (a_country.mobileOperatorByMnc(a_mnc)) // if already exist
        return false;

    QSqlQuery query;
    query.prepare("INSERT INTO operators (mcc, mnc, name) VALUES (':mcc', ':mnc', ':name');");

    query.bindValue(":mcc", a_country.mcc());
    query.bindValue(":mnc", a_mnc);
    query.bindValue(":name", a_name);

    if(!query.exec())
    {
        qDebug() << "Can't insert mobile operator to DB. Error:" << query.lastError().text();
        return false;
    }

    this->addMobileOperatorToModel(a_name, a_country, a_mnc);
    return true;
}

void MobileOperatorsModel::addMobileOperatorToModel(const QString &a_name, const Country &a_country, int a_mnc)
{
    QModelIndex parentIndex = this->modelIndexOf(&a_country);

    if (!parentIndex.isValid())
        return;

    int currentIndex = a_country.mobileOperatorsCount();

    this->QAbstractItemModel::beginInsertRows(parentIndex, currentIndex, currentIndex);

    Country *country = this->m_countries.value(this->indexOf(&a_country));
    MobileOperator* newOperator = new MobileOperator(*country);
    newOperator->setName(a_name);
    newOperator->setMnc(a_mnc);

    this->QAbstractItemModel::endInsertRows();
}

bool MobileOperatorsModel::changeMobileOperator(const MobileOperator &a_mobileOperator, const QString &a_name)
{
    QSqlQuery query;
    query.prepare("UPDATE operators SET name = ':name' WHERE mcc  = ':mcc' AND mnc  = ':mnc';");

    query.bindValue(":mcc", a_mobileOperator.country()->mcc());
    query.bindValue(":mnc", a_mobileOperator.mnc());
    query.bindValue(":name", a_name);

    if (query.exec())
    {
        QModelIndex changedIndex = this->modelIndexOf(&a_mobileOperator);
        static_cast<MobileOperator*>(changedIndex.internalPointer())->setName(a_name);
        emit this->dataChanged(changedIndex, changedIndex, {Qt::DisplayRole});
        return true;
    }
    return false;
}

QModelIndex MobileOperatorsModel::index(int a_row, int a_column, const QModelIndex &a_parent) const
{
    if (!hasIndex(a_row, a_column, a_parent))
        return QModelIndex();

    if (a_parent.isValid()) //for mobileOperator
    {
        Country* contry = static_cast<Country*>(a_parent.internalPointer());
        const MobileOperator* childItem = contry->mobileOperator(a_row);

        if (!childItem)
            return {};

        return this->createIndex(a_row, a_column, const_cast<MobileOperator*>(childItem));
    }
    else //for country
        return this->createIndex(a_row, a_column, m_countries.value(a_row));
}

QModelIndex MobileOperatorsModel::parent(const QModelIndex &a_index) const
{
    if (!a_index.isValid())
        return QModelIndex();

    MobileOperatorModelItem *item = static_cast<MobileOperatorModelItem*>(a_index.internalPointer());
    const MobileOperatorModelItem *parent = item->parent();

    if (!parent) // for countries
        return {};

    //for mobile operators:
    const Country* countryParent = static_cast<const Country*>(parent);
    return this->modelIndexOf(countryParent);
}

int MobileOperatorsModel::rowCount(const QModelIndex &a_parent) const
{
    if (!a_parent.isValid())  //root
        return m_countries.count();

    if (a_parent.parent().isValid()) //operator
        return 0;

    Country* country = static_cast<Country*>(a_parent.internalPointer());
    return country->mobileOperatorsCount();
}

int MobileOperatorsModel::columnCount(const QModelIndex &a_parent) const
{
    Q_UNUSED(a_parent)
    return 1;
}

QVariant MobileOperatorsModel::data(const QModelIndex &a_index, int a_role) const
{
    if (!a_index.isValid())
        return QVariant();

    MobileOperatorModelItem *item = static_cast<MobileOperatorModelItem*>(a_index.internalPointer());

    return item->data(a_role);
}
