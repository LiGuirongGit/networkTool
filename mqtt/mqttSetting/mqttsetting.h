#ifndef MQTTSETTING_H
#define MQTTSETTING_H

#include <QWidget>
#include <QMessageBox>
#include "../mqttSvc/mqttAPI.h"

namespace Ui {
class mqttSetting;
}

class mqttSetting : public QWidget
{
    Q_OBJECT

public:
    explicit mqttSetting(QWidget *parent = nullptr);
    ~mqttSetting();
private slots:
    void slotCreateId();
    void slotConnect();
    void slotDisconnect();
    void slotConnectDon();
    void slotconnectFailed();
    void slotDisconnected();

private:
    Ui::mqttSetting *ui;

    inline QString getId();
};

#endif // MQTTSETTING_H
