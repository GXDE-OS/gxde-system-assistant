#include <DApplication>
#include <DWidgetUtil>
#include "mainwindow.h"

DWIDGET_USE_NAMESPACE

int main(int argc, char *argv[])
{
    DApplication::loadDXcbPlugin();
    DApplication a(argc, argv);

    a.setAttribute(Qt::AA_UseHighDpiPixmaps);
    a.loadTranslator();
    a.setOrganizationName("GXDE");
    a.setApplicationVersion(DApplication::buildVersion("1.0"));
    a.setApplicationAcknowledgementPage("https://gitee.com/GXDE-OS");
    a.setProductIcon(QIcon::fromTheme("gxde-system-monitor"));
    a.setProductName(DApplication::translate("Main", "电源计划"));
//    a.setApplicationDescription(DApplication::translate("Main", "网络测速"));

    if (!a.setSingleInstance("sysbro-cpufreq")) {
        return 0;
    }

    MainWindow w;
    w.show();

    Dtk::Widget::moveToCenter(&w);

    return a.exec();

}
