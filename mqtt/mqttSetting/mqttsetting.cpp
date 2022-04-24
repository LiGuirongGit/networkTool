#include "mqttsetting.h"
#include "ui_mqttsetting.h"
#include <QUuid>
#include <QDebug>

mqttSetting::mqttSetting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::mqttSetting)
{
    ui->setupUi(this);
    ui->nameLineEdit->setPlaceholderText(tr("请输入用户名"));
    ui->pwdLineEdit->setPlaceholderText(tr("请输入用户密码"));
    ui->addrLineEdit->setPlaceholderText(tr("请输入服务器地址"));
    ui->portLineEdit->setPlaceholderText(tr("请输入端口号"));
    ui->idLineEdit->setPlaceholderText(tr("请输入ID"));
    ui->qosBox->addItems({"0", "1", "2"});
    ui->idLineEdit->setText(getId());

    connect(ui->idBtn, SIGNAL(clicked()), this, SLOT(slotCreateId()));
    connect(ui->connectBtn, SIGNAL(clicked()), this, SLOT(slotConnect()));
    connect(ui->disconnectBtn, SIGNAL(clicked()), this, SLOT(slotDisconnect()));
    ui->connectBtn->setDisabled(false);
    ui->disconnectBtn->setDisabled(true);
}

mqttSetting::~mqttSetting()
{
    delete ui;
}

inline QString mqttSetting::getId() {
    QString id = QUuid::createUuid().toString();
    id.remove(0, 1);
    id.remove(id.size()-1, 1);
    return id;
}

void mqttSetting::slotCreateId() {
    ui->idLineEdit->setText(getId());
}

void mqttSetting::slotConnect(){
    mqttConfig par;
    par.clientId = ui->idLineEdit->text();
    par.name = ui->nameLineEdit->text();
    par.pwd = ui->pwdLineEdit->text();
    par.qos = ui->qosBox->currentIndex();
    par.serverAddr = ui->addrLineEdit->text() + ":" + ui->portLineEdit->text();

    MqttSvc *mqSvc = getMqttSvcHandle();
    if(mqSvc) {
        mqttSetConf(par);
        connect(mqSvc, SIGNAL(connectDone()), this, SLOT(slotConnectDon()));
        connect(mqSvc, SIGNAL(connectFailed()), this, SLOT(slotconnectFailed()));
        connect(mqSvc, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));
        mqttStart();
    } else {
        qDebug("[%s:%d]mqtt handle is nullptr", __func__, __LINE__);
    }
}

void mqttSetting::slotDisconnect(){
    mqttStop();
}

void mqttSetting::slotDisconnected() {
    ui->connectBtn->setDisabled(false);
    ui->disconnectBtn->setDisabled(true);
    QMessageBox::information(this, tr("提示"), tr("连接已断开"), QMessageBox::Yes);
    MqttSvc *mqSvc = getMqttSvcHandle();
    disconnect(mqSvc, SIGNAL(connectDone()), this, SLOT(slotConnectDon()));
    disconnect(mqSvc, SIGNAL(connectFailed()), this, SLOT(slotconnectFailed()));
    disconnect(mqSvc, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));
}

void mqttSetting::slotConnectDon() {
    ui->connectBtn->setDisabled(true);
    ui->disconnectBtn->setDisabled(false);
    QMessageBox::information(this, tr("提示"), tr("连接成功"), QMessageBox::Yes);
}

void mqttSetting::slotconnectFailed() {
    ui->connectBtn->setDisabled(false);
    ui->disconnectBtn->setDisabled(true);
    QMessageBox::warning(this, tr("错误"), tr("连接失败"), QMessageBox::Yes);
    MqttSvc *mqSvc = getMqttSvcHandle();
    disconnect(mqSvc, SIGNAL(connectDone()), this, SLOT(slotConnectDon()));
    disconnect(mqSvc, SIGNAL(connectFailed()), this, SLOT(slotconnectFailed()));
    disconnect(mqSvc, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));
}
