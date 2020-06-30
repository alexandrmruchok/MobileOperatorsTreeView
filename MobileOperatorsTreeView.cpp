#include "MobileOperatorsTreeView.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QtDebug>
#include <QtSql>
#include <QSqlError>
#include <QStandardItemModel>
#include <QPushButton>
#include <QTableView>
#include <QMouseEvent>
#include <QBoxLayout>
#include <QSizePolicy>

#include "MobileOperatorDelegate.h"
#include "MobileOperatorsProxyModel.h"
#include "MobileOperatorsModel.h"
#include "Country.h"

MobileOperatorsTreeView::MobileOperatorsTreeView(QWidget *a_parent)
    : QTreeView(a_parent)
    , m_pbtOperator(this)
    , m_pbtAddOperator(this)
    , m_operatorDialog(this)
{
    m_pbtOperator.setObjectName("pbtOperatorButton");
    m_pbtOperator.setCursor(Qt::PointingHandCursor);
    m_pbtOperator.hide();

    Q_ASSERT(createDBConnection());

    this->setObjectName("trvMobileOperators");
    this->setSelectionMode(QAbstractItemView::NoSelection);
    this->setFocusPolicy(Qt::FocusPolicy::NoFocus);
    this->setHeaderHidden(true);
    this->setMouseTracking(true);
    this->setItemDelegate(new MobileOperatorDelegate);
    this->setModel(new MobileOperatorsProxyModel);

    this->setupAddOperatorButton();

    connect(this, &QTreeView::doubleClicked, [this]()
    {
        if (m_currentOperatorIndex.isValid())
            this->m_operatorDialog.editMobileOperator(*this->currentMobileOperator());
    });

    connect(&m_pbtOperator, &QPushButton::clicked, [this]()
    {
        auto currentOprerator = this->currentMobileOperator();
        this->doSomethingWithMobileOperator(currentOprerator->country()->mcc(), currentOprerator->mnc());
    });
}

void MobileOperatorsTreeView::setupAddOperatorButton()
{
    m_pbtAddOperator.setObjectName("pbtAddMobileOperator");
    m_pbtAddOperator.setCursor(Qt::PointingHandCursor);
    QVBoxLayout *verLayout = new QVBoxLayout(this);
    verLayout->setMargin(0);

    verLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding));

    QFrame* frmAddButtonWrapper = new QFrame;
    frmAddButtonWrapper->setObjectName("frmAddButtonWrapper");
    verLayout->addWidget(frmAddButtonWrapper, 0, Qt::AlignRight);


    frmAddButtonWrapper->setLayout(new QHBoxLayout);
    frmAddButtonWrapper->layout()->setMargin(0);
    frmAddButtonWrapper->layout()->addWidget(&m_pbtAddOperator);

    connect(&m_pbtAddOperator, &QPushButton::clicked, [this] {
        m_operatorDialog.createMobileOperator();
    });
}

void MobileOperatorsTreeView::doSomethingWithMobileOperator(int mcc, int mnc)
{
    qDebug()<<"Some action with operator " << mcc << mnc;
    //TODO: ???
}

bool MobileOperatorsTreeView::createDBConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("OM system.db");

    if (!db.open())
    {
        qDebug()<< "Cannot open database";
        return false;
    }
    qDebug()<< "Database opened";
    return  true;
}

void MobileOperatorsTreeView::mouseMoveEvent(QMouseEvent *event)
{
    this->updateCurrentOperator(event->pos());

    QTreeView::mouseMoveEvent(event);
}


void MobileOperatorsTreeView::paintEvent(QPaintEvent *event)
{
    this->updateCurrentOperator(this->mapFromGlobal(QCursor::pos()));

    this->QTreeView::paintEvent(event);
}

void MobileOperatorsTreeView::updateCurrentOperator(QPoint a_cursorPos)
{
    QModelIndex index = this->indexAt(a_cursorPos);
    QRect operatorRect;

    if (!index.parent().isValid())
        index = {};

    if (index.isValid())
    {
        operatorRect = this->visualRect(index);
        operatorRect.setWidth(this->sizeHintForIndex(index).width());

        if (m_currentOperatorIndex == index)
            operatorRect.setWidth(operatorRect.width() + m_pbtOperator.width());

        if (!operatorRect.contains(a_cursorPos))
            index = {};
    }

    if (m_currentOperatorIndex == index)
        return;
    m_currentOperatorIndex = index;

    if (!index.isValid())
        return m_pbtOperator.hide();
    m_currentOperatorIndex = index;

    int buttonX = operatorRect.right();
    int buttonY = operatorRect.top() + (operatorRect.height() - m_pbtOperator.height())/2; // Center
    m_pbtOperator.move(buttonX, buttonY);
    m_pbtOperator.show();
}

const MobileOperator *MobileOperatorsTreeView::currentMobileOperator() const
{
    MobileOperatorsProxyModel* proxyModel = qobject_cast<MobileOperatorsProxyModel*>(this->model());
    if (!proxyModel)
        return nullptr;

    return proxyModel->mobileOperatorAtIndex(m_currentOperatorIndex);
}
