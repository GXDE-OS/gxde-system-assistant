#include "toolslistmodel.h"
#include <QLocale>
#include <qt5xdg/xdgdesktopfile.h>
#include <QCoreApplication>
#include <QDir>
//#include <XdgDesktopFile>

ToolsListModel::ToolsListModel(QObject *parent)
    : QAbstractListModel(parent),
      m_currentIndex(QModelIndex())
{
    QStringList readPath;
    readPath << "/usr/share/gxde/gxde-system-assistant/tool-extensions/"
             << QCoreApplication::applicationDirPath() + "/tool-extensions/"
             << QDir::homePath() + "/.config/gxde/gxde-system-assistant/tool-extensions/";
    // << "/usr/share/applications/"
    // << QDir::homePath() + "/.local/share/applications/"
    for (QString j: readPath) {
        QDir desktopPath(j);
        if (!desktopPath.exists()) {
            // 文件夹不存在则直接跳过
            continue;
        }
        QStringList fileList = desktopPath.entryList();
        for (QString i: fileList) {
            XdgDesktopFile desktopFile;
            desktopFile.load(j + i);
            if (!desktopFile.isValid()) {
                // 不可读则直接忽略
                continue;
            }
            // 需要设置 OnlyShowIn 且设置有 gxde-system-assistant
            if(!desktopFile.value("OnlyShowIn").isNull() &&
               !desktopFile.value("OnlyShowIn").toStringList().contains("gxde-system-assistant")){
                // 否则不显示
                continue;
            }
            // 或者 NotShowIn 没有 gxde-system-assistant
            if(!desktopFile.value("NotShowIn").isNull() &&
               desktopFile.value("NotShowIn").toStringList().contains("gxde-system-assistant")){
                // 否则不显示
                continue;
            }
            m_list << desktopFile.value("Exec").toString();
            m_nameMap.insert(desktopFile.value("Exec").toString(),
                             desktopFile.name());
            QIcon icon = desktopFile.icon();
            QString iconStr = desktopFile.iconName();
            m_icon.append(iconStr.isEmpty() ? QIcon(":/resources/sysbro.svg") : icon);
        }
    }
}

int ToolsListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return m_list.size();
}

QVariant ToolsListModel::data(const QModelIndex &index, int role) const
{
    const int row = index.row();

    switch (role) {
    case AppNameRole:
        return m_nameMap.value(m_list.at(row));
    case AppKeyRole:
        return m_list.at(row);
    case CurrentIndexRole:
        return index == m_currentIndex;
    case IconRole:
        return m_icon.at(row);
    }

    return QVariant();
}

void ToolsListModel::setCurrentIndex(QModelIndex idx)
{
    m_currentIndex = idx;

    emit dataChanged(m_currentIndex, m_currentIndex);
}
