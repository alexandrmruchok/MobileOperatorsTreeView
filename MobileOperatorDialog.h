#ifndef EDITOPERATORDIALOG_H
#define EDITOPERATORDIALOG_H

#include <QDialog>
#include <QStateMachine>
#include <QMap>

class Country;

class MobileOperator;

namespace Ui {
class MobileOperatorDialog;
}

class MobileOperatorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MobileOperatorDialog(QWidget *parent = nullptr);
    ~MobileOperatorDialog();

    QString name();
    int mnc();
    int mcc();

signals:
    void mccChangedToRight();
    void mccChangedToWrong();

    void nameChangedToFilled();
    void nameChangedToEmpty();

    void mncChangedToRight();
    void mncChangedToWrong();

    void allFieldsBecameRight();

    void switchedToCreatingMode();
    void switchedToEditingMode();

    void savedToDb();

public slots:

    void editMobileOperator(const MobileOperator &a_mobileOperator);
    void createMobileOperator();

    void setName(const QString& a_name);
    void setMnc(int a_MNC);
    void setMcc(int a_mcc);

private slots:
    void checkName(const QString &a_name);
    void checkAndUpdateMcc(const QString &a_mcc);
    void checkAndUpdateMccMnc(const QString &a_mnc);
    void updateOperatorIcon();

    void saveToDb();

private:
    void setupStateMachine();

    Ui::MobileOperatorDialog *ui;

    QStateMachine m_stateMachine;

    QState m_stt_mode;
    QState m_stt_mode_creating;
    QState m_stt_mode_editing;

    QState m_stt_acceptability;
    QState m_stt_acceptability_yes;
    QState m_stt_acceptability_no;

    QState m_stt_nameValid;
    QState m_stt_nameValid_right;
    QState m_stt_nameValid_wrong;

    QState m_stt_mccValid;
    QState m_stt_mccValid_right;
    QState m_stt_mccValid_wrong;

    QState m_stt_mncValid;
    QState m_stt_mncValid_right;
    QState m_stt_mncValid_wrong;

    const MobileOperator* m_mobileOperator;
    const Country* m_country;

    const QString MISSING_ICON_PATH {":/icons/question.png"};
    const QString OPERATOR_ICON_PATH {":/icons/Operators/%1_%2.png"};
};

#endif // EDITOPERATORDIALOG_H
