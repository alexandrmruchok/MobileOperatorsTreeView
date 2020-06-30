#ifndef CUSTOMLINEEDIT_H
#define CUSTOMLINEEDIT_H

#include <QLineEdit>
#include <QPushButton>

class CustomLineEdit : public QLineEdit
{
public:
    CustomLineEdit(QWidget *a_parent = nullptr);

private:
    QPushButton* m_button;
};

#endif // CUSTOMLINEEDIT_H
