#include "Utils.h"

namespace Utils
{
    void setPropertyAndUpdateStyle(QWidget *a_widget, const QString &a_property, const QVariant &a_value /*= true*/)
    {
        QByteArray l_tempStr = a_property.toLatin1();
        const char* l_property = l_tempStr.constData();
        if (a_widget->property(l_property) == a_value)
            return;
        a_widget->setProperty(l_property, a_value);

        Utils::updateStyle(a_widget);
    }

    void updateStyle(QWidget *a_widget)
    {
        a_widget->style()->unpolish(a_widget);
        a_widget->style()->polish(a_widget);
    }
}
