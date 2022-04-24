#ifndef MQTTSUBSCRIBE_H
#define MQTTSUBSCRIBE_H

#include <QWidget>
#include <QStandardItemModel>
#include <QHash>
#include "mqttsubscribedelegate.h"
#include "../mqttSvc/mqttAPI.h"

namespace Ui {
class mqttSubscribe;
}

class mqttSubscribe : public QWidget
{
    Q_OBJECT

public:
    explicit mqttSubscribe(QWidget *parent = nullptr);
    ~mqttSubscribe();
private slots:
    void slotMqttSubscribe();
    void slotTopicListViewForClicked(QModelIndex index);
    void slotMsgListViewForClicked(QModelIndex index);
    void slotsubscribeDone(QString topic);
    void slotDeleteTopic();
    void slotUnsubscribeDone(QString topic);
    void slotMqttPublishMsg();
    void slotRecvMqttMsgTopic(QString topic, QString msg);

private:
    Ui::mqttSubscribe *ui;
    QList<itemProperty> topicItems;
    QList<QList<itemProperty>> msgItems;
    QHash<QString, int> topicHash;
    QStandardItemModel *topicModel;
    QStandardItemModel *msgModel;
    mqttSubscribeDelegate *listItemDelegate;
    uint32_t curSelectRow;

    void initListview();
    void updateTopicList(int row);
    void updateMsgList(int topicRow, int msgRow);
};

#endif // MQTTSUBSCRIBE_H
