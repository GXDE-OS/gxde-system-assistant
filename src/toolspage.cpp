#include "toolspage.h"
#include "toolsitemdelegate.h"
#include <QProcess>
#include <QVBoxLayout>
#include <QDebug>
#include <QMenu>
#include <QStandardPaths>

ToolsPage::ToolsPage(QWidget *parent)
    : QWidget(parent),
      m_toolsView(new ToolsView),
      m_listModel(new ToolsListModel)
{
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(m_toolsView);
    layout->setMargin(20);

    // m_toolsView->setViewMode(QListView::IconMode);
    m_toolsView->setItemDelegate(new ToolsItemDelegate);
    // m_toolsView->setMouseTracking(true);
    // m_toolsView->setDragEnabled(false);
    // m_toolsView->setAutoScroll(false);
    m_toolsView->setModel(m_listModel);
    // m_toolsView->setWrapping(true);

    setLayout(layout);
    m_toolsView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_toolsView, &QListView::entered, m_listModel, &ToolsListModel::setCurrentIndex);
    connect(m_toolsView, &QListView::clicked, this, &ToolsPage::handleViewClicked);
    connect(m_toolsView, &QWidget::customContextMenuRequested, this, &ToolsPage::showMenu);
}

void ToolsPage::showMenu(QPoint point)
{
    m_menuMousePoint = QCursor::pos();
    QMenu menu(this);
    QAction open(tr("Open"));
    QAction sendToDesktop(tr("Send to desktop"));

    menu.addAction(&open);
    menu.addSeparator();
    menu.addAction(&sendToDesktop);

    connect(&open, &QAction::triggered, this, [this, point](){
        handleViewClicked(m_toolsView->indexAt(point));
    });
    connect(&sendToDesktop, &QAction::triggered, this, [this, point](){
        sendFileToDesktop(m_toolsView->indexAt(point));
    });

    menu.exec(m_menuMousePoint);
}

void ToolsPage::leaveEvent(QEvent *e)
{
    QWidget::leaveEvent(e);

    m_listModel->setCurrentIndex(QModelIndex());
}

bool ToolsPage::sendFileToDesktop(QModelIndex idx)
{
    QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString desktopFilePath = idx.data(ToolsListModel::DesktopFilePath).toString();
    QFileInfo desktopFileInfo(desktopFilePath);
    qDebug() << desktopPath + desktopFileInfo.fileName();
    return QFile::copy(desktopFilePath, desktopPath + "/" + desktopFileInfo.fileName());
}

/*void ToolsPage::handleViewClicked(QModelIndex idx)
{
    // startup application.
    QString appExec = idx.data(ToolsListModel::AppKeyRole).toString();
    qDebug() << appExec;
    QProcess::startDetached(appExec);
}*/

void ToolsPage::handleViewClicked(QModelIndex idx) 
{
    //修复打开从工具箱打开的应用UI显示异常的问题
    // startup application.
    QString appExec = idx.data(ToolsListModel::AppKeyRole).toString();
    qDebug() << appExec;
    if (appExec.isEmpty())
        return;

    QStringList parts = QProcess::splitCommand(appExec);
    if (parts.isEmpty())
        return;
    QString program = parts.takeFirst();
    QStringList args = parts;

    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.insert("QT_QPA_PLATFORM", "dxcb;xcb;dwayland");

    QProcess process;
    process.setProgram(program);
    process.setArguments(args);
    process.setProcessEnvironment(env);
    
    qint64 pid;
    if (!process.startDetached(&pid)) {
        qWarning() << "Failed to start:" << program;
    }
}
