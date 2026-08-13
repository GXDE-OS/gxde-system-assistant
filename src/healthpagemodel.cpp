#include "healthpagemodel.h"
#include <QDBusConnection>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

HealthPageModel::HealthPageModel() {}

int HealthPageModel::totalTime()
{
    QDBusMessage reply = requestDBus("getBasicStatJson");
    if (reply.type() != QDBusMessage::ReplyMessage || reply.arguments().isEmpty()) {
        return 0;
    }
    QJsonDocument json = QJsonDocument::fromJson(reply.arguments().first().toByteArray());
    if (!json.isObject()) {
        return 0;
    }
    // eg:
    // '{"longestUsedApp":"Microsoft Edge","totalTime":3967}'
    return json.object()["totalTime"].toInt();
}

QString HealthPageModel::longestUsedApp()
{
    QDBusMessage reply = requestDBus("getBasicStatJson");
    if (reply.type() != QDBusMessage::ReplyMessage || reply.arguments().isEmpty()) {
        return QString();
    }
    QJsonDocument json = QJsonDocument::fromJson(reply.arguments().first().toByteArray());
    if (!json.isObject()) {
        return QString();
    }
    // eg:
    // '{"longestUsedApp":"Microsoft Edge","totalTime":3967}'
    return json.object()["longestUsedApp"].toString();
}

QJsonArray HealthPageModel::getPerAppStatJson()
{
    QDBusMessage reply = requestDBus("getPerAppStatJson");
    if (reply.type() != QDBusMessage::ReplyMessage || reply.arguments().isEmpty()) {
        return QJsonArray();
    }
    QByteArray str = reply.arguments().first().toByteArray();
    QJsonDocument json = QJsonDocument::fromJson(str);
    if (!json.isArray()) {
        return QJsonArray();
    }
    return json.array();
}

QDBusMessage HealthPageModel::requestDBus(QString func)
{
    QDBusMessage dbus = QDBusMessage::createMethodCall(DAEMON_DBUS_DESTINATION,
                                                       DAEMON_DBUS_PATH,
                                                       DAEMON_DBUS_INTERFACE,
                                                       func);
    QDBusMessage reply = QDBusConnection::sessionBus().call(dbus);
    // 若守护进程未运行（如非 deepin 环境），返回错误回复，
    // 此时 arguments() 为空，需安全地返回一个空回复避免下层崩溃。
    if (reply.type() != QDBusMessage::ReplyMessage || reply.arguments().isEmpty()) {
        // 返回一个默认（无效）的 QDBusMessage，调用方会因类型判断走安全分支。
        return QDBusMessage();
    }
    return reply;
}
