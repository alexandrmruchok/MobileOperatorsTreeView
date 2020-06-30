#include "MobileOperatorDialog.h"
#include "ui_MobileOperatorDialog.h"

#include <QDebug>

#include <QFileInfo>
#include <QtSql>
#include "Utils.h"
#include "defines.h"
#include "MobileOperatorsModel.h"

MobileOperatorDialog::MobileOperatorDialog(QWidget *parent /*= nullptr*/)
    : QDialog(parent)
    , ui(new Ui::MobileOperatorDialog)
    , m_stt_mode                 (&m_stateMachine)
    , m_stt_mode_creating        (&m_stt_mode)
    , m_stt_mode_editing         (&m_stt_mode)
    , m_stt_acceptability        (&m_stateMachine)
    , m_stt_acceptability_yes    (&m_stt_acceptability)
    , m_stt_acceptability_no     (&m_stt_acceptability)
    , m_stt_nameValid            (&m_stateMachine)
    , m_stt_nameValid_right      (&m_stt_nameValid)
    , m_stt_nameValid_wrong      (&m_stt_nameValid)
    , m_stt_mccValid             (&m_stateMachine)
    , m_stt_mccValid_right       (&m_stt_mccValid)
    , m_stt_mccValid_wrong       (&m_stt_mccValid)
    , m_stt_mncValid             (&m_stateMachine)
    , m_stt_mncValid_right       (&m_stt_mncValid)
    , m_stt_mncValid_wrong       (&m_stt_mncValid)
{
    ui->setupUi(this);
    this->setupStateMachine();

    QIntValidator *intValidator = new QIntValidator(0, 99999, this);
    ui->lneMcc->setValidator(intValidator);
    ui->lneMnc->setValidator(intValidator);


    connect(ui->lneMcc         , SIGNAL(textChanged(const QString &)), this, SLOT(checkAndUpdateMcc(const QString &)));
    connect(ui->lneMnc         , SIGNAL(textChanged(const QString &)), this, SLOT(checkAndUpdateMccMnc(const QString &)));
    connect(ui->lneName        , SIGNAL(textChanged(const QString &)), this, SLOT(checkName(const QString &)));

    connect(this->ui->pbtSave  , SIGNAL(clicked  ()), this, SLOT(saveToDb()));
    connect(this               , SIGNAL(savedToDb()), this, SLOT(close()));
    connect(this->ui->pbtCancel, SIGNAL(clicked  ()), this, SLOT(close()));
}

void MobileOperatorDialog::setupStateMachine()
{
    m_stateMachine.setChildMode(QState::ParallelStates);
    m_stt_mode_creating.setChildMode(QState::ParallelStates);
    m_stateMachine.setGlobalRestorePolicy(QState::RestorePolicy::RestoreProperties);

    m_stt_mode.setInitialState(&m_stt_mode_creating);
    m_stt_nameValid.setInitialState(&m_stt_nameValid_wrong);
    m_stt_mccValid.setInitialState(&m_stt_mccValid_wrong);
    m_stt_mncValid.setInitialState(&m_stt_mncValid_wrong);
    m_stt_acceptability.setInitialState(&m_stt_acceptability_no);


    m_stt_nameValid_wrong.addTransition(this, &MobileOperatorDialog::nameChangedToFilled, &m_stt_nameValid_right);
    m_stt_nameValid_right.addTransition(this, &MobileOperatorDialog::nameChangedToEmpty, &m_stt_nameValid_wrong);

    m_stt_mccValid_wrong.addTransition(this, &MobileOperatorDialog::mccChangedToRight, &m_stt_mccValid_right);
    m_stt_mccValid_right.addTransition(this, &MobileOperatorDialog::mccChangedToWrong, &m_stt_mccValid_wrong);

    m_stt_mncValid_wrong.addTransition(this, &MobileOperatorDialog::mncChangedToRight, &m_stt_mncValid_right);
    m_stt_mncValid_right.addTransition(this, &MobileOperatorDialog::mncChangedToWrong, &m_stt_mncValid_wrong);


    m_stt_acceptability_yes.addTransition(this, &MobileOperatorDialog::nameChangedToEmpty  , &m_stt_acceptability_no);
    m_stt_acceptability_yes.addTransition(this, &MobileOperatorDialog::mccChangedToWrong   , &m_stt_acceptability_no);
    m_stt_acceptability_yes.addTransition(this, &MobileOperatorDialog::mncChangedToWrong   , &m_stt_acceptability_no);
    m_stt_acceptability_no .addTransition(this, &MobileOperatorDialog::allFieldsBecameRight, &m_stt_acceptability_yes);

    m_stt_mode_editing  .addTransition(this, &MobileOperatorDialog::switchedToCreatingMode, &m_stt_mode_creating);
    m_stt_mode_creating .addTransition(this, &MobileOperatorDialog::switchedToEditingMode , &m_stt_mode_editing );


    connect(&m_stt_mccValid_wrong, &QState::entered, [this]{
        Utils::setPropertyAndUpdateStyle(this->ui->lneMcc, Properties::WRONG, true);
    });
    connect(&m_stt_mccValid_right, &QState::entered, [this]
    {
        Utils::setPropertyAndUpdateStyle(this->ui->lneMcc, Properties::WRONG, false);

        if (m_stt_nameValid_right.active()&& m_stt_mncValid_right.active())
            emit this->allFieldsBecameRight();
    });

    connect(&m_stt_mncValid_wrong, &QState::entered, [this]{
            Utils::setPropertyAndUpdateStyle(this->ui->lneMnc, Properties::WRONG, true);
    });
    connect(&m_stt_mncValid_right, &QState::entered, [this]
    {
        Utils::setPropertyAndUpdateStyle(this->ui->lneMnc, Properties::WRONG, false);

        if (m_stt_nameValid_right.active()&& m_stt_mccValid_right.active())
            emit this->allFieldsBecameRight();
    });

    connect(&m_stt_nameValid_wrong, &QState::entered, [this]
    {
        Utils::setPropertyAndUpdateStyle(this->ui->lneName, Properties::WRONG, true);
    });
    connect(&m_stt_nameValid_right, &QState::entered, [this]
    {
        Utils::setPropertyAndUpdateStyle(this->ui->lneName, Properties::WRONG, false);

        if (m_stt_mccValid_right.active()&& m_stt_mncValid_right.active())
            emit this->allFieldsBecameRight();
    });

    m_stt_acceptability_no.assignProperty(this->ui->pbtSave, qPrintable(Properties::ENABLED), false);
    m_stt_mode_editing    .assignProperty(this->ui->lneMcc , qPrintable(Properties::ENABLED), false);
    m_stt_mode_editing    .assignProperty(this->ui->lneMnc , qPrintable(Properties::ENABLED), false);

    m_stateMachine.start();
}

MobileOperatorDialog::~MobileOperatorDialog()
{
    delete ui;
}

QString MobileOperatorDialog::name()
{
    return this->ui->lneName->text();
}

int MobileOperatorDialog::mnc()
{
    return this->ui->lneMnc->text().toInt();
}

int MobileOperatorDialog::mcc()
{
    return this->ui->lneMcc->text().toInt();
}

void MobileOperatorDialog::editMobileOperator(const MobileOperator &a_mobileOperator /*= nullptr*/)
{
    emit this->switchedToEditingMode();

    this->m_mobileOperator = &a_mobileOperator;

    this->setName(a_mobileOperator.name());
    this->setMcc(a_mobileOperator.country()->mcc());
    this->setMnc(a_mobileOperator.mnc());
    this->open();
}

void MobileOperatorDialog::createMobileOperator()
{
    emit this->switchedToCreatingMode();

    this->m_mobileOperator = nullptr;

    this->setName("");
    this->setMcc(0);
    this->setMnc(0);
    this->open();
}

void MobileOperatorDialog::setName(const QString &a_name)
{
    this->ui->lneName->setText(a_name);
}

void MobileOperatorDialog::setMnc(int a_MNC)
{
    this->ui->lneMnc->setText(QString::number(a_MNC));
}

void MobileOperatorDialog::setMcc(int a_mcc)
{
    this->ui->lneMcc->setText(QString::number(a_mcc));
}

void MobileOperatorDialog::checkName(const QString &a_name)
{
    if (a_name.isEmpty())
        emit this->nameChangedToEmpty();
    else
        emit this->nameChangedToFilled();
}


void MobileOperatorDialog::checkAndUpdateMcc(const QString &a_mcc)
{
    if (a_mcc.isEmpty())
        return setMcc(0);

    else if (a_mcc.size() >=1 && (*a_mcc.begin() == '0'))
         return setMcc(a_mcc.toInt());


    m_country = MobileOperatorsModel::instance()->countryByMcc(this->mcc());
    QString iconPath {};

    if (m_country)
    {
        iconPath = m_country->iconPath();
        emit this->mccChangedToRight();
    }
    else
        emit this->mccChangedToWrong();

    if (iconPath.isEmpty())
        iconPath = MISSING_ICON_PATH;

    this->ui->lblMccIcon->setPixmap(iconPath);

    this->checkAndUpdateMccMnc(this->ui->lneMnc->text());
}

void MobileOperatorDialog::checkAndUpdateMccMnc(const QString &a_mnc)
{
    if (a_mnc.isEmpty())
        return this->setMnc(0);

    else if (a_mnc.size() >=1 && (*a_mnc.begin() == '0'))
        return this->setMnc(a_mnc.toInt());


    if (m_stt_mode_creating.active() && MobileOperatorsModel::instance()->mobileOperatorByMccMnc(this->mcc(), this->mnc()))
        emit this->mncChangedToWrong();
    else
        emit this->mncChangedToRight();

    this->updateOperatorIcon();
}

void MobileOperatorDialog::updateOperatorIcon()
{
    QString filePath {OPERATOR_ICON_PATH};

    filePath = filePath.arg(this->mcc()).arg(this->mnc());

    if (!QFileInfo::exists(filePath))
        filePath = MISSING_ICON_PATH;

    this->ui->lblOperatorIcon->setPixmap(filePath);
}

void MobileOperatorDialog::saveToDb()
{
    bool result = false;

    if (m_mobileOperator)
        result = MobileOperatorsModel::instance()->changeMobileOperator(*m_mobileOperator, this->name());

    else
        result = MobileOperatorsModel::instance()->addMobileOperator(this->name(), *this->m_country, this->mnc());

    if (!result)
        qDebug() << "Error while saving to db!";
    else emit this->savedToDb();
}
