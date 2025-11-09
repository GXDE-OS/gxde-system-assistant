#ifndef HEALTHPAGEMODEL_H
#define HEALTHPAGEMODEL_H

#include <QJsonArray>
#include <QDBusMessage>

#define DAEMON_DBUS_DESTINATION "org.deepin.dde.digitalWellbeing"
#define DAEMON_DBUS_PATH "/org/deepin/dde/digitalWellbeing"
#define DAEMON_DBUS_INTERFACE "org.deepin.dde.digitalWellbeing"

class HealthPageModel
{
public:
    HealthPageModel();

    static int totalTime();
    static QString longestUsedApp();
    static QJsonArray getPerAppStatJson();

private:
    static QDBusMessage requestDBus(QString func);
};

#endif // HEALTHPAGEMODEL_H
