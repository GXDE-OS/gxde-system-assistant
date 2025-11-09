#include "healthpagemodel.h"
#include <QDBusConnection>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

HealthPageModel::HealthPageModel() {}

int HealthPageModel::totalTime()
{
    auto json = QJsonDocument::fromJson(requestDBus("getBasicStatJson").arguments().first().toByteArray());
    // eg:
    // '{"longestUsedApp":"Microsoft Edge","totalTime":3967}'
    return json.object()["totalTime"].toInt();
}

QString HealthPageModel::longestUsedApp()
{
    auto json = QJsonDocument::fromJson(requestDBus("getBasicStatJson").arguments().first().toByteArray());
    // eg:
    // '{"longestUsedApp":"Microsoft Edge","totalTime":3967}'
    return json.object()["longestUsedApp"].toString();
}

QJsonArray HealthPageModel::getPerAppStatJson()
{
    QByteArray str = requestDBus("getPerAppStatJson").arguments().first().toByteArray();
    return QJsonDocument::fromJson(str).array();
}

QDBusMessage HealthPageModel::requestDBus(QString func)
{
    QDBusMessage dbus = QDBusMessage::createMethodCall(DAEMON_DBUS_DESTINATION,
                                                       DAEMON_DBUS_PATH,
                                                       DAEMON_DBUS_INTERFACE,
                                                       func);
    return QDBusConnection::sessionBus().call(dbus);
}
