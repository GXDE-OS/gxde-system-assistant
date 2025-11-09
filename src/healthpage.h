#ifndef HEALTHPAGE_H
#define HEALTHPAGE_H

#include <QWidget>
#include <QLabel>
#include <QChartView>
#include <QPieSeries>
#include <QListWidget>
#include <QGridLayout>

class HealthPage : public QScrollArea
{
    Q_OBJECT
public:
    explicit HealthPage(QWidget *parent = nullptr);

private:
    void refresh();
    void refreshCharts();
    void initChart();
    void onPieSeriesClicked(QtCharts::QPieSlice *slice);
    QString secondToTimeText(int second);

    QWidget *m_mainWidget;
    QLabel *m_totalTime;
    QLabel *m_longestUsedApp;
    QLabel *m_appInfo;
    QGridLayout *m_appUsedTimeView;
    QtCharts::QChartView *m_timeChartView;
    QtCharts::QPieSeries *m_timePieSeries;
    QList<QtCharts::QPieSlice *> m_timePieSliceList;
    //QList<QListWidgetItem *> m_appUsedTimeViewItemList;
    QList<QLabel *> m_appUsedTimeViewItemList;
    QList<QLabel *> m_appUsedTimeViewTimeItemList;
};

#endif // HEALTHPAGE_H
