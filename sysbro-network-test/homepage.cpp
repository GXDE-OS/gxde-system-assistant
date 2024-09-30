#include "homepage.h"
#include <QVBoxLayout>
#include <QApplication>
#include <QImageReader>
#include <QLabel>
#include "dlinkbutton.h"

DWIDGET_USE_NAMESPACE

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

HomePage::HomePage(QWidget *parent)
    : QWidget(parent),
      m_comboBox(new QComboBox),
      m_settings(new QSettings("sysbro", "network-test"))
{
    QVBoxLayout *layout = new QVBoxLayout;
    QLabel *iconLabel = new QLabel;
    QPixmap iconPixmap = renderSVG(":/images/network.svg", QSize(128, 128));
    iconLabel->setPixmap(iconPixmap);

    QLabel *splitLine = new QLabel;
    QPixmap splitPixmap = renderSVG(":/images/split_line.svg", QSize(214, 2));
    splitLine->setPixmap(splitPixmap);

    DLinkButton *btn = new DLinkButton("立即测速");
    QLabel *tipsLabel = new QLabel("测速前请关闭占用网络资源的软件");

    m_comboBox->addItem("服务器1");
    m_comboBox->addItem("服务器2");
    m_comboBox->addItem("服务器3");
    m_comboBox->addItem("服务器4（国外，国内可能无法连接）");
    m_comboBox->addItem("服务器5（国外，国内可能无法连接）");
    m_comboBox->setFocusPolicy(Qt::NoFocus);

    if (m_settings->contains("server_index")) {
        m_comboBox->setCurrentIndex(m_settings->value("server_index").toInt());
    } else {
        m_settings->setValue("server_index", QVariant(0));
    }

    layout->addSpacing(15);
    layout->addWidget(iconLabel, 0, Qt::AlignHCenter);
    layout->addSpacing(15);
    layout->addWidget(tipsLabel, 0, Qt::AlignHCenter);
    layout->addSpacing(5);
    layout->addWidget(m_comboBox, 0, Qt::AlignHCenter);
    layout->addSpacing(20);
    layout->addWidget(splitLine, 0, Qt::AlignHCenter);
    layout->addSpacing(20);
    layout->addWidget(btn, 0, Qt::AlignHCenter);
    layout->addStretch();

    setLayout(layout);

    connect(btn, &DLinkButton::clicked, this, [=] {
        emit startButtonClicked(m_comboBox->currentIndex());
    });
    connect(m_comboBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &HomePage::handleCurrentIndexChanged);
}

void HomePage::handleCurrentIndexChanged(int index)
{
    m_settings->setValue("server_index", QVariant(index));
}
