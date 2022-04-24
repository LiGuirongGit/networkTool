#include "mqttsubscribe.h"
#include "ui_mqttsubscribe.h"
#include <QDebug>

#define STYLE_DEL_STR "QPushButton{border-image: url(:/del.png)};"

mqttSubscribe::mqttSubscribe(QWidget *parent) :
    QWidget(parent),
    topicModel(nullptr),
    msgModel(nullptr),
    listItemDelegate(nullptr),
    curSelectRow(0),
    ui(new Ui::mqttSubscribe)
{
    ui->setupUi(this);
    initListview();
    connect(ui->subscribeBtn,SIGNAL(clicked()), this, SLOT(slotMqttSubscribe()));
    connect(ui->publishMsgBtn,SIGNAL(clicked()), this, SLOT(slotMqttPublishMsg()));
    MqttSvc *mqSvc = getMqttSvcHandle();
    connect(mqSvc, SIGNAL(subscribeDone(QString)), this, SLOT(slotsubscribeDone(QString)));
    connect(mqSvc, SIGNAL(unsubscribeDone(QString)), this, SLOT(slotUnsubscribeDone(QString)));
    connect(mqSvc, SIGNAL(mqttRecvMsgTopic(QString, QString)), this, SLOT(slotRecvMqttMsgTopic(QString, QString)));
}

mqttSubscribe::~mqttSubscribe()
{
    delete ui;
    if(topicModel) {
        delete topicModel;
        topicModel = nullptr;
    }
    if(msgModel) {
        delete msgModel;
        msgModel = nullptr;
    }
    if(listItemDelegate) {
        delete listItemDelegate;
        listItemDelegate = nullptr;
    }
}

void mqttSubscribe::initListview() {
    topicModel = new QStandardItemModel(this);
    msgModel = new QStandardItemModel(this);
    listItemDelegate = new mqttSubscribeDelegate(this);

    ui->msgListView ->setItemDelegate(listItemDelegate);
    ui->msgListView->setModel(msgModel);
    ui->msgListView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->topicListView ->setItemDelegate(listItemDelegate);
    ui->topicListView->setModel(topicModel);
    ui->topicListView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(ui->topicListView,SIGNAL(clicked(QModelIndex)), this, SLOT(slotTopicListViewForClicked(QModelIndex)));
    connect(ui->msgListView,SIGNAL(clicked(QModelIndex)), this, SLOT(slotMsgListViewForClicked(QModelIndex)));

}

void mqttSubscribe::updateTopicList(int row) {
    topicModel->setData(topicModel->index(row,0),
                        QVariant::fromValue(topicItems[row]), Qt::UserRole + 1);
    ui->topicListView->setModel(topicModel);
    ui->topicListView->update();
}

void mqttSubscribe::updateMsgList(int topicRow, int msgRow) {

    msgModel->setData(msgModel->index(msgRow,0),
                        QVariant::fromValue(msgItems[topicRow][msgRow]), Qt::UserRole + 1);

    ui->msgListView->setModel(msgModel);
    ui->msgListView->update();

}

void mqttSubscribe::slotMqttSubscribe() {
    QString topic = ui->topicLineEdit->text();
    if(!topic.isEmpty() && topicHash.find(topic) == topicHash.end()) {
        mqttSubscribeTopic(topic);
    }
}

void mqttSubscribe::slotsubscribeDone(QString topic) {
    if(!topic.isEmpty()){
        QPushButton *btn = new QPushButton;
        btn->setParent(ui->topicListView);
        btn->setProperty("BtnID", QString("%1").arg(topicItems.size()));
        btn->setStyleSheet(STYLE_DEL_STR);
        connect(btn, SIGNAL(clicked()), this, SLOT(slotDeleteTopic()));

        topicItems.append({topic, "0", btn, 0});
        QStandardItem *pItem = new QStandardItem;
        pItem->setData(QVariant::fromValue(topicItems[topicItems.size() - 1]), Qt::UserRole + 1);
        topicModel->appendRow(pItem);
        topicHash[topic] = 1;

        QList<itemProperty> msgItem;
        msgItem.append({"", "", NULL, 0});
        msgItems.append(msgItem);
    }
}

void mqttSubscribe::slotUnsubscribeDone(QString topic) {
    if(!topic.isEmpty()){
        int i = 0;
        bool isDel = false;
        for(i = 0; i<topicItems.count(); ++i) {
            if(!isDel && topicItems[i].titleText.compare(topic) == 0){
                topicItems.removeAt(i);
                isDel = true;
                msgModel->removeRow(i);
                break;
            } else if(isDel) {
                if(topicItems[i].widget) {
                    topicItems[i].widget->setProperty("BtnID", QString("%1").arg(i-1));
                }
            }
        }
        topicHash.remove(topic);
    }
}

void mqttSubscribe::slotDeleteTopic() {
    QPushButton *btn = dynamic_cast<QPushButton*>(sender());
    if(btn != Q_NULLPTR) {
       int id = btn->property("BtnID").toInt();
        qDebug() << topicItems[id].titleText;
        mqttUnsubscribeTopic(topicItems[id].titleText);
        topicHash.remove(topicItems[id].titleText);
        if(topicItems[id].widget){
            delete topicItems[id].widget;
            topicItems[id].widget = nullptr;
        }
        topicItems.removeAt(id);
        topicModel->removeRow(id);
        for(int i = id; i<topicItems.count(); ++i) {
            if(topicItems[i].widget){
                topicItems[i].widget->setProperty("BtnID", QString("%1").arg(i));
            }
        }

    }
}

void mqttSubscribe::slotTopicListViewForClicked(QModelIndex index) {
    static int32_t curCnt = -1, lastRow = -1;
    uint32_t row = index.row();
    curSelectRow = row;
    //if(msgItems[row].count() != curCnt){
        curCnt = msgItems[row].count();
        msgModel->clear();
        for(int listRow=0; listRow<msgItems[row].count(); ++listRow) {
            QStandardItem *pItem = new QStandardItem;
            pItem->setData(QVariant::fromValue(msgItems[row][listRow]), Qt::UserRole + 1);
            msgModel->appendRow(pItem);
        }
        if(topicItems[row].state.compare("1") == 0) {
            topicItems[row].state = "0";
            updateTopicList(row);
        }

   // }

    if(lastRow != row){
        if(lastRow != -1 && topicItems[lastRow].isSelect) {
            topicItems[lastRow].isSelect = false;
            updateTopicList(lastRow);
        }
        lastRow = row;
        if(!topicItems[row].isSelect) {
            topicItems[row].isSelect = true;
            updateTopicList(row);
        }
        ui->publishLineEdit->setText(topicItems[row].titleText);

    } else {
        qDebug("[%s:%d] select item no change", __func__, __LINE__);
    }
}

void mqttSubscribe::slotMsgListViewForClicked(QModelIndex index) {
    static int32_t curRow = -1;
    uint32_t row = index.row();

    if(curRow != row) {
        curRow = row;
    } else {
        qDebug("[%s:%d] select item no change", __func__, __LINE__);
        return;
    }
    ui->msgTextEdit->setText(index.data(Qt::UserRole+1).value<itemProperty>().titleText);
}

void mqttSubscribe::slotMqttPublishMsg() {
    mqttPublishPar par;
    par.msg = ui->publishTextEdit->toPlainText();
    par.topic = ui->publishLineEdit->text();
    if(!par.msg.isEmpty())
        mqttSendMsg(par);
}


void mqttSubscribe::slotRecvMqttMsgTopic(QString topic, QString msg){
    for(int i=0; i<topicItems.count(); ++i) {
        if(topicItems[i].titleText.compare(topic) == 0) {
            topicItems[i].state = "1";
            if(msgItems[i].count() >= 3) {
                msgItems[i].clear();
                msgItems[i].append({"", "", NULL, 0});
                msgModel->clear();
            }
            if(msgItems[i][msgItems[i].count()-1].titleText.isEmpty()) {
                msgItems[i][msgItems[i].count()-1].titleText = msg;
            } else {
                msgItems[i].push_back({msg, 0, NULL, 0});
            }
            updateTopicList(i);
            if(curSelectRow == i) {
                slotTopicListViewForClicked(topicModel->index(i,0));
            }
            break;
        }
    }
}
