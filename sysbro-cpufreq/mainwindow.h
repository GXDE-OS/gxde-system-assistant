#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <DMainWindow>
#include <QVBoxLayout>
#include <QLabel>
#include <dspinner.h>
#include <QRadioButton>

#define POWER_DESTINATION "com.gxde.daemon.power"
#define POWER_PATH "/com/gxde/daemon/power/cpu"
#define POWER_INTERFACE "com.gxde.daemon.power.cpu"

DWIDGET_USE_NAMESPACE

class MainWindow : public DMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QStringList GetModeList();
    QString GetMode();
    void SetMode(QString mode);
    void ModeRadioTriggered();

    QVBoxLayout *m_layout;

    QRadioButton *m_performanceMode;
    QRadioButton *m_ondemandMode;
    QRadioButton *m_powersaveMode;
    QRadioButton *m_otherMode;

};

#endif // MAINWINDOW_H
