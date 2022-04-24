#ifndef MQTTMANAGEMEN_H
#define MQTTMANAGEMEN_H

#include "lib/inc/mqttsvc.h"

MqttSvc* getMqttSvcHandle(void);
void initMqttSvc();
void deinitMqttSvc();
void mqttStart();
void mqttStop();
void mqttSetConf(mqttConfig &mqttConf);
void mqttSendMsg(mqttPublishPar &par);
void mqttSubscribeTopic(QString &topic);
void mqttUnsubscribeTopic(QString &topic);

#endif // MQTTMANAGEMEN_H
