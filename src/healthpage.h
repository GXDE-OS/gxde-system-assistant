#ifndef HEALTHPAGE_H
#define HEALTHPAGE_H

#include <QWidget>
#include <QLabel>
#include <QChartView>
#include <QPieSeries>
#include <QListWidget>
#include <QGridLayout>
#include <QScrollArea>

// Qt6 中 QtCharts 的类不再位于 QtCharts 命名空间下
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
namespace QtCharts {}
#endif
using namespace QtCharts;

class HealthPage : public QScrollArea
{
    Q_OBJECT
public:
    explicit HealthPage(QWidget *parent = nullptr);

private:
    void refresh();
    void refreshCharts();
    void initChart();
    void onPieSeriesClicked(QPieSlice *slice);
    QString secondToTimeText(int second);

    QWidget *m_mainWidget;
    QLabel *m_totalTime;
    QLabel *m_longestUsedApp;
    QLabel *m_appInfo;
    QGridLayout *m_appUsedTimeView;
    QChartView *m_timeChartView;
    QPieSeries *m_timePieSeries;
    QList<QPieSlice *> m_timePieSliceList;
    //QList<QListWidgetItem *> m_appUsedTimeViewItemList;
    QList<QLabel *> m_appUsedTimeViewItemList;
    QList<QLabel *> m_appUsedTimeViewTimeItemList;
};

#endif // HEALTHPAGE_H
