#include "mqttAPI.h"

static MqttSvc* mqttSvc = nullptr;

MqttSvc* getMqttSvcHandle(void) {
    return mqttSvc;
}

void initMqttSvc() {
    if(!mqttSvc){
        mqttSvc = new MqttSvc;
    }
}

void mqttSetConf(mqttConfig &mqttConf) {
    if(mqttSvc){
        mqttSvc->setMqttConf(mqttConf);
    }
}

void deinitMqttSvc() {
    if(mqttSvc) {
        mqttSvc->stop();
        mqttSvc->wait(200);
        delete mqttSvc;
        mqttSvc = nullptr;
    }
}

void mqttStart() {
    if(mqttSvc) {
        mqttSvc->start();
    } else {
        qDebug("[%s:%d]mqtt handle is nullptr", __func__, __LINE__);
    }
}
void mqttStop() {
    if(mqttSvc) {
        mqttSvc->stop();
        mqttSvc->wait(200);
    } else {
        qDebug("[%s:%d]mqtt handle is nullptr", __func__, __LINE__);
    }
}

void mqttSendMsg(mqttPublishPar &msg) {
    if(mqttSvc) {
        mqttSvc->mqttPublish(msg);
    } else {
        qDebug("[%s:%d]mqtt handle is nullptr", __func__, __LINE__);
    }
}

void mqttSubscribeTopic(QString &topic) {
    if(mqttSvc) {
        mqttSvc->mqttSubscribe(topic);
    } else {
        qDebug("[%s:%d]mqtt handle is nullptr", __func__, __LINE__);
    }
}

void mqttUnsubscribeTopic(QString &topic) {
    if(mqttSvc) {
        mqttSvc->mqttDelTopic(topic);
    } else {
        qDebug("[%s:%d]mqtt handle is nullptr", __func__, __LINE__);
    }
}
