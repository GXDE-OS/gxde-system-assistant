#include "healthpage.h"
#include "healthpagemodel.h"
#include <QTimer>
#include <QVBoxLayout>
#include <QJsonObject>
#include <QScroller>
#include <QFile>

HealthPage::HealthPage(QWidget *parent)
    : QScrollArea(parent),
    m_mainWidget(new QWidget),
    m_totalTime(new QLabel),
    m_longestUsedApp(new QLabel),
    m_appInfo(new QLabel),
    m_timeChartView(new QChartView),
    m_timePieSeries(new QPieSeries),
    m_appUsedTimeView(new QGridLayout)

{
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(m_timeChartView);
    layout->addWidget(m_appInfo);
    layout->addWidget(new QLabel("<hr>"));
    layout->addLayout(m_appUsedTimeView);
    layout->setAlignment(Qt::AlignCenter);
    m_timeChartView->setMinimumHeight(this->height() / 2);

    m_mainWidget->setLayout(layout);
    this->setWidget(m_mainWidget);
    setWidgetResizable(true);

    m_appInfo->setAlignment(Qt::AlignCenter);
    m_longestUsedApp->setAlignment(Qt::AlignCenter);

    initChart();

    QTimer *timer = new QTimer();
    //timer->setInterval(30 * 1000);
    timer->setInterval(0.1 * 1000);
    connect(timer, &QTimer::timeout, this, &HealthPage::refresh);
    timer->start();
    refresh();
}

void HealthPage::initChart()
{
    QChart *chart = m_timeChartView->chart();
    for (int i = 0; i < 9; ++i) {
        QPieSlice *pie_slice = new QPieSlice(this);
        pie_slice->setLabelVisible(false);
        m_timePieSliceList.append(pie_slice);
        m_timePieSeries->append(pie_slice);
    }
    m_timePieSeries->setHoleSize(0.25);   //饼图中间空心的大小
    chart->legend()->setVisible(false);
    chart->setMargins(QMargins(0, 0, 0, 0));
    chart->addSeries(m_timePieSeries);

    connect(m_timePieSeries, &QPieSeries::clicked, this, &HealthPage::onPieSeriesClicked);

    m_timeChartView->setRenderHint(QPainter::Antialiasing);
}

void HealthPage::refresh()
{
    m_timeChartView->chart()->setTitle(tr("Total Time: ") +
                         secondToTimeText(HealthPageModel::totalTime()) + "\n");
    QFont font;
    font.setBold(true);
    m_timeChartView->chart()->setTitleFont(font);
    m_longestUsedApp->setText(tr("Longest Used App: ") +
                         HealthPageModel::longestUsedApp());
    refreshCharts();
}

void HealthPage::refreshCharts()
{
    // 计算饼图
    // 加载数据
    QJsonArray json = HealthPageModel::getPerAppStatJson();
    const int sliceCount = m_timePieSliceList.count();
    const int dataCount = json.count();
    for (int i = 0; i < sliceCount - 1 && i < dataCount; ++i) {
        if (!json[i].isObject()) {
            continue;
        }
        QJsonObject object = json[i].toObject();
        int time = object["time"].toInt();
        QString name = object["name"].toString();
        m_timePieSliceList[i]->setValue(time);
        m_timePieSliceList[i]->setLabel(name);
        if (i == 0) {
            m_timePieSliceList[i]->setLabelVisible(true);
            m_timePieSliceList[i]->setLabel(name + tr(" (Longest)"));
        }
    }
    // 计算 Others
    int otherTotalTime = 0;
    for (int i = sliceCount; i < dataCount; ++i) {
        if (!json[i].isObject()) {
            continue;
        }
        QJsonObject object = json[i].toObject();
        otherTotalTime += object["time"].toInt();
    }
    m_timePieSliceList.last()->setLabel(tr("Others"));
    m_timePieSliceList.last()->setValue(otherTotalTime);
    // 刷新应用列表
    for (auto i: m_appUsedTimeViewItemList) {
        i->setVisible(false);
    }
    for (auto i: m_appUsedTimeViewTimeItemList) {
        i->setVisible(false);
    }
    for (int i = 0; i < json.count(); ++i) {
        if (!json[i].isObject()) {
            continue;
        }
        QJsonObject object = json[i].toObject();
        int time = object["time"].toInt();
        QString name = object["name"].toString();
        QString icon = object["icon"].toString();
        if (m_appUsedTimeViewItemList.count() - 1 < i) {
            QLabel *item = new QLabel();
            QLabel *timeItem = new QLabel();
            timeItem->setAlignment(Qt::AlignRight);
            m_appUsedTimeView->addWidget(item, i, 0);
            m_appUsedTimeView->addWidget(timeItem, i, 1);
            m_appUsedTimeViewItemList << item;
            m_appUsedTimeViewTimeItemList << timeItem;
        }
        QLabel *item = m_appUsedTimeViewItemList[i];
        QLabel *timeItem = m_appUsedTimeViewTimeItemList[i];
        QIcon labelIcon;
        if (QFile::exists(icon)) {
            labelIcon = QIcon(icon);
        }
        else {
            labelIcon = QIcon::fromTheme(icon);
        }

        item->setPixmap(labelIcon.pixmap(item->font().pointSize(), item->font().pointSize()));
        item->setText(name);
        timeItem->setText(secondToTimeText(time));
        item->setVisible(true);
        timeItem->setVisible(true);
    }
}

QString HealthPage::secondToTimeText(int second)
{
    // 秒
    if (second < 60) {
        return QString::number(second) + tr("S");
    }
    // 分钟
    if (second >= 60 && second < 60 * 60) {
        return QString::number(int(second / 60)) + tr("min");
    }
    // 小时
    return QString::number(int(second / 60 / 60)) + tr("h") +
           QString::number(int(second / 60 % 60)) + tr("min");
}

void HealthPage::onPieSeriesClicked(QPieSlice *slice)
{
    for (auto i: m_timePieSliceList) {
        i->setExploded(false);
    }
    slice->setExploded(true);
    m_appInfo->setText(slice->label() + "\n" +
                       secondToTimeText(slice->value()));
}
