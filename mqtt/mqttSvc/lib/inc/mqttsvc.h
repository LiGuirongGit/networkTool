#ifndef MQTTSVC_H
#define MQTTSVC_H

#include <QDebug>
#include <QThread>
#include <QQueue>

extern "C" {
#include "mongoose/mongoose.h"
}

struct mqttConfig{
    QString name;
    QString pwd;
    QString clientId;
    QString topic;
    QString serverAddr;
    quint8 qos;
};

struct mqttPublishPar{
    QString topic;
    QString msg;
};

class MqttSvc : public QThread
{
    Q_OBJECT
public:
    explicit MqttSvc(QObject *parent = nullptr);
    MqttSvc(QObject *, mqttConfig &);
    void run();
    void stop();
    void setMqttConf(mqttConfig &p);
    void mqttPublish(mqttPublishPar &par);
    void mqttSubscribe(QString &topic);
    void mqttDelTopic(QString &topic);

signals:
    void errSignal(QString);
    void connectDone();
    void connectFailed();
    void disconnected();
    void subscribeDone(QString);
    void unsubscribeDone(QString);
    void mqttRecvMsgTopic(QString, QString);

public slots:
private:
    bool isStop;
};

#endif // MQTTSVC_H
