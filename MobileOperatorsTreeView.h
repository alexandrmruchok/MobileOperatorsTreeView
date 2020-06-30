#ifndef MOBILEOPERATORSTREEVIEW_H
#define MOBILEOPERATORSTREEVIEW_H

#include <QTreeView>
#include <QDebug>
#include <QPushButton>
#include <QStandardItem>
#include "MobileOperatorDialog.h"
#include "MobileOperator.h"


class MobileOperatorsTreeView : public QTreeView
{
    Q_OBJECT
public:
    MobileOperatorsTreeView(QWidget *a_parent = nullptr);

public slots:
    void doSomethingWithMobileOperator(int mcc, int mnc);

protected:
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;

private:
    const int ID = Qt::UserRole;

    bool createDBConnection();
    void setupAddOperatorButton();
    void updateCurrentOperator(QPoint a_cursorPos);
    const MobileOperator* currentMobileOperator() const;

    QPushButton m_pbtOperator;
    QPushButton m_pbtAddOperator;
    QModelIndex m_currentOperatorIndex;
    QMap<int, QStandardItem*> m_countryItems;
    MobileOperatorDialog m_operatorDialog;
};


#endif // MOBILEOPERATORSTREEVIEW_H
