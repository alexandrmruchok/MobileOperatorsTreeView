#include <QApplication>
#include "MobileOperatorsTreeView.h"

#include <QFileInfo>

void setStyleSheet(QApplication& a_app);

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MobileOperatorsTreeView operatorsView;
    operatorsView.show();

    ::setStyleSheet(app);

    return app.exec();
}

void setStyleSheet(QApplication& a_app)
{
    QString filePath {":/appStylesheet.css"};
    if (!QFileInfo::exists(filePath)) {
        QString errorMsg("File " + filePath + " not exists");
        qCritical() << errorMsg;
        throw errorMsg;
    }
    QFile styleSheetFile(filePath);

    if (!styleSheetFile.open(QFile::ReadOnly))
    {
        QString errorMsg("Can't load style sheet. " + styleSheetFile.errorString());
        qCritical() << errorMsg;
        throw errorMsg;
    }

    QString styleSheet(styleSheetFile.readAll());
    styleSheetFile.close();

    a_app.setStyleSheet(styleSheet);
}
