#ifndef TOOLSLISTMODEL_H
#define TOOLSLISTMODEL_H

#include <QAbstractListModel>
#include <QCoreApplication>
#include <QDir>

class ToolsListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum DataRole {
        AppNameRole,
        AppKeyRole,
        CurrentIndexRole,
        IconRole,
        DesktopFilePath
    };

    explicit ToolsListModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    void setCurrentIndex(QModelIndex idx);

private:
    QStringList m_desktopFilePath;
    QStringList m_list;
    QList<QIcon> m_icon;
    QModelIndex m_currentIndex;
    QMap<QString, QString> m_nameMap;
};

#endif // TOOLSLISTMODEL_H
