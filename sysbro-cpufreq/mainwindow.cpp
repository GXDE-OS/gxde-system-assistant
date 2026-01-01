#include "mainwindow.h"
#include <QDBusConnection>
#include <QVBoxLayout>
#include <QApplication>
#include <QImageReader>
#include <QPushButton>
#include <QRadioButton>
#include <QDBusMessage>
#include <dtitlebar.h>

static QPixmap renderSVG(const QString &path, const QSize &size)
{
    QImageReader reader;
    QPixmap pixmap;
    reader.setFileName(path);

    if (reader.canRead()) {
        const qreal ratio = qApp->devicePixelRatio();
        reader.setScaledSize(size * ratio);
        pixmap = QPixmap::fromImage(reader.read());
        pixmap.setDevicePixelRatio(ratio);
    } else {
        pixmap.load(path);
    }

    return pixmap;
}

MainWindow::MainWindow(QWidget *parent)
    : DMainWindow(parent),
    m_layout(new QVBoxLayout)
{
    QWidget *centralWidget = new QWidget;
    centralWidget->setLayout(m_layout);

    const qreal ratio = qApp->devicePixelRatio();
    QPixmap iconPixmap = QIcon::fromTheme("gxde-system-monitor").pixmap(QSize(128, 128) * ratio);
    iconPixmap.setDevicePixelRatio(ratio);

    QLabel *iconLabel = new QLabel();
    m_performanceMode = new QRadioButton(tr("高性能模式"));
    m_ondemandMode = new QRadioButton(tr("平衡模式"));
    m_powersaveMode = new QRadioButton(tr("节能模式"));
    m_otherMode = new QRadioButton(tr("其它模式"));

    m_performanceMode->setDisabled(true);
    m_ondemandMode->setDisabled(true);
    m_powersaveMode->setDisabled(true);
    for (QString i: GetModeList()) {
        if ("powersave" == i) {
            m_powersaveMode->setEnabled(true);
        }
        if ("ondemand" == i) {
            m_ondemandMode->setEnabled(true);
        }
        if ("performance" == i) {
            m_performanceMode->setEnabled(true);
        }
    }

    QString mode = GetMode();
    if (mode == "powersave") {
        m_powersaveMode->setChecked(true);
    }
    else if (mode == "performance") {
        m_performanceMode->setChecked(true);
    }
    else if (mode == "ondemand") {
        m_ondemandMode->setChecked(true);
    }
    else {
        m_otherMode->setChecked(true);
    }


    iconLabel->setPixmap(iconPixmap);
    m_otherMode->setDisabled(true);

    m_layout->addWidget(iconLabel, 0, Qt::AlignHCenter);
    m_layout->addSpacing(45);
    m_layout->addWidget(m_performanceMode, 0, Qt::AlignHCenter);
    m_layout->addWidget(m_ondemandMode, 0, Qt::AlignHCenter);
    //m_layout->addSpacing(30);
    m_layout->addWidget(m_powersaveMode, 0, Qt::AlignHCenter);
    m_layout->addWidget(m_otherMode, 0, Qt::AlignHCenter);
    m_layout->addSpacing(30);

    setEnableWindowBackground(true);

    setWindowTitle(tr("电源计划"));
    titlebar()->setTitle(tr("电源计划"));
    setWindowFlags(Qt::Dialog);
    setFixedSize(350, 420);
    setCentralWidget(centralWidget);

    connect(m_performanceMode, &QRadioButton::toggled, this, &MainWindow::ModeRadioTriggered);
    connect(m_ondemandMode, &QRadioButton::toggled, this, &MainWindow::ModeRadioTriggered);
    connect(m_powersaveMode, &QRadioButton::toggled, this, &MainWindow::ModeRadioTriggered);
    connect(m_otherMode, &QRadioButton::toggled, this, &MainWindow::ModeRadioTriggered);
}

MainWindow::~MainWindow()
{

}

void MainWindow::ModeRadioTriggered()
{
    if (m_performanceMode->isChecked()) {
        SetMode("performance");
    }
    else if (m_powersaveMode->isChecked()) {
        SetMode("powersave");
    }
    else if (m_ondemandMode->isChecked()) {
        SetMode("ondemand");
    }
    else {
        SetMode("powersave");
    }
}

void MainWindow::SetMode(QString mode)
{
    QDBusMessage dbus = QDBusMessage::createMethodCall(POWER_DESTINATION,
                                                       POWER_PATH,
                                                       POWER_INTERFACE,
                                                       "SetGovernor");
    QList<QVariant> arlist;
    arlist << mode;
    dbus.setArguments(arlist);
    QDBusMessage res = QDBusConnection::systemBus().call(dbus);
    qDebug() << res;
}

QString MainWindow::GetMode()
{
    QDBusMessage dbus = QDBusMessage::createMethodCall(POWER_DESTINATION,
                                                       POWER_PATH,
                                                       POWER_INTERFACE,
                                                       "Governor");
    QDBusMessage res = QDBusConnection::systemBus().call(dbus);
    if (res.arguments().count() <= 0) {
        return "";
    }
    return res.arguments().first().toString();
}


QStringList MainWindow::GetModeList()
{
    QDBusMessage dbus = QDBusMessage::createMethodCall(POWER_DESTINATION,
                                                       POWER_PATH,
                                                       POWER_INTERFACE,
                                                       "GetGovernorList");
    QDBusMessage res = QDBusConnection::systemBus().call(dbus);
    if (res.arguments().count() <= 0) {
        return QStringList();
    }
    return res.arguments().first().toStringList();
}
