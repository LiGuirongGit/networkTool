#include "networkTool.h"
#include "ui_networkTool.h"

networkTool::networkTool(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::networkTool)
{
    ui->setupUi(this);
    ui->tabWidget->setTabText(1, tr("MQTT连接设置"));
    ui->tabWidget->setTabText(2, tr("MQTT"));
    ui->tabWidget->setCurrentIndex(1);
    ui->tabWidget->setTabEnabled(2, false);

    MqttSvc *mqSvc = getMqttSvcHandle();
    connect(mqSvc, SIGNAL(connectDone()), this, SLOT(slotConnectDone()));
    connect(mqSvc, SIGNAL(disconnected()), this, SLOT(slotDisconnected()));
}

networkTool::~networkTool()
{
    delete ui;
}

void networkTool::slotConnectDone() {
    ui->tabWidget->setTabEnabled(2, true);
}

void networkTool::slotDisconnected() {
    ui->tabWidget->setTabEnabled(2, false);
}
