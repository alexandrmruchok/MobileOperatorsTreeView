#ifndef UTILS_H
#define UTILS_H

#include <QWidget>
#include <QVariant>
#include <QStyle>

namespace Utils {

    void setPropertyAndUpdateStyle(QWidget *a_widget, const QString &a_property, const QVariant &a_value /*= true*/);
    void updateStyle(QWidget *a_widget);
}

#endif // UTILS_H
