#ifndef TOOLSPAGE_H
#define TOOLSPAGE_H

#include <QWidget>
#include "toolsview.h"
#include "toolslistmodel.h"

class ToolsPage : public QWidget
{
    Q_OBJECT

public:
    explicit ToolsPage(QWidget *parent = nullptr);

    void leaveEvent(QEvent *);

private:
    void handleViewClicked(QModelIndex idx);
    bool sendFileToDesktop(QModelIndex idx);
    void showMenu(QPoint point);

private:
    ToolsView *m_toolsView;
    QPoint m_menuMousePoint;
    ToolsListModel *m_listModel;

};

#endif // TOOLSPAGE_H
