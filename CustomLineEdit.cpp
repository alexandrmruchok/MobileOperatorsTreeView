#include "CustomLineEdit.h"

#include <QHBoxLayout>
#include <QPushButton>

CustomLineEdit::CustomLineEdit(QWidget *a_parent /*= nullptr*/)
    : QLineEdit(a_parent)
    , m_button(new QPushButton)
{
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setMargin(0);
    layout->addItem(new QSpacerItem(0,0, QSizePolicy::Expanding, QSizePolicy::Expanding));

    layout->addWidget(m_button);

    m_button->setObjectName("pbtClear");
    m_button->setCursor(Qt::ArrowCursor);

    connect(m_button, &QPushButton::clicked, this, &QLineEdit::clear);
    connect(this, &QLineEdit::textChanged, [this](const QString &a_text)
    {
        this->m_button->setVisible(!a_text.isEmpty());
    });
}
